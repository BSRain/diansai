#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR |= BIT0;                            // Set P1.0 to output direction
  P2REN |= BIT1;                //I/O����Ĭ������ PxREN ��1ʹ����/��������  ��0�ر���/��������
  P2OUT |= BIT1;                //����PxREN �Ĵ�����PxOUT ��1Ϊ�������裻 ��0Ϊ�������裻

  while (1)                                 // Test P2.1
  {
    if (P2IN & BIT1)
       P1OUT &= ~BIT0;                      // if P2.1 set, set P2.1
    else
      P1OUT |= BIT0;                        // else reset
  }

}