/*
* maker:风小松
*
* time:2021 7.22
*
* */
/*
* 本程序有两种循迹模式，一种是全差速转弯循迹模式，一种是全舵机转向模式。本人两种模式都尝试过，舵机转向模式效果比较好
*
*
* 硬件：一块msp4305529板子，四路红外对管循迹模块，直流减速小电机（小拇指那么大那种），一个tb6612电机驱动模块（供电8V），两个降压模块（一个由12v降压到5V给单片机使用，另一个降压到8V给TB6612使用，电机和单片机分电源）
* ，一个12V的电池包，一个15kg数字舵机（用于舵机转向），一个oled显示屏（IIC通信）
* 一个mpu6050(用于适应不同倾斜角度的坡道)具体如下：
* 根据实验测试发现，随着坡道倾斜角度的增加，小车在转90度大弯时所需要的前轮转向幅度越小，所以本程序会在小车行驶前，也就是按下启动按键的实验、后先用陀螺仪检测
* 板子的倾斜角度，从而修改最大转弯舵机旋转角度。
*
*
*
*
* 本车子为前后驱动，左边两个电机并联，右边两个电机也并联，没有使用编码器，调速为开环，因为减速电机的运行速度比较稳定，通过简单的调节pwm就可以比较精准地调节速度
* 车子能够上40度的坡
*
*
* 接线：
* 4.1 4.2 mpu6050数据线；6.0 6.1 6.3 6.4 借循迹模块；3.5 3.6 oled数据线；2.0舵机信号线，1.2 1.3 tb6612pwm, 2.5 2.4 1.5 4.3 tb6612正反转控制线
*
*
* 如有任何疑惑可以联系本人：qq 1147176261 欢迎一起技术交流
*
*/


#include "stdio.h"
#include "MPU6050.h"
#include <msp430f5529.h>
#include "Black_All.h"
#include "Black_OLED.h"
#include "PID.h"
int i=10,a=0,sight=0;
int pwm0=16;//16
int pwm1=30;
float pwmgo=12.24;//12.24
//int pwm0_0=20;
//int pwm0_1=40;//舵机和控速度参数
int line=0;
int rightsight=0;
int leftsight=0;//大幅度转向标志
int duojidafuduzuo=100,duojidafuduyou=100;//舵机转向大幅度程度
int cnt1=0;//记录编码器距离
int cnt2=0;
extern float v1,v2,Target;

int cal(int time)
{
  int a=100/time;
  double b=81.64/a;
  return (int)((100/b)/1.5);
}
void initClockTo16MHz()
{
  UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO
  __bis_SR_register(SCG0);                  // Disable the FLL control loop
  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
  UCSCTL1 = DCORSEL_5;                      // Select DCO range 16MHz operation
  UCSCTL2 = FLLD_0 + 487;                   // Set DCO Multiplier for 16MHz
  // (N + 1) * FLLRef = Fdco
  // (487 + 1) * 32768 = 16MHz
  // Set FLL Div = fDCOCLK
  __bic_SR_register(SCG0);                  // Enable the FLL control loop
  
  // Worst-case settling time for the DCO when the DCO range bits have been
  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
  // UG for optimization.
  // 32 x 32 x 16 MHz / 32,768 Hz = 500000 = MCLK cycles for DCO to settle
  __delay_cycles(500000);//
  // Loop until XT1,XT2 & DCO fault flag is cleared
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG); // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                          // Clear fault flags
  }while (SFRIFG1&OFIFG);                         // Test oscillator fault flag
}
void catinit(void)
{
  OUTPUT_GPIO_Config(8,2);//下左
  OUTPUT_GPIO_Config(3,7);
  
  OUTPUT_GPIO_Config(2,2);//上右
  OUTPUT_GPIO_Config(7,4);
  
  OUTPUT_GPIO_Config(2,6);//上左
  OUTPUT_GPIO_Config(2,3);
  
  OUTPUT_GPIO_Config(4,0);//下右
  OUTPUT_GPIO_Config(4,3);
  
  INPUT_GPIO_Config(6,0);
  INPUT_GPIO_Config(6,1);
  INPUT_GPIO_Config(6,2);
  INPUT_GPIO_Config(6,3);
}
void front(void)
{
  OUTPUT_GPIO_State(2,2,1);//上右
  OUTPUT_GPIO_State(7,4,0);
  OUTPUT_GPIO_State(4,0,1);//下右
  OUTPUT_GPIO_State(4,3,0);
  
  
  OUTPUT_GPIO_State(2,6,1);//upzuo
  OUTPUT_GPIO_State(2,3,0);
  OUTPUT_GPIO_State(8,2,1);//downzuo
  OUTPUT_GPIO_State(3,7,0);
}

void left(void)
{
  OUTPUT_GPIO_State(2,2,1);//4.3上右
  OUTPUT_GPIO_State(7,4,0);
  OUTPUT_GPIO_State(4,0,1);//4.3下右
  OUTPUT_GPIO_State(4,3,0);
  
  
  OUTPUT_GPIO_State(2,6,0);//upzuo
  OUTPUT_GPIO_State(2,3,1);
  OUTPUT_GPIO_State(8,2,0);//zuo
  OUTPUT_GPIO_State(3,7,1);
}
void right(void)
{
  OUTPUT_GPIO_State(2,2,0);//上右
  OUTPUT_GPIO_State(7,4,1);
  OUTPUT_GPIO_State(4,0,0);//下右
  OUTPUT_GPIO_State(4,3,1);
  
  
  OUTPUT_GPIO_State(2,6,1);//upzuo
  OUTPUT_GPIO_State(2,3,0);
  OUTPUT_GPIO_State(8,2,1);//zuo
  OUTPUT_GPIO_State(3,7,0);
}
void back(void)
{
  OUTPUT_GPIO_State(2,2,0);//4.3上右
  OUTPUT_GPIO_State(7,4,1);
  OUTPUT_GPIO_State(4,0,0);//4.3下右
  OUTPUT_GPIO_State(4,3,1);
  
  
  OUTPUT_GPIO_State(2,6,0);//upzuo
  OUTPUT_GPIO_State(2,3,1);
  OUTPUT_GPIO_State(8,2,0);//zuo
  OUTPUT_GPIO_State(3,7,1);
}
void stop(void)
{
  OUTPUT_GPIO_State(2,2,0);//上右
  OUTPUT_GPIO_State(7,4,0);
  OUTPUT_GPIO_State(4,0,0);//下右
  OUTPUT_GPIO_State(4,3,0);
  
  
  OUTPUT_GPIO_State(2,6,0);//上左
  OUTPUT_GPIO_State(2,3,0);
  OUTPUT_GPIO_State(8,2,0);//下左
  OUTPUT_GPIO_State(3,7,0);
}
void you(void)
{
  
  PWM0_State(pwm1);
  PWM1_State(pwm0);
  PWM2_State(pwm1);
  PWM3_State(pwm0);
}

void zuo(void)
{
  
  PWM0_State(pwm0);
  PWM1_State(pwm1);
  PWM2_State(pwm0);
  PWM3_State(pwm1);
}
void qian(void)
{
  PWM0_State(pwmgo);
  PWM1_State(pwmgo);
  PWM2_State(pwmgo);
  PWM3_State(pwmgo);
}
void ting(void)
{
  PWM0_State(0);
  PWM1_State(0);
  PWM2_State(0);
  PWM3_State(0);
}
void quansujianjin(void)
{PWM0_State(100);
PWM1_State(100);
PWM2_State(100);
PWM3_State(100);
}
void xunji_0(void)//全差速循迹
{
  if(((P6IN & BIT0) || (P6IN & BIT3)) && ((P6IN & BIT4) && (P6IN & BIT1)))//全1 全不亮
  {go();ting();a=0;}               
  //else if( (((P6IN & BIT0) || (P6IN & BIT1) || (P6IN & BIT3) )== 0) && (P6IN & BIT4))//1左灭一个
  //{left();zuo();}//l反了
  else if( ((P6IN & BIT1 || P6IN & BIT3 || P6IN & BIT4 )== 0) && (P6IN & BIT0))//1右灭一个
  {right();you();delay(100);}
  else if((((P6IN & BIT0) || (P6IN & BIT1) || (P6IN & BIT3) || (P6IN & BIT4)) == 0)||(P6IN & BIT1)||(P6IN & BIT3))//全0 全亮
  {go();qian();} 
  //else if( (((P6IN & BIT0) || (P6IN & BIT1) )== 0) && (P6IN & BIT4))//1最左灭一个
  //{right();PWM0_State(pwm0_0);PWM1_State(pwm0_1);delay(300);}
  //else if( (((P6IN & BIT3) || (P6IN & BIT4) )== 0) && (P6IN & BIT0))//1最右灭一个
  //{right();you();PWM0_State(pwm0_1);PWM1_State(pwm0_0);PWM2_State(pwm0_1);PWM3_State(pwm0_0);delay(500);}
  else ting();
}



