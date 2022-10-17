/*����Timer_A�Ƚ�ģʽ�µĶ�·��ʱ����LED��˸*/
#include "msp430f5529.h"

void main(void)

{

   WDTCTL=WDTPW+WDTHOLD;

   P1DIR |=(BIT0+BIT2+BIT3+BIT4+BIT5);      //P1.1-P1.5Ϊ�������   ��������������������
   P1OUT |= 0X00;    //ȫ�����ͣ���ʼ��LEDȫ��

   TA0CCTL1 = CCIE;          //����Ƚ���1����CCIFGλ�жϡ�����
   TA0CCR1=13107;             //����Ҫ�Ƚϵ���ֵ0xff/5=13107
   TA0CCTL2 = CCIE;         //����Ƚ���2�����жϡ�������������������      ��������������������
   TA0CCR2=26214;            //13107*2=26214    ����������������������
   TA0CCTL3 = CCIE;          //����Ƚ���3�����ж�       ����������������
   TA0CCR3 = 39321;           //13107*3=39321    ��������������������
   TA0CCTL4 = CCIE;        //����Ƚ���4�����ж�   ��������������������
   TA0CCR4=52428;          //13107*4=52428      ����������������
   TA0CTL|=TASSEL_1+MC_2+TACLR+TAIE;    //ѡ��SCLK32.768KHZ��Ϊʱ�ӣ�ѡ������ģʽ���������ж�
   /*�����Ļ���5������һ���ʱ��Ϊ0xffff/32768=2S*/

  __enable_interrupt();
  while(1);

}

/*TIMER0_A0_VECTOR�Ǽ�ʱ��0��CCR0���жϼĴ�����TIMER0_A1_VECTOR�Ǽ�ʱ��0��CCR1-CCR4��TA�ļĴ���*/

/*ͬ��ʱ��TA1Ҳ�Ƿ�Ϊ����TIMER1_A0_VECTOR��TIMER1_A1_VECTOR*/

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TimerA(void)
{
  switch(__even_in_range(TA0IV,14))    /* ��仰����˼�ǣ�ֻ����TA0IV��ֵ����0--14�ڵ�ż��ʱ�Ż�ִ��switch�����ڵ����
            �����������switch����Ч��*/
  {
  case 2:P1OUT = BIT0;break;//TACCR1 CCIFG��λ����������ֵ���趨��13107����ˣ�Ҳ����˵����0.4S��
  case 4:P1OUT = BIT2;break;//TACCR2 CCIFG��λ����������0.8S��
  case 6:P1OUT = BIT3;break;//TACCR3 CCIFG��λ����������1.2S��
  case 8:P1OUT = BIT4;break;//TACCR4 CCIFG��λ����������1.6S��
  case 14:P1OUT = BIT5;break; //TAIFG��λ����������2S��
  }
}





