#include "msp430f5529.h"

void main(void)
{
   WDTCTL = WDTPW + WDTHOLD;//�رտ��Ź���ʱ��

   P1DIR |= BIT0;
   P2DIR &= ~BIT5;         //P2.5����Ϊ����
   P2SEL |= BIT5;          //PxSEL ���⹦�ܼĴ��� ��P2.5��Ϊ��ʱ����������
   TA2CTL = TASSEL_2 + MC_2 + TACLR;  //ѡ��ʱ��SMCLK(1.05MHZ) +  ��������ģʽ + ��ʱ������������
   TA2CCTL2 = CM_1 + SCS + CAP + CCIE; //�����ز�׽ +  ʱ��ͬ�� + ����ģʽ + ����Ƚ���1�����ж�
   _EINT();                   //�������ж�
   while(1);
}
#pragma vector = TIMER2_A1_VECTOR
__interrupt void TIMER2_A1_ISR(void)
{
  P1OUT ^= BIT0;
  TA2CCTL2 &= ~CCIFG;  //�жϱ�־λ��Ҫ����ֶ�����
}