/*
* 没有检测黑线时候亮电平状态为0，检测到黑线反之
* 中间左边灭就往左转，右边灭就往右边转
* 最旁边两个，如果最左边检测到就把舵机向左打满直到中间右边检测到黑线为止；如果最右边检测到反之
*
*/
//void xunji(void)//全舵机转向循迹
//{
//  if((P6IN & BIT1) && (P6IN & BIT3) )//中间两个1 中间两个不亮停
//  {
//    ting();
//    a=0;
//    TimerA1_1_SetPWM(20000*1.4/20);//舵机前
//  }
//  else if((((P6IN & BIT0) || (P6IN & BIT1) || (P6IN & BIT3) || (P6IN & BIT4)) == 0)&&((!rightsight)&(!leftsight)))//全0 全亮没大幅度转向标志 跑
//  {
//    qian();
//    TimerA1_1_SetPWM(20000*1.4/20);//舵机前
//  }
//  else if( (((P6IN & BIT0) || (P6IN & BIT1) || (P6IN & BIT4) )== 0) && (P6IN & BIT3) )//1右灭一个
//  {
//    if(rightsight ==0)
//      TimerA1_1_SetPWM(20000*1.7/20);//舵机右
//    delay(50);
//    leftsight=0;//左标志清零
//  }
//  else if( ((P6IN & BIT0 || P6IN & BIT3 || P6IN & BIT4 )== 0) && (P6IN & BIT1))//1左灭一个
//  {
//    if(leftsight ==0)
//      TimerA1_1_SetPWM(20000*1.1/20);//舵机左
//    delay(50);
//    rightsight=0;//右标志清零
//  }
//  else if( (((P6IN & BIT0) || (P6IN & BIT1) )== 0) && (P6IN & BIT4))//1最右灭一个
//  {
//    TimerA1_1_SetPWM(2100-10*duojidafuduyou);//舵机大幅度右(1000*(2.0-0.7*duojidafuduyou/100))
//    rightsight=1;
//  }
//  else if( (((P6IN & BIT3) || (P6IN & BIT4) )== 0) && (P6IN & BIT0))//1最左灭一个
//  {
//    TimerA1_1_SetPWM(500+9*duojidafuduyou);//舵机大幅度左20000*0.5*duojidafuduyou/(20*100)=1000*（0.5+0.9*duojidafuduyou/100）//
//    leftsight=1;
//  }
//  else
//    ;
//}
void interrupt_two_key(void)
{
  P1DIR |= BIT0;
  P4DIR |= BIT7;            //P1.0，P4.7口设置为输出
  P1OUT &= ~BIT0;
  P4OUT |= BIT7;          //初始化P1.0,P4.7输出低电平（初始化LED灯为灭）
  
  P1DIR &= ~BIT1;
  P2DIR &= ~BIT1;          //设置P1.1,P2.1口为输入
  
  P1REN |= BIT1;
  P2REN |= BIT1;
  
  P1OUT |= BIT1;
  P2OUT |= BIT1;          //配置P1.1,P2.1上拉电阻
  
  //  P1IES |= BIT1;
  //  P2IES |= BIT1;          //P1.1,P2.1下降沿触发中断
  //  
  //  P1IE |= BIT1;
  //  P2IE |= BIT1;           //P1.1,P2.1中断使能
  //  
  //  P1IFG &= ~BIT1;
  //  P2IFG &= ~BIT1;        //清除中断标志位
  
  //_EINT();      //打开全局中断允许中断嵌套
}
void ceshu_init()
{
  P1IE |= BIT6;
  
  P1IES |= BIT6;
  P1IES &= ~BIT6;
  P1IFG &= ~BIT6;
  P1REN |=  BIT6;
  P1OUT |=  BIT6;
  
  P2IE |= BIT7;
  P2IES |= BIT7;
  P2IES &= ~BIT7;
  P2IFG &= ~BIT7;
  P2REN |=  BIT7;
  P2OUT |=  BIT7;
}

