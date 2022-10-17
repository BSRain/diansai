/* Timer_A0 CCR0 �ж�*/
#include <msp430.h>
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR |= 0x01;                            // P1.0 output
  TA0CCTL0 = CCIE;                          // CCR0 interrupt enabled  CCR0�ж�ʹ��
  TA0CCR0 = 50000;
  TA0CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, upmode, clear TAR ��ϵͳʱ�� + ������CCR0 + ��ʱ������������

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, enable interrupts
}

#pragma vector=TIMER0_A0_VECTOR    //   CCR0�ж�������ַΪTIMER0_A0_VECTOR CCR0-CCR4�ж�������ַΪ TIMER0_A1_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
  P1OUT ^= 0x01;                            // Toggle P1.0
}
