/*在比较和增减模式下产生PWM波（矩形波） */

/*提一个PWM波的用处：驱动直流电机。我们知道对于直流电机，驱动它的电流的频率并不影响转速 ，只有占空比会影响转速*/

/*开发板上P2.0是有外接排针的，所以用这一端口输出PWM*/

/*看CPU引脚发现，P2.0为TA1.1，也就是定时器A1的1号捕获比较器输出口*/

#include <msp430.h>

void main(void)

{

   WDTCTL=WDTPW+WDTHOLD;
   P2SEL|=BIT0;   //声明有特殊功能，不做普通I/O使用
   P2DIR |= BIT0;  // 输出
   P2DS |= BIT0;//全力驱动，否则可能无法驱动电机
   P2OUT &= ~BIT0;

   /*把SMCL配置为XT2 4MHZ*/
   P5SEL = BIT2 + BIT3; //声明特殊功能，将用作外部时钟晶振XT2输入
   UCSCTL6&=~XT2OFF; //开启XT2
   while(SFRIFG1 & OFIFG)

   {

      UCSCTL7 &=~(XT2OFFG+DCOFFG+XT1LFOFFG);//清除3种时钟错误标志
      SFRIFG1 &=~(OFIFG);
   }//直到XT2从起振到振荡正常，没有错误发生
   UCSCTL4|=SELS_5;//把SMCLK的时钟源选为XT2 4MHZ
   TA1CCTL0=CCIE;//定时器A1的捕获比较器0开启CCIFG位中断
   TA1CCR0=200;//置入计数终值，则PWM频率为10KHZ
   TA1CCTL1=CCIE;//捕获比较器1开启中断
   TA1CCR1=50;//占空比为75%
   TA1CTL|=TACLR;//将计时器A1清零
   TA1CTL|=TASSEL_2+MC_3;//定时器选择SMCLK作为时钟源，且为增减模式
   TA1CCTL1=OUTMOD_4;//定时器A1中的捕获比较器1，输出模式为4翻转
   while(1);

}