void main(void)
{
//  int k=0;
//  int result,ax,ay,az,gx,gy,gz;
  //OLED_Init();//oled初始化
  //    P6DIR |= BIT3+BIT4;
  WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
  //initClockTo16MHz();
//  P1DIR|=0x01;
//  //_EINT();//开总中断，作用相当于GIE
//  TA1CCTL0=CCIE;
//  //开启比较中断（比较/捕获中断使能控制位）（以后无论使用定时器溢出中断还是比较捕获中断，都建议打开这个中断控制位）
//  TA1CTL=TASSEL0+MC_1+TACLR;//TASSEL0参考时钟ACLK、增计数模式、预先清空TAR
//  TA1CCR0=32767;//中断服务程序执行的时间间隔大约1s
  
  
//  OLED_Clear();//oled屏幕清零
//  interrupt_two_key();//两个按键初始化
//  ceshu_init();//编码器初始化
//  __bis_SR_register( GIE); 
  catinit();//小车初始化
  PWM0_Config();//1.2 pwm调速
  PWM1_Config();//1.3 pwm调速
  PWM2_Config();//1.4 pwm调速
  PWM3_Config();//1.5
  PWM0_State(100);
  PWM1_State(100);
  PWM2_State(100);
  PWM3_State(100);
    
  //TimerA1_1_PWM_Init(100,20000);//舵机
  
  //        TimerA1_1_SetPWM(20000*0.5/20);//舵机左
  //        TimerA1_1_SetPWM(20000*1.43/20);//舵机前
  //        TimerA1_1_SetPWM(20000*2.5/20);//舵机右

  int time=10;
  //OLED_ShowNum(10,0,Get6050Data(ACCEL_XOUT_H),10,16);//显示初始角度
  //duojidafuduyou=(GetData(ACCEL_XOUT_H)*50/13000);//陀螺仪调整最大极限舵机转向
  while(1)
  {
    int n=0;
    if((P1IN&BIT1)==0)
    {
      for(int j=0;j<=1000;j++)       //P1.1为按下状态消抖
      {
        if((P1IN&BIT1)==0)
          n++;
      }
      P4OUT^=BIT7;
      if(n>=900) 
      {
        if(sight==0)//菜单选择
        {
          sight=1;
          OLED_ShowString(0,4,"time:    ",16);
        }
        else if(sight==1)
        {
          sight=0;
          OLED_ShowString(0,4,"pleass:   ",16);
        }
      }
    }
    if(((P2IN&BIT1)==0)&&(sight==0))
    {
      n=0;
      for(int j=0;j<=1000;j++)   //按键的消抖功能：检测1000次，将<900的过滤掉（由于按键抖动产生的毛刺干扰）
      {
        if((P2IN&BIT1)==0)
          n++;
      }
      P1OUT^=BIT0;
      if(n>=900)
      {
        OLED_ShowString(0,4,"go      ",16);//发车
        a=1;
        rightsight=0;//右标志清零
        leftsight=0;//左标志清零
        //x=Get6050Data(ACCEL_XOUT_H);//检测初始板子角度
        //OLED_ShowNum(10,0,x,10,16);
        //duojidafuduyou=(x*50/13000);//陀螺仪调整最大极限舵机转向
        break;
      }
    }
    else if(((P2IN&BIT1)==0)&&(sight==1))
    {
      for(int j=0;j<=1000;j++)   //按键的消抖功能：检测1000次，将<900的过滤掉（由于按键抖动产生的毛刺干扰）
      {
        if((P2IN&BIT1)==0)
          n++;
      }
      P1OUT|=BIT0;
      if(n>=900)
      {
        if(i<20) i++,time++;
        else i=10;
        OLED_ShowString(0,4,"time:",16);
        switch(i)//调速组合
        {
        case 10 : pwmgo=12.24;OLED_ShowNum(40,4,i,8,16);Target=10.0; break;
        case 11 : pwmgo=11.12;OLED_ShowNum(40,4,i,8,16);Target=9.09; break;
        case 12 : pwmgo=10.2;OLED_ShowNum(40,4,i,8,16);Target=8.33;break;
        case 13 : pwmgo=12.24/1.3;OLED_ShowNum(40,4,i,8,16);Target=7.69; break;
        case 14 : pwmgo=12.24/1.4;OLED_ShowNum(40,4,i,8,16);Target=7.14; break;
        case 15 : pwmgo=12.24/1.5;OLED_ShowNum(40,4,i,8,16);Target=6.66;break;
        case 16 : pwmgo=12.24/1.6;OLED_ShowNum(40,4,i,8,16);Target=6.25; break;
        case 17 : pwmgo=12.24/1.7;OLED_ShowNum(40,4,i,8,16);Target=5.88; break;
        case 18 : pwmgo=12.24/1.8;OLED_ShowNum(40,4,i,8,16);Target=5.56; break;
        case 19 : pwmgo=12.24/1.9;OLED_ShowNum(40,4,i,8,16);Target=5.26;break;
        case 20 : pwmgo=12.24/2.0;OLED_ShowNum(40,4,i,8,16);Target=5.0; break;
        }
      }
    }
    
  }
  go();
  delay_nus(1000);
  while(1)
  {
    go();
    //qian();
    if(a==1)
    {
      //xunji();//全舵机转向循迹
      xunji_0();//全差速循迹
    }
  }
}



#pragma vector = PORT1_VECTOR    //S1中断服务函数
__interrupt void P1_ISR()
{
  
  cnt1++;
  
  P1IFG &=~ BIT6;
}
#pragma vector = PORT2_VECTOR    //S1中断服务函数
__interrupt void P2_ISR()
{
  
  cnt2++;
  
  P2IFG &=~ BIT7;
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
  v1=cnt1*3.14/20*6.5;//cm/s
  v2=cnt2*3.14/20*6.5; 

  
  cnt1=0;
  
  cnt2=0;
}

