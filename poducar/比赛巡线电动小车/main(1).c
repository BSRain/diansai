/*------------------
//项目：坡道行驶电动小车，寻迹小车
//作者：化作尘
//时间：2020年10月13日17点51分
//版本：v1.0
//邮箱：2809786963@qq.com
//csdn: 化作尘
//bilibili: 化作尘my
//哔哩哔哩项目展示视频：
	https://www.bilibili.com/video/BV1oz4y1d7mH
--------------------*/
#include "msp430.h"
#include "car.c"
#include "oled.h"
#include "oled.c"
#include "oledfont.h"
#include "gpio.h"
double X_1,X_2,X_3,X_4,V_1,V_2,V_3,V_4;//mm  ms
double x1,x2,x3,x4,s1,s2,s3,s4;
char flag=0;
uchar data=0;
int n=0;
int va=15,vr=0;
int time=10;
int go = 0;
#include "PID.h"
void PIDOUT_qian();
void PIDOUT_hou();
void PIDOUT_zuo();
void PIDOUT_you();
void display();
void UART_Init();
void Send_Byte(unsigned char);
void Print_Str(unsigned char*);
void Set_temp(float ,float ,float, float);
void PIDOUT_left();
void PIDOUT_right();

void interrupt(void)
{
 /*外部中断*/
  P1IE |= BIT0 + BIT1 + BIT2 +BIT3;  //打开P1_1中断
  P1IES |= BIT0 + BIT1 + BIT2 +BIT3;  //下降沿触发
  P1IFG &=~ BIT0 + BIT1 + BIT2 +BIT3; //清楚标志位
  
  
  P2IE |= 0xff;  //打开P2中断
  //P2DIR = 0x00;
  P2IES |= 0xff;   //下降沿触发
  P2IFG &=~ 0xff;  //清楚标志位
}

/*晶振初始化*/
void Sysclk_init()
{
  WDTCTL = WDT_ADLY_250;
  IE1 |= WDTIE;
  unsigned char i;
  BCSCTL1&=~XT2OFF;                 //打开XT2振荡器
  BCSCTL2|=SELM1+SELS;              //MCLK为8MHZ，SMCLK为8MHZ
  do{
    IFG1&=~OFIFG;                   //清楚振荡器错误标志
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
  IFG1&=~OFIFG; 
  
  _EINT();
}


/*定时器初始化*/
void Tim_init()
{
  TACTL   |= TASSEL_2+MC_2+TAIE+TACLR+ ID_3;//SMCLK,连续计数，中断允许，计数器清零
  TACCTL1 |= CAP+CM_1+CCIS_0+SCS+CCIE;//捕获模式，上升沿捕获，CCI1A输入，同步捕获，中断允许
  TACCTL2 |= CAP+CM_1+CCIS_0+SCS+CCIE;//捕获模式，上升沿捕获，CCI1A输入，同步捕获，中断允许 

  TBCTL |= TBCCTL1_+ID_3+MC_1;     //4069HZ计数
  TBCCTL1 |= OUTMOD_7;
  TBCCTL2 |= OUTMOD_7;
  TBCCTL3 |= OUTMOD_7;
  TBCCTL4 |= OUTMOD_7;
  TBCCTL5 |= CAP+CM_1+CCIS_0+SCS+CCIE;//捕获模式，上升沿捕获，CCI1B输入，同步捕获，中断允许 
  TBCCTL6 |= CAP+CM_1+CCIS_0+SCS+CCIE;//捕获模式，上升沿捕获，CCI1B输入，同步捕获，中断允许 
  
  //P5DIR &= ~BIT0;
  
  TBCCR0 = 1000;       //PWM周期
  TBCCR1 = 0;          
  TBCCR2 = 0;
  TBCCR3 = 0;
  TBCCR4 = 0;
  P4DIR |= BIT1+BIT2+BIT3+BIT4;       //PWMIO口初始化
  P4SEL |= BIT1+BIT2+BIT3+BIT4;
  P1DIR &=~ BIT2+BIT3;               //初始化捕获IO口
  P1SEL |= BIT2+BIT3; 
  P4DIR &=~ BIT5+BIT6;
  P4SEL |= BIT5+BIT6;
}

void main(void)
{
  // Stop watchdog timer to prevent time out reset
  Sysclk_init();
  Tim_init();
  
  P3DIR |=0X1f;//P2.0 P2.1 P2.2 P2.3 P2.4  xxxx 1111
  P3OUT=0xff;

  interrupt();//外部中断 初始化
  OLED_Init();//初始化OLED  
  
  IO_init();//电机初始化
  P3DIR |= BIT5;
  P3OUT &= ~BIT5;
  
  OLED_Clear();//清屏函数 
  //OLED_ShowCHinese(0,0,0);//显示汉字 x y 第几个汉字
  OLED_ShowString(4,4,"speed:      ");//速度
  OLED_ShowString(4,6,"dista:      ");//
      
  //等待go按键按下
  while(!go)
  {
    display();
  }va = 150/time; 
  
  while(1)
  {   
    display(); //显示
    PID_calc();//PID计算 
    
    if((vr == 0) || (vr == 50/time) || ((vr == -50/time) || (vr == 150/time)))
    {
      PIDOUT_qian();
      Set_temp(va-vr,va-vr,va+vr,va+vr);
    }
    else if(vr == -150/time)
    {
      PIDOUT_zuo();
      Set_temp(va-vr,va-vr,3,6);
    }
    if(go == 0)
    {
      P3OUT |= BIT5;delayms(600);//蜂鸣器
      P3OUT &=~ BIT5;delayms(600);
      P3OUT |= BIT5;delayms(600);
      P3OUT &=~ BIT5;delayms(600);
      P3OUT |= BIT5;delayms(600);
      P3OUT &=~ BIT5;delayms(600);
      while(1);//停止
    }
  }
}
#pragma vector=PORT1_VECTOR   //中断向量1
__interrupt void Port1()
{
  if(P1IFG & BIT0) //按键1
  {
    delayms(10);
    if(time<20)time++;
    va = 150/time;
  }
  if(P1IFG & BIT1)//按键2
  {
    go = 1;
  }
  P1IFG = 0; //清楚标志位
}
#pragma vector=PORT2_VECTOR   //中断向量2
__interrupt void Port2()
{
  if(P2IFG&BIT0 || P2IFG&BIT4)
  {
    vr = 150/time;
  }
  if(P2IFG&BIT1 || P2IFG&BIT5)
  {
    vr = 50/time;
  }
  if(P2IFG&BIT2 || P2IFG&BIT6)
  {
    vr = -50/time;
  }
  if(P2IFG&BIT3 || P2IFG&BIT7)
  {
    vr = -150/time;
  }
  
   if((P2IN&BIT1==BIT1) && (P2IN&BIT2==BIT2))
   {
      go = 0;
      Set_temp(0,0,0,0);
      stop();
   }
//   if((P2IN&BIT0==BIT0) && (P2IN&BIT1==BIT1))
//   {
//      go = 0;
//      Set_temp(0,0,0,0);
//   }
  
  P2IFG = 0; //清楚标志位7
}
#pragma vector=TIMERA1_VECTOR  //Timer_A捕获中断向量
__interrupt void Timer_A(void)
{
  switch(TAIV)   
  { 
    case 2 : X_1+=0.4619989; break; 
    case 4 : X_2+=0.4619989; break;     
  }      
}

#pragma vector=TIMERB1_VECTOR  //Timer_B捕获中断向量
__interrupt void Timer_B(void)
{
  switch(TBIV)   
  {
   case 10 : X_3+=0.4619989; break;
   case 12 : X_4+=0.4619989; break;
  }
}
#pragma vector=WDT_VECTOR  //看门狗定时器中断
__interrupt void WDT_1(void)
{
  P6OUT ^= BIT7;
  x1=s1;
  s1=X_1;
  V_1=(X_1-x1)/2.50;//单位cm/s
  x2=s2;
  s2=X_2;
  V_2=(X_2-x2)/2.50;
  x3=s3;
  s3=X_3;
  V_3=(X_3-x3)/2.50;
  x4=s4;
  s4=X_4;
  V_4=(X_4-x4)/2.50;
}

void PIDOUT_qian()
{
    zhen_1((int)PID_OUT);//输出速度  
    zhen_2((int)PID_OUT1);//输出速度   
    zhen_3((int)PID_OUT2);//输出速度  
    zhen_4((int)PID_OUT3);//输出速度  
}

void PIDOUT_zuo()
{
    zhen_1((int)PID_OUT);//输出速度  
    zhen_2((int)PID_OUT1);//输出速度   
    fan_3((int)PID_OUT2);//输出速度  
    fan_4((int)PID_OUT3);//输出速度  
}

void Set_temp(float a,float b,float c, float d)
  {
    Set_Temp= a;//预置速度30cm/s  V_4
    Set_Temp1= b;//预置速度30cm/s  V_3
    Set_Temp2= c;//预置速度30cm/s  V_2
    Set_Temp3= d;//预置速度30cm/s  V_1
  }

void display()
{
    OLED_ShowChar(0,0,(int) (P*1000)/1000+0x30);//1234/1000=1
    OLED_ShowChar(8,0,(int)(P*1000)%1000/100+0x30);//1234%1000/100=2
    OLED_ShowChar(16,0,(int)(P*1000)%1000%100/10+0x30);//1234%1000%100/10=3
    OLED_ShowChar(24,0,(int)(P*1000)%1000%100%10+0x30);//1234%1000%100/10=4
    
    OLED_ShowChar(34,0,(int)(I*1000)/1000+0x30);//1234/1000=1
    OLED_ShowChar(42,0,(int)(I*1000)%1000/100+0x30);//1234%1000/100=2
    OLED_ShowChar(50,0,(int)(I*1000)%1000%100/10+0x30);//1234%1000%100/10=3
    OLED_ShowChar(58,0,(int)(I*1000)%1000%100%10+0x30);//1234%1000%100/10=4
    
    OLED_ShowChar(68,0,(int)D/1000+0x30);//1234/1000=1
    OLED_ShowChar(76,0,(int)D%1000/100+0x30);//1234%1000/100=2
    OLED_ShowChar(84,0,(int)D%1000%100/10+0x30);//1234%1000%100/10=3
    OLED_ShowChar(92,0,(int)D%1000%100%10+0x30);//1234%1000%100/10=4
    
    OLED_ShowChar(106,0,(int)time/10+0x30);//1234%1000%100/10=3
    OLED_ShowChar(114,0,(int)time%10+0x30);//1234%1000%100/10=4
    
    OLED_ShowChar(95,6,(int) X_2/1000+0x30);//1234/1000=1
    OLED_ShowChar(103,6,(int)X_2%1000/100+0x30);//1234%1000/100=2
    OLED_ShowChar(111,6,(int)X_2%1000%100/10+0x30);//1234%1000%100/10=3
    OLED_ShowChar(119,6,(int)X_2%1000%100%10+0x30);//1234%1000%100/10=4
    OLED_ShowChar(61,6,(int) X_1/1000+0x30);//1234/1000=1
    OLED_ShowChar(69,6,(int)X_1%1000/100+0x30);//1234%1000/100=2
    OLED_ShowChar(77,6,(int)X_1%1000%100/10+0x30);//1234%1000%100/10=3
    OLED_ShowChar(85,6,(int)X_1%1000%100%10+0x30);//1234%1000%100/10=4
   /* OLED_ShowChar(43,2,(int)X_3/1000+0x30);//1234/1000=1
    OLED_ShowChar(51,2,(int)X_3%1000/100+0x30);//1234%1000/100=2
    OLED_ShowChar(59,2,(int)X_3%1000%100/10+0x30);//1234%1000%100/10=3
    OLED_ShowChar(67,2,(int)X_3%1000%100%10+0x30);//1234%1000%100/10=4
    OLED_ShowChar(95,2,(int)X_4/1000+0x30);//1234/1000=1
    OLED_ShowChar(103,2,(int)X_4%1000/100+0x30);//1234%1000/100=2
    OLED_ShowChar(111,2,(int)X_4%1000%100/10+0x30);//1234%1000%100/10=3
    OLED_ShowChar(119,2,(int)X_4%1000%100%10+0x30);//1234%1000%100/10=4*/
    OLED_ShowChar(57,4, (int)V_1/10+0x30);//1234/1000=1
    OLED_ShowChar(65,4,(int)V_1%10+0x30);//1234%1000/100=2
    OLED_ShowChar(75,4,(int)V_2/10+0x30);//1234%1000%100/10=3
    OLED_ShowChar(83,4,(int)V_2%10+0x30);//1234%1000%100/10=4
    OLED_ShowChar(93,4, (int)V_3/10+0x30);//1234/1000=1
    OLED_ShowChar(101,4,(int)V_3%10+0x30);//1234%1000/100=2
    OLED_ShowChar(111,4,(int)V_4/10+0x30);//1234%1000%100/10=3
    OLED_ShowChar(119,4,(int)V_4%10+0x30);//1234%1000%100/10=4
    OLED_ShowChar(103,2,(int)PID_OUT/100+0x30);//1234%1000/100=2
    OLED_ShowChar(111,2,(int)PID_OUT%100/10+0x30);//1234%1000%100/10=3
    OLED_ShowChar(119,2,(int)PID_OUT1%10+0x30);//1234%1000%100/10=4
    OLED_ShowChar(77,2,(int)PID_OUT1/100+0x30);//1234%1000/100=2
    OLED_ShowChar(85,2,(int)PID_OUT1%100/10+0x30);//1234%1000%100/10=3
    OLED_ShowChar(93,2,(int)PID_OUT1%10+0x30);//1234%1000%100/10=4
    
}