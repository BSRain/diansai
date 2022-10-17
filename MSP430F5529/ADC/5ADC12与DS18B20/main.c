#include "MSP430f5529.h"
#include "DS18B20.h"
void System_overclocking(unsigned char Fre) //��Ƶ����
{
  P5SEL |= BIT2|BIT3|BIT4|BIT5;//�����ⲿ����ʱ��

  UCSCTL6 |= XCAP_3|XT1OFF;          // XT1 ��� ����
  UCSCTL6 |= XT2DRIVE_0 |XT2OFF;     // XT2 ��� ����
  //�������������ĵ�ѹ���ֵĴ���
  PMMCTL0_H = 0xA5;                                         //��PMM��Դ����
  SVSMLCTL |= SVSMLRRL_1 + SVMLE;                            //����SVML��ѹ
  PMMCTL0 =  PMMPW + PMMCOREV_3;                             //�����ں˵�ѹ
  while((PMMIFG & SVSMLDLYIFG ) == 0);                       //�ȴ��������
  PMMIFG &= ~(SVMLVLRIFG + SVMLIFG + SVSMLDLYIFG);
  if((PMMIFG & SVMLIFG) == 1)                                //�ж��ں˵�ѹ�Ƿ�������VSVML
    while((PMMIFG & SVMLVLRIFG) == 0);                    //���û�еȴ�
  SVSMLCTL &= ~SVMLE;                                        //�ص�SVMLģ��
  PMMCTL0_H = 0X00;

  __bis_SR_register(SCG0);                 //���﷨Ϊ�̶���ʽ����Ϊ�������ڵı�����λ��SCG0��ϵͳ����ģʽ�йأ���ʱ MCLK ��ͣ����
  UCSCTL0 = 0;                             //�����㣬FLL ����ʱ���üĴ���ϵͳ���Զ����ã����ù�
  UCSCTL6 = (UCSCTL6&(~(XT2OFF|XT1OFF))|XCAP_3|XT2DRIVE_0);
  UCSCTL3 = (5<<4)|(2<<0);                 // ѡ�� XTAL2 ��ʱ���ź���Ϊ�ο��ź� ���ҷ�Ƶ��1MHz
  if(Fre < 5)
    UCSCTL1 = DCORSEL_2;
  else if(Fre<15)
    UCSCTL1 = DCORSEL_4;
  else
    UCSCTL1 = DCORSEL_7;
  UCSCTL2 = (Fre-1);
  __bic_SR_register(SCG0);
  __delay_cycles(782000);
  while (SFRIFG1 & OFIFG) {                               // Check OFIFG fault flag
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);           // Clear OSC flaut Flags
    SFRIFG1 &= ~OFIFG;                                    // Clear OFIFG fault flag
  }
  UCSCTL4 = UCSCTL4&(~(SELS_7|SELM_7))|SELS_3|SELM_3;

  P2DIR |= BIT2;  // ����P2.2Ϊ���ģʽ
  P2SEL |= BIT2;  // ���ܸ���:SMCLK��� ����ϵͳ��ʱ�ӣ�
  P7DIR |= BIT7;  // ����P7.7Ϊ���ģʽ
  P7SEL |= BIT7;  // ���ܸ���:MCLK���    ����ϵͳʱ�ӣ�
}
void main()
{
   WDTCTL = WDTPW + WDTHOLD;
   System_overclocking(12);   //12MHZ
   while(1)
   {
    B20_Read_temp();          //��ȡ�¶�ֵ
   }
}