#include "stdio.h"
#include <msp430f5529.h>
#include "Black_All.h"
#include "oled.h"
int pwm1_L=45;
int pwm1_H=45;
int pwm1_go=30;
int pwm2_L=30;
int pwm2_H=70;
int pwm2_go=50;
int mode=0;//模式
int flag=0;//发车
int cycle=0;//旋转圈数
int cn=0;//cn防止检测到起点误加
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
  
  OUTPUT_GPIO_Config(2,4);//下右
  OUTPUT_GPIO_Config(2,5);
  
  INPUT_GPIO_Config(6,0);
  INPUT_GPIO_Config(6,1);
  INPUT_GPIO_Config(6,2);
  INPUT_GPIO_Config(6,3);
}
void front(void)
{
  OUTPUT_GPIO_State(2,2,1);//上右
  OUTPUT_GPIO_State(7,4,0);
  OUTPUT_GPIO_State(2,4,1);//下右
  OUTPUT_GPIO_State(2,5,0);
  
  
  OUTPUT_GPIO_State(2,6,1);//upzuo
  OUTPUT_GPIO_State(2,3,0);
  OUTPUT_GPIO_State(8,2,1);//downzuo
  OUTPUT_GPIO_State(3,7,0);
}

void left(void)
{
  OUTPUT_GPIO_State(2,2,1);//4.3上右
  OUTPUT_GPIO_State(7,4,0);
  OUTPUT_GPIO_State(2,4,1);//4.3下右
  OUTPUT_GPIO_State(2,5,0);
  
  
  OUTPUT_GPIO_State(2,6,0);//upzuo
  OUTPUT_GPIO_State(2,3,1);
  OUTPUT_GPIO_State(8,2,0);//zuo
  OUTPUT_GPIO_State(3,7,1);
}
void right(void)
{
  OUTPUT_GPIO_State(2,2,0);//上右
  OUTPUT_GPIO_State(7,4,1);
  OUTPUT_GPIO_State(2,4,0);//下右
  OUTPUT_GPIO_State(2,5,1);
  
  
  OUTPUT_GPIO_State(2,6,1);//upzuo
  OUTPUT_GPIO_State(2,3,0);
  OUTPUT_GPIO_State(8,2,1);//zuo
  OUTPUT_GPIO_State(3,7,0);
}
void back(void)
{
  OUTPUT_GPIO_State(2,2,0);//4.3上右
  OUTPUT_GPIO_State(7,4,1);
  OUTPUT_GPIO_State(2,4,0);//4.3下右
  OUTPUT_GPIO_State(2,5,1);
  
  
  OUTPUT_GPIO_State(2,6,0);//upzuo
  OUTPUT_GPIO_State(2,3,1);
  OUTPUT_GPIO_State(8,2,0);//zuo
  OUTPUT_GPIO_State(3,7,1);
}
void stop(void)
{
  OUTPUT_GPIO_State(2,2,0);//上右
  OUTPUT_GPIO_State(7,4,0);
  OUTPUT_GPIO_State(2,4,0);//下右
  OUTPUT_GPIO_State(2,5,0);
  
  
  OUTPUT_GPIO_State(2,6,0);//上左
  OUTPUT_GPIO_State(2,3,0);
  OUTPUT_GPIO_State(8,2,0);//下左
  OUTPUT_GPIO_State(3,7,0);
}
void you(int pwm1,int pwm0)
{ 
  PWM0_State(pwm1);//2.0上左
  PWM1_State(pwm0);//1.3上右
  PWM2_State(pwm1);//1.4下左
  PWM3_State(pwm0);//1.5下右
}
void big_you(int pwm1,int pwm0)
{
  PWM0_State(pwm1);
  PWM1_State(pwm0);
  PWM2_State(pwm1);
  PWM3_State(pwm0);
}
void zuo(int pwm1,int pwm0)
{
  
  PWM0_State(pwm0);
  PWM1_State(pwm1);
  PWM2_State(pwm0);
  PWM3_State(pwm1);
}
void big_zuo(int pwm1,int pwm0)
{
  
  PWM0_State(pwm1);
  PWM1_State(pwm0);
  PWM2_State(pwm1);
  PWM3_State(pwm0);
}
void qian(void )
{
  PWM0_State(pwm1_go);
  PWM1_State(pwm1_go);
  PWM2_State(pwm1_go);
  PWM3_State(pwm1_go);
}
void ting(void)
{
  PWM0_State(0);
  PWM1_State(0);
  PWM2_State(0);
  PWM3_State(0);
}
void quansujianjin(void)
{
  PWM0_State(100);
  PWM1_State(100);
  PWM2_State(100);
  PWM3_State(100);
}

