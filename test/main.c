
#include "msp430.h"

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  P1DIR |=BIT3;
  P1OUT |=BIT3;
  
  P1DIR |= BIT0;
  P1OUT &=~ BIT0;
  TA0CCTL0 = CCIE;
  P1DIR |= BIT0;
  TA0CCR0 = 32768;//—” ±1s  
  TA0CTL = TASSEL_1 + MC_1 + TACLR;
_EINT();
  while(1);
  return 0;
}
//#pragma vector=TIMER0_A0_VECTOR
#pragma vector = TIMER0_A0_VECTOR
__interrupt void func(){
P1OUT ^= BIT0;
}
