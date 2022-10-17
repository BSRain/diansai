#include <stdio.h>
#include <msp430.h>
#include "keys.h"
//#include "main.h"
//引脚 f5529
/*
  列p1.4~1.5andP2.4~p2.5 
  行p6.0~p6.3 
*/

//int result=0;
//int row=0;
//int flag,flag1,flag2,flag3;
//unsigned int Push_Key=0;

#define List0   0x80
#define List1   0x90
#define List2   0x88
#define List3   0x98
#define CPU_F ((double)1035000)
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))


/*
  行列扫描法
  初始化
*/
void keyboard_init()
{
  //设置行管脚为输入
  P3DIR &= ~BIT5; //P3.5端口设为输入
  P3REN |= BIT5;  //使3.5能上拉/下拉电阻
  P3OUT &=~ BIT5;  //置P3.5为下拉电阻模式
  P3SEL &= ~BIT5; //P3.5为I/O接口
  //    P3IE  |= BIT5;
  //    P3IES |= BIT5;
  //    P3IFG &= ~BIT5;
  
  
  P3DIR &= ~BIT6; //P3.6端口设为输入
  P3REN |= BIT6;  //使P3.6能上拉/下拉电阻
  P3OUT &=~ BIT6;  //置P3.6为下拉电阻模式
  P3SEL &= ~BIT6; //P3.6为I/O接口
  //    P3IE |= BIT6;
  //    P3IES |= BIT6;
  //    P3IFG &= ~BIT6;
  
  P7DIR &= ~BIT0; //P7.0端口设为输入
  P7REN |= BIT0;  //使P7.0能上拉/下拉电阻
  P7OUT &=~ BIT0;  //置P7.0为下拉电阻模式
  P7SEL &= ~BIT0; //P7.0为I/O接口
  //    P7IE  |= BIT0;
  //    P7IES |= BIT0;
  //    P7IFG &= ~BIT0;
  
  P6DIR &= ~BIT4; //P6.4端口设为输入
  P6REN |= BIT4;  //使P6.4能上拉/下拉电阻
  P6OUT &=~ BIT4;  //置P6.4为下拉电阻模式
  P6SEL &=~ BIT4; //P6.4为I/O接口
  //    P6IE  |= BIT4;
  //    P6IES |= BIT4;
  //    P6IFG &= ~BIT4;
  
  
  //设置列管脚为输出
  P6DIR |= BIT3+BIT2+BIT1+BIT0; //P6.3,2,1,0端口设为输出
  P6OUT &=~(BIT3+BIT2+BIT1+BIT0);
}

char scanKey()
{
  //扫描列
  char key_value=0;
  unsigned char state=0;
  do{
    key_value=0;
    if(state==0)
    {
      P6OUT |= BIT0;
      P6OUT &=~ (BIT1+BIT2+BIT3);
      
    }else if(state==1)
    {
      P6OUT |= BIT1;
      P6OUT &=~ (BIT0+BIT2+BIT3);
      
    }else if(state==2)
    {
      P6OUT |= BIT2;
      P6OUT &=~ (BIT0+BIT1+BIT3);
      
    }else if(state==3)
    {
      P6OUT |= BIT3;
      P6OUT &=~ (BIT0+BIT1+BIT2);
      
    }
    
    
    
    if(++state==4) state=0;
    
    
    if(P6OUT & BIT0)
      key_value = BIT4;
    
    if(P6OUT & BIT1)
      key_value = BIT5;
    
    if(P6OUT & BIT2)
      key_value = BIT6;
    
    if(P6OUT & BIT3)
      key_value = BIT7;
    
    
    if(P3IN & BIT5) key_value |= BIT0;
    else key_value &=~ BIT0;
    
    if(P3IN & BIT6) key_value |= BIT1;
    else key_value &=~ BIT1;
    
    if(P7IN & BIT0) key_value |= BIT2;
    else key_value &=~ BIT2;
    
    if(P6IN & BIT4) key_value |= BIT3;
    else key_value &=~ BIT3;
    
    if((key_value&0x0f)==0)
      delay_ms(35);
  }while((key_value&0x0f)==0);//直到有行被按下
  switch(key_value)
  {//返回按下的字符
  case 129:return '1';    //s13   //按键
  case 65: return '4';    //s14
  case 33: return '7';    //s15
  case 17: return '0';    //s16
  case 130: return '2';   //s9
  case 66: return '5';    //s10
  case 34: return '8';    //s11
  case 18: return 'S';    //s12   //设置 //set
  case 132: return '3';   //s5
  case 68: return '6';    //s6
  case 36: return '9';    //s7
  case 20: return 'B';    //s8    //启动  //boot
  case 136: {return 'M';}   //s1    //模式  //mode
  case 72: return 'X';    //s2    //x     
  case 40: return 'Y';    //s3    //y
  case 24: return 'A';    //s4    //确认  //acept
  default: return 0;
  }
}