/*
从后往前
6.3 6.2 6.1 


*/

//模式1
void xunji_1(void)//全差速循迹
{

  if(( (P6IN & BIT1)) && ((P6IN & BIT2) && (P6IN & BIT3)))//全不亮
  {
    if(cycle>=2)
    {
      front();
      ting();
    }
    else
    {
      front();
      qian();
    }
    if(cn==0)
    {cycle+=1;cn=1;}
    
  }
  else if(((P6IN & BIT1)==0) && ((P6IN & BIT2)&&(P6IN & BIT3)))//左都灭 识别内圈
  {front();qian();cn=0;} 
//  else if((((P6IN & BIT1) || (P6IN & BIT2) || (P6IN & BIT3))== 0) && (P6IN & BIT3))//左6.3灭
//  {right();big_you(pwm1_H,pwm1_L);cn=0;delay_nms(200);}
//  else if((((P6IN & BIT1) || (P6IN & BIT2) || (P6IN & BIT3))== 0)  )//右6.0灭
//  {left(); big_zuo(pwm1_H,pwm1_L);cn=0;delay_nms(200);}
  else if((( (P6IN & BIT1) || (P6IN & BIT2))== 0) && (P6IN & BIT3))//左6.3灭
  {left(); zuo(pwm1_H,pwm1_L);   cn=0;delay_nms(120);}
  else if((( (P6IN & BIT2) || (P6IN & BIT3))== 0) && (P6IN & BIT1))//右6.1灭
  {right();you(pwm1_H,pwm1_L);   cn=0;delay_nms(120);}
  else if((((P6IN & BIT1)  && (P6IN & BIT3))== 0) && (P6IN & BIT2))//中6.2灭 
  {front();qian();cn=0;} 
  else {front();qian();}
}
//模式二
void xunji_2(void)
{

  if(( (P6IN & BIT1)) && ((P6IN & BIT2) && (P6IN & BIT3)))//全不亮
  {
    front();ting();
    P8OUT |=BIT1;
    delay_nms(100);
    P8OUT &=~ BIT1;
    if(cn==0)
    {cycle+=1;cn=1;}
  }
  else if(((P6IN & BIT1)==0) && ((P6IN & BIT2)&&(P6IN & BIT3)))//左都灭 识别内圈
  {front();qian();cn=0;} 
//  else if((((P6IN & BIT1) || (P6IN & BIT2) || (P6IN & BIT3))== 0) && (P6IN & BIT3))//左6.3灭
//  {right();big_you(pwm1_H,pwm1_L);cn=0;delay_nms(200);}
//  else if((((P6IN & BIT1) || (P6IN & BIT2) || (P6IN & BIT3))== 0)  )//右6.0灭
//  {left(); big_zuo(pwm1_H,pwm1_L);cn=0;delay_nms(200);}
  else if((( (P6IN & BIT1) || (P6IN & BIT2))== 0) && (P6IN & BIT3))//左6.3灭
  {left(); zuo(pwm1_H,pwm1_L);   cn=0;delay_nms(120);}
  else if((( (P6IN & BIT2) || (P6IN & BIT3))== 0) && (P6IN & BIT1))//右6.1灭
  {right();you(pwm1_H,pwm1_L);   cn=0;delay_nms(120);}
  else if((((P6IN & BIT1)  && (P6IN & BIT3))== 0) && (P6IN & BIT2))//中6.2灭 
  {front();qian();cn=0;} 
  else {front();qian();};
}
void interrupt_two_key(void)
{
  
  P1DIR &= ~BIT1;
  P2DIR &= ~BIT1;          //设置P1.1,P2.1口为输入
  
  P1REN |= BIT1;
  P2REN |= BIT1;
  
  P1OUT |= BIT1;
  P2OUT |= BIT1;          //配置P1.1,P2.1上拉电阻
  
  P1IES |= BIT1;
  P2IES |= BIT1;          //P1.1,P2.1下降沿触发中断
  
  P1IE |= BIT1;
  P2IE |= BIT1;           //P1.1,P2.1中断使能
  
  P1IFG &= ~BIT1;
  P2IFG &= ~BIT1;        //清除中断标志位
  
}
void others_init(void)
{
  // 蜂鸣器初始化
  P8DIR |= BIT1;
  P8OUT &=~ BIT1;
  //小灯初始化
  P1DIR |= BIT0;
  P1OUT &=~ BIT0;
}
void main(void)
{
  
  WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
  //initClockTo16MHz();
  OLED_Init();//oled初始化
  OLED_Clear();//oled屏幕清零
  others_init();//灯和蜂鸣器初始化
  interrupt_two_key();//两个按键初始化
  PWM0_Config();//2.0 pwm调速
  PWM1_Config();//1.3 pwm调速
  PWM2_Config();//1.4 pwm调速
  PWM3_Config();//1.5
  PWM0_State(100);
  PWM1_State(100);
  PWM2_State(100);
  PWM3_State(100);
  USART0_Config();//串口（蓝牙）初始化
  catinit();//小车初始化
  OLED_ShowString(0,4,"Mode",16);
  __bis_SR_register(GIE);
  cycle=0;
  while(1)
  {
    //这是从机的
    while(mode==1)
    {
      OLED_ShowString(0,5,"1",16);
      if(flag==1)
      {
        //开车咯
        send0_str("go");
        OLED_ShowString(2,4,"go",16);
        //先小开一会 过起点 从机不需要
//        front();
//        qian();
//        delay_nms(400);
        while(1)
        {
          xunji_1();
          if(cycle>=1)
            break;
        }
        
        //检测到停止位，蜂鸣器响
        send0_str("stop");
        P8OUT |=BIT1;
        delay_nms(100);
        P8OUT &=~ BIT1;
        OLED_ShowString(2,4,"stop",16);
        OLED_ShowNum(4,6,cycle,1,16);
        return ;
      }
      else if(flag==0)
      {
        stop();
        ting();
        //
        if(((P6IN & BIT1)) && ((P6IN & BIT2) && (P6IN & BIT3)))
        {
          P8OUT |=BIT1;
          delay_nms(100);
          P8OUT &=~ BIT1;
        }
      }
    } 
    //这是从机的
    while(mode==2)
    {
      OLED_ShowString(0,5,"2",16);
      if(flag==1)
      {
        //开车咯
        send0_str("go");
        OLED_ShowString(2,4,"go",16);
        //先小开一会 过起点 从机不需要
//        front();
//        qian();
//        delay_nms(400);
        while(1)
        {
          xunji_2();
          if(cycle>=2)
            break;
        }
        
        //检测到停止位，蜂鸣器响
        send0_str("stop");
        P8OUT |=BIT1;
        delay_nms(100);
        P8OUT &=~ BIT1;
        OLED_ShowString(2,4,"stop",16);
        OLED_ShowNum(4,6,cycle,1,16);
        return ;
      }
    }
    
  }
}



#pragma vector = PORT1_VECTOR    //S1中断服务函数
__interrupt void P1_ISR()
{
  if(P1IFG & BIT1)
  {
    flag=1;
    delay_nms(500);
  }
  P1IFG &=~ BIT1;
        
}
#pragma vector = PORT2_VECTOR    //S1中断服务函数
__interrupt void P2_ISR()
{
  if(P2IFG & BIT1)
  {
    delay_nms(50);
    if(P2IN & BIT1)
      mode++;
    delay_nms(50);
    mode%=5;
  }
  P2IFG &=~ BIT1;
}

#pragma vector=USCI_A0_VECTOR  // 进入中断向量地址
__interrupt void USCI_A0_ISR(void)
{
  static char Buff = 0;
  while (!(UCA0IFG&UCRXIFG));
  Buff = UCA0RXBUF;
  if(Buff=='s')
    flag=0;
  flag=Buff-'0';
}


