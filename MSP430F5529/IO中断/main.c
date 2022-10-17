/*�����жϿ���LE�Ͱ�������*/
#include <msp430.h>
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR |= BIT0;                            // Set P1.0 to output direction
  P2REN |= BIT1;                            // Enable P2.1 internal resistance
  P2OUT |= BIT1;                            // Set P2.1 as pull-Up resistance
  P2IES |= BIT1;                             // �ߵ�ƽ��ʾ�½��ش������͵�ƽ��ʾ�����ش�����
  P2IFG &= ~BIT1;                           // P2.1IFG cleared //����жϱ�־
  P2IE |= BIT1;                             // P2.1interrupt enabled

  __bis_SR_register(LPM4_bits + GIE);       // Enter LPM4 w/interrupt//����͹���ģʽ4 �� ����ȫ���ж�
}
#pragma vector=PORT2_VECTOR                 //�����ж�������ַ
__interrupt void Port_1(void)                       //�жϷ�����

{
   __delay_cycles(10000);                   //��ʱ����
  if((P2IN & BIT1)==0)
  P1OUT ^= BIT0;                            // P1.0 = toggle
  P2IFG &= ~BIT1;                          // P2.1 IFG cleared
}
