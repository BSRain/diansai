/*�ڱȽϺ�����ģʽ�²���PWM�������β��� */

/*��һ��PWM�����ô�������ֱ�����������֪������ֱ��������������ĵ�����Ƶ�ʲ���Ӱ��ת�� ��ֻ��ռ�ձȻ�Ӱ��ת��*/

/*��������P2.0�����������ģ���������һ�˿����PWM*/

/*��CPU���ŷ��֣�P2.0ΪTA1.1��Ҳ���Ƕ�ʱ��A1��1�Ų���Ƚ��������*/

#include <msp430.h>

void main(void)

{

   WDTCTL=WDTPW+WDTHOLD;
   P2SEL|=BIT0;   //���������⹦�ܣ�������ͨI/Oʹ��
   P2DIR |= BIT0;  // ���
   P2DS |= BIT0;//ȫ����������������޷��������
   P2OUT &= ~BIT0;

   /*��SMCL����ΪXT2 4MHZ*/
   P5SEL = BIT2 + BIT3; //�������⹦�ܣ��������ⲿʱ�Ӿ���XT2����
   UCSCTL6&=~XT2OFF; //����XT2
   while(SFRIFG1 & OFIFG)

   {

      UCSCTL7 &=~(XT2OFFG+DCOFFG+XT1LFOFFG);//���3��ʱ�Ӵ����־
      SFRIFG1 &=~(OFIFG);
   }//ֱ��XT2��������������û�д�����
   UCSCTL4|=SELS_5;//��SMCLK��ʱ��ԴѡΪXT2 4MHZ
   TA1CCTL0=CCIE;//��ʱ��A1�Ĳ���Ƚ���0����CCIFGλ�ж�
   TA1CCR0=200;//���������ֵ����PWMƵ��Ϊ10KHZ
   TA1CCTL1=CCIE;//����Ƚ���1�����ж�
   TA1CCR1=50;//ռ�ձ�Ϊ75%
   TA1CTL|=TACLR;//����ʱ��A1����
   TA1CTL|=TASSEL_2+MC_3;//��ʱ��ѡ��SMCLK��Ϊʱ��Դ����Ϊ����ģʽ
   TA1CCTL1=OUTMOD_4;//��ʱ��A1�еĲ���Ƚ���1�����ģʽΪ4��ת
   while(1);

}