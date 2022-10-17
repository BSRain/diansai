/* Timer_A0 CCR0 中断*/
#include <msp430.h>
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR |= 0x01;                            // P1.0 output
  TA0CCTL0 = CCIE;                          // CCR0 interrupt enabled  CCR0中断使能
  TA0CCR0 = 50000;
  TA0CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, upmode, clear TAR 子系统时钟 + 计数至CCR0 + 计时器计数器清零

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, enable interrupts
}

#pragma vector=TIMER0_A0_VECTOR    //   CCR0中断向量地址为TIMER0_A0_VECTOR CCR0-CCR4中断向量地址为 TIMER0_A1_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
  P1OUT ^= 0x01;                            // Toggle P1.0
}
