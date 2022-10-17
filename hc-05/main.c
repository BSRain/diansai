#include <msp430f5529.h>     // �����ѵ��� ��ֱ��ʹ��
void main(void)
{
    WDTCTL = WDTPW +WDTHOLD;    // Stop WDT
    P3SEL |= 0xC0;              // P3.6,7 = USART1 TXD/RXD
    ME2 |= UTXE1 + URXE1;       // EnableUSART1 TXD/RXD
    UCTL1 |= CHAR;              // 8-bitcharacter
    UTCTL1 |= SSEL0;            // UCLK = ACLK �����ڷ��ͼĴ���ѡ����ʱ�ӣ�
    UBR01 = 0x03;               //32k/9600 - 3.41  ��UBR�������ʿ��ƼĴ�����
    UBR11 = 0x00;               //
    UMCTL1 = 0x4A;              // Modulation ���ʵ���
    UCTL1 &= ~SWRST;            // Initialize USART statemachine
    IE2 |= URXIE1;              // EnableUSART1 RX interrupt
    _BIS_SR(LPM3_bits +GIE);    // Enter LPM3 w/interrupt 
}
#pragma vector=USART1RX_VECTOR  // ���ڶ�ȡ�ж�
__interrupt void usart1_rx (void)
{
   while (!(IFG2 &UTXIFG1));    // USART1 TXbuffer ready? 
   TXBUF1 = RXBUF1;             // RXBUF1to TXBUF1
}