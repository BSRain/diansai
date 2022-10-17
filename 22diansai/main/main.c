#include "stdio.h"
#include <msp430f5529.h>
#include "Black_All.h"
#include "oled.h"
int pwm1_L=40;
int pwm1_H=40;
int pwm1_go=26;

int pwm2_L=50;
int pwm2_H=50;
int pwm2_go=36;

int pwm3_L=40;
int pwm3_H=40;
int pwm3_go=26;
int pwm3_go2=36;

int pwm4_L=40;
int pwm4_H=70;
int pwm4_go=95;
int mode=0;//模式
int flag=0;//发车
int cycle=0;//旋转圈数
int cn=0;//cn防止检测到起点误加
int time=0;//要求4时间

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
  INPUT_GPIO_Config(6,4);
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
void clock_init(void)//定时器初始化
{
  TB0CCTL0 = CCIE;
  TB0CCR0 = 32768;//延时1s  
  TB0CTL = TASSEL_1 + MC_1 + TACLR;
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
void you(int pwm1,int pwm0)
{ 
  PWM0_State(pwm1);//2.0上左
  PWM1_State(pwm0);//1.3上右
  PWM2_State(pwm1);//1.4下左
  PWM3_State(pwm0);//1.5下右
}

void zuo(int pwm1,int pwm0)
{
  
  PWM0_State(pwm0);
  PWM1_State(pwm1);
  PWM2_State(pwm0);
  PWM3_State(pwm1);
}

void qian(int pwmgo )
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
void turn_left()
{
  while((P6IN & BIT2)==0)//当中灭的时候跳出
  {
    left(); 
    zuo(pwm1_H,pwm1_L);
     cn=0;
  }
}
//模式1
void xunji_1(void)//全差速循迹
{

  if(( (P6IN & BIT1)) && ((P6IN & BIT2) && (P6IN & BIT3)))//全不亮
  {
    if(cn==0)
    {cycle+=1;cn=1;}
    if(cycle>=1)
    {
      back();
      qian(pwm1_go);
      delay_nms(150);
      ting();
    }
    else
    {
      front();
      qian(pwm1_go);
    }
    
    
  }
  else if(((P6IN & BIT1)==0) && ((P6IN & BIT2)&&(P6IN & BIT3)))//左都灭 识别内圈
  {front();qian(pwm1_go);cn=0;} 
//  else if((((P6IN & BIT1) || (P6IN & BIT2) || (P6IN & BIT3))== 0) && (P6IN & BIT3))//左6.3灭
//  {right();big_you(pwm1_H,pwm1_L);cn=0;delay_nms(200);}
//  else if((((P6IN & BIT1) || (P6IN & BIT2) || (P6IN & BIT3))== 0)  )//右6.0灭
//  {left(); big_zuo(pwm1_H,pwm1_L);cn=0;delay_nms(200);}
  else if((( (P6IN & BIT1) || (P6IN & BIT2))== 0) && (P6IN & BIT3))//左6.3灭
  {
    while((P6IN & BIT2)==0)//当中灭的时候跳出
  {
    left(); 
    zuo(pwm1_H,pwm1_L);
     cn=0;
  }
    //left(); zuo(pwm1_H,pwm1_L);   cn=0;delay_nms(150);
  }
  else if((( (P6IN & BIT2) || (P6IN & BIT3))== 0) && (P6IN & BIT1))//右6.1灭
  {right();you(pwm1_H,pwm1_L);   cn=0;delay_nms(80);}
  else if((((P6IN & BIT1)  && (P6IN & BIT3))== 0) && (P6IN & BIT2))//中6.2灭 
  {front();qian(pwm1_go);cn=0;} 
  else {front();qian(pwm1_go);}
}
//模式二
void xunji_2(void)
{

  if(( (P6IN & BIT1)) && ((P6IN & BIT2) && (P6IN & BIT3)))//全不亮
  {
    if(cn==0)
    {cycle+=1;cn=1;}
    if(cycle>=2)
    {
      back();
      qian(pwm2_go);
      delay_nms(150);
      ting();
    }
    else
    {
      front();
      P8OUT |=BIT1;
      delay_nms(50);
      P8OUT &=~ BIT1;
      qian(pwm2_go);
    }

  }
  else if(((P6IN & BIT1)==0) && ((P6IN & BIT2)&&(P6IN & BIT3)))//左都灭 识别内圈
  {front();qian(pwm2_go);cn=0;} 
  else if((( (P6IN & BIT1) || (P6IN & BIT2))== 0) && (P6IN & BIT3))//左6.3灭
  {
    while((P6IN & BIT2)==0)//当中灭的时候跳出
    {
      left(); 
      zuo(pwm2_H,pwm2_L);
      cn=0;
    }
    //left(); zuo(pwm1_H,pwm1_L);   cn=0;delay_nms(150);
  }
  else if((( (P6IN & BIT2) || (P6IN & BIT3))== 0) && (P6IN & BIT1))//右6.1灭
  {
    right();you(pwm2_H,pwm2_L);   cn=0;delay_nms(70);
  }
  else if((((P6IN & BIT1)  && (P6IN & BIT3))== 0) && (P6IN & BIT2))//中6.2灭 
  {front();qian(pwm2_go);cn=0;} 
  else {front();qian(pwm2_go);}
}
//模式3
void xunji_3(void)//全差速循迹
{

  if(( (P6IN & BIT1)) && ((P6IN & BIT2) && (P6IN & BIT3)))//全不亮
  {
    if(cn==0)
    {cycle+=1;cn=1;}
    if(cycle==3)
    {
      back();
      qian(pwm1_go);
      delay_nms(150);
      ting();
    }
    else if(cycle==1)
    {
      
    }
    else if(cycle==1)
    {
      front();
      qian(pwm1_go);
    }
    
    
  }
  else if(((P6IN & BIT1)==0) && ((P6IN & BIT2)&&(P6IN & BIT3)))//左都灭 识别内圈
  {
    if(cycle==2)
    {
      while((P6IN & BIT2)==0)//当中灭的时候跳出
      {
        left(); 
        zuo(pwm1_H,pwm1_L);
        cn=0;
      }
    }
    else
    front();qian(pwm1_go);cn=0;
  } 
  else if((( (P6IN & BIT1) || (P6IN & BIT2))== 0) && (P6IN & BIT3))//左6.3灭
  {
    while((P6IN & BIT2)==0)//当中灭的时候跳出
    {
      left(); 
      zuo(pwm1_H,pwm1_L);
      cn=0;
    }
    //left(); zuo(pwm1_H,pwm1_L);   cn=0;delay_nms(150);
  }
  else if((( (P6IN & BIT2) || (P6IN & BIT3))== 0) && (P6IN & BIT1))//右6.1灭
  {right();you(pwm1_H,pwm1_L);   cn=0;delay_nms(80);}
  else if((((P6IN & BIT1)  && (P6IN & BIT3))== 0) && (P6IN & BIT2))//中6.2灭 
  {front();qian(pwm1_go);cn=0;} 
  else {front();qian(pwm1_go);}
}
//模式4
void xunji_4(void)//全差速循迹
{

  if(( (P6IN & BIT1)) && ((P6IN & BIT2) && (P6IN & BIT3)&& ((P6IN & BIT4)&&(P6IN & BIT0))))//全不亮
  {
    if(cn==0)
    {cycle+=1;cn=1;}
    if(cycle>=3)
    {
      back();
      qian(pwm4_go);
      delay_nms(50);
      ting();
    }
    else if(cycle==1)
    {
      back();
      qian(pwm4_go);
      delay_nms(80);
      stop();
      ting();
      while(time!=5)
      {
        stop();
        ting();
        send0_char('s');
        OLED_ShowNum(4,0,time,3,16);
        delay_nms(1000);
        time+=1;
        send0_char('s');
      }
      send0_char('d');
      front();
      qian(pwm4_go);
    }
    else
    {
      front();
      qian(pwm4_go);
    }
    
    
  }
  else if(((P6IN & BIT1)==0) && ((P6IN & BIT2)&&(P6IN & BIT3)))//左都灭 识别内圈
  {front();qian(pwm4_go);cn=0;} 
  else if((( (P6IN & BIT1) || (P6IN & BIT2))== 0) && (P6IN & BIT3))//左6.3灭
  {
    while((P6IN & BIT2)==0)//当中灭的时候跳出
  {
    left(); 
    zuo(pwm4_H,pwm4_L);
     cn=0;
  }
    //left(); zuo(pwm1_H,pwm1_L);   cn=0;delay_nms(150);
  }
  else if((( (P6IN & BIT2) || (P6IN & BIT3))== 0) && (P6IN & BIT1))//右6.1灭
  {
    
     while((P6IN & BIT2)==0)//当中灭的时候跳出
  {
    right(); 
    you(pwm4_H,pwm4_L);
     cn=0;
  }
    //right();you(pwm4_H,pwm4_L);   cn=0;delay_nms(8);
  }
  else if((((P6IN & BIT1)  && (P6IN & BIT3))== 0) && (P6IN & BIT2))//中6.2灭 
  {front();qian(pwm4_go);cn=0;} 
  else {front();qian(pwm4_go);}
}
void main(void)
{
  
   WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
  //initClockTo16MHz();
  OLED_Init();//oled初始化
  OLED_Clear();//oled屏幕清零
  others_init();//灯和蜂鸣器初始化
  interrupt_two_key();//两个按键初始化
  catinit();//小车初始化
  PWM0_Config();//2.0 pwm调速
  PWM1_Config();//1.3 pwm调速
  PWM2_Config();//1.4 pwm调速
  PWM3_Config();//1.5
  PWM0_State(100);
  PWM1_State(100);
  PWM2_State(100);
  PWM3_State(100);
  USART0_Config();//串口（蓝牙）初始化
  //clock_init();//定时器初始化
  //OLED_ShowString(0,4,"Mode",16);
  __bis_SR_register(GIE);
  cycle=0;
  while(1)
  {
    //这是主机的
    while(mode==1)
    {
      //OLED_ShowString(0,5,"1",16);
      if(flag==1)
      {
        //开车咯
        send0_char('a');//主机发送信息
        //OLED_ShowString(2,4,"go",16);
        //先小开一会 过起点 
        front();
        qian(pwm1_go);
        delay_nms(300);
        while(1)
        {
          xunji_1();
          if(cycle>=1)
            break;
        }
        
        //检测到停止位，蜂鸣器响
        send0_char('s');
        stop();
        
        P8OUT |=BIT1;
        delay_nms(100);
        P8OUT &=~ BIT1;
        flag=0;
        while(flag!=1);
        //OLED_ShowString(2,4,"stop",16);
        //OLED_ShowNum(4,6,cycle,1,16);
      }
    } 
    //这是主机的
    while(mode==2)
    {
      //OLED_ShowString(0,5,"2",16);
      if(flag==1)
      {
        //开车咯
        send0_char('b');
        //OLED_ShowString(2,4,"go",16);
        //先小开一会 过起点 
        front();
        qian(pwm2_go);
        delay_nms(200);
        while(1)
        {
          xunji_2();
          if(cycle>=2)
            break;
        }
        
        //检测到停止位，蜂鸣器响
        send0_char('s');
        P8OUT |=BIT1;
        delay_nms(100);
        P8OUT &=~ BIT1;
        flag=0;
        //OLED_ShowString(2,4,"stop",16);
        //OLED_ShowNum(4,6,cycle,1,16);
        while(flag!=1);
      }
    }
    while(mode==3)
    {
      
    }
    //还是主机的
    while(mode==4)
    {
      if(flag==1)
      {
        //开车咯
        send0_char('d');//主机发送信息
        //OLED_ShowString(2,4,"go",16);
        //先小开一会 过起点 
        front();
        qian(pwm4_go);
        delay_nms(100);
        while(1)
        {
          xunji_4();
          if(cycle>=3)
            break;
        }
        
        //检测到停止位，蜂鸣器响
        send0_char('s');
        stop();
        
        P8OUT |=BIT1;
        delay_nms(100);
        P8OUT &=~ BIT1;
        flag=0;
        while(flag!=1);
      }
    }
    
  }
}



#pragma vector = PORT1_VECTOR    //S1中断服务函数
__interrupt void P1_ISR()
{

    flag=1;
    delay_nms(1000);
  
   _BIC_SR_IRQ(GIE);
   P1OUT&=~BIT0;
  P1IFG &=~ BIT1;
        
}
#pragma vector = PORT2_VECTOR    //S1中断服务函数
__interrupt void P2_ISR()
{

  delay_nms(50);
  if((P2IN & BIT1)==0) 
      mode+=1;

    mode%=5;
  
  P2IFG &=~ BIT1;
}




// Echo back RXed character, confirm TX buffer is ready first
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA0IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
    UCA1TXBUF = UCA1RXBUF; //任意函数        // TX -> RXed character
    break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}