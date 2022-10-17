/*利用Timer_A比较模式下的多路定时，让LED闪烁*/
#include "msp430f5529.h"

void main(void)

{

   WDTCTL=WDTPW+WDTHOLD;

   P1DIR |=(BIT0+BIT2+BIT3+BIT4+BIT5);      //P1.1-P1.5为输出方向   　　　　　　　　　　
   P1OUT |= 0X00;    //全部拉低，初始化LED全灭

   TA0CCTL1 = CCIE;          //捕获比较器1开启CCIFG位中断　　　
   TA0CCR1=13107;             //置入要比较的数值0xff/5=13107
   TA0CCTL2 = CCIE;         //捕获比较器2开启中断　　　　　　　　　　      　　　　　　　　　　
   TA0CCR2=26214;            //13107*2=26214    　　　　　　　　　　　
   TA0CCTL3 = CCIE;          //捕获比较器3开启中断       　　　　　　　　
   TA0CCR3 = 39321;           //13107*3=39321    　　　　　　　　　　
   TA0CCTL4 = CCIE;        //捕获比较器4开启中断   　　　　　　　　　　
   TA0CCR4=52428;          //13107*4=52428      　　　　　　　　
   TA0CTL|=TASSEL_1+MC_2+TACLR+TAIE;    //选择SCLK32.768KHZ作为时钟，选用连续模式，并开启中断
   /*这样的话，5个灯闪一遍的时间为0xffff/32768=2S*/

  __enable_interrupt();
  while(1);

}

/*TIMER0_A0_VECTOR是计时器0的CCR0的中断寄存器，TIMER0_A1_VECTOR是计时器0的CCR1-CCR4、TA的寄存器*/

/*同理定时器TA1也是分为两个TIMER1_A0_VECTOR和TIMER1_A1_VECTOR*/

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TimerA(void)
{
  switch(__even_in_range(TA0IV,14))    /* 这句话的意思是：只有在TA0IV的值是在0--14内的偶数时才会执行switch函数内的语句
            其作用是提高switch语句的效率*/
  {
  case 2:P1OUT = BIT0;break;//TACCR1 CCIFG置位，表明计数值和设定的13107相等了，也就是说计了0.4S了
  case 4:P1OUT = BIT2;break;//TACCR2 CCIFG置位，表明计了0.8S了
  case 6:P1OUT = BIT3;break;//TACCR3 CCIFG置位，表明计了1.2S了
  case 8:P1OUT = BIT4;break;//TACCR4 CCIFG置位，表明计了1.6S了
  case 14:P1OUT = BIT5;break; //TAIFG置位，表明计了2S了
  }
}





