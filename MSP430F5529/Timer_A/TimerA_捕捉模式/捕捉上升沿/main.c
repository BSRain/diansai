#include "msp430f5529.h"

void main(void)
{
   WDTCTL = WDTPW + WDTHOLD;//关闭看门狗定时器

   P1DIR |= BIT0;
   P2DIR &= ~BIT5;         //P2.5方向为输入
   P2SEL |= BIT5;          //PxSEL 特殊功能寄存器 将P2.5设为定时器捕获引脚
   TA2CTL = TASSEL_2 + MC_2 + TACLR;  //选择时钟SMCLK(1.05MHZ) +  连续计数模式 + 计时器计数器清零
   TA2CCTL2 = CM_1 + SCS + CAP + CCIE; //上升沿捕捉 +  时钟同步 + 捕获模式 + 捕获比较器1开启中断
   _EINT();                   //开启总中断
   while(1);
}
#pragma vector = TIMER2_A1_VECTOR
__interrupt void TIMER2_A1_ISR(void)
{
  P1OUT ^= BIT0;
  TA2CCTL2 &= ~CCIFG;  //中断标志位需要软件手动清零
}