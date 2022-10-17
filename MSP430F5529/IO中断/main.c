/*按键中断控制LE和按键消抖*/
#include <msp430.h>
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR |= BIT0;                            // Set P1.0 to output direction
  P2REN |= BIT1;                            // Enable P2.1 internal resistance
  P2OUT |= BIT1;                            // Set P2.1 as pull-Up resistance
  P2IES |= BIT1;                             // 高电平表示下降沿触发；低电平表示上升沿触发；
  P2IFG &= ~BIT1;                           // P2.1IFG cleared //清除中断标志
  P2IE |= BIT1;                             // P2.1interrupt enabled

  __bis_SR_register(LPM4_bits + GIE);       // Enter LPM4 w/interrupt//进入低功耗模式4 和 开启全局中断
}
#pragma vector=PORT2_VECTOR                 //进入中断向量地址
__interrupt void Port_1(void)                       //中断服务函数

{
   __delay_cycles(10000);                   //延时消抖
  if((P2IN & BIT1)==0)
  P1OUT ^= BIT0;                            // P1.0 = toggle
  P2IFG &= ~BIT1;                          // P2.1 IFG cleared
}
