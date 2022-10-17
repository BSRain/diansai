/*UART�ж��շ�*/
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  P3SEL |= BIT3+BIT4;                       // P3.3,4 = USCI_A0 TXD/RXD
  UCA0CTL1 |= UCSWRST;                      // **Put state machine in resetʹ״̬����λ**
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK ��ϵͳʱ��MCLK��ϵͳʱ��ACLK����ϵͳʱ��
  UCA0BR0 = 9;                              // 1MHz 115200 (see User's Guide)
  UCA0BR1 = 0;                              // 1MHz 115200
  UCA0MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
  UCA0CTL1 &= ~UCSWRST;                     // **��ʼ�� usci ״̬�� Initialize USCI state machine**
  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
  P1DIR |= BIT0;
  P1OUT &=~BIT0;
  __bis_SR_register(LPM0_bits + GIE);       //����͹���ģʽ0 �� ��ȫ���ж� Enter LPM0, interrupts enabled
}

#pragma vector=USCI_A0_VECTOR  // �����ж�������ַ
__interrupt void USCI_A0_ISR(void)
{
  //static char Buff = 0;
  while (!(UCA0IFG&UCRXIFG));
  //Buff = UCA0RXBUF;
  switch(__even_in_range(UCA0IV,4))
  {
  /*switch(Buff)
  {*/
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    while (!(UCA0IFG&UCTXIFG));                                                // USCI_A0 TX buffer ready?
    P1OUT ^= BIT0;
    UCA0TXBUF = UCA0RXBUF;                  // TX -> RXed character ��ʲô��ʲô
    break;
  case 4: P1OUT |= BIT0;break;                             // Vector 4 - TXIFG
  default: break;
  UCA0IFG &= ~UCRXIFG;
  }
}