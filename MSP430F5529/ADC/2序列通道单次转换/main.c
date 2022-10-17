#include <msp430.h>//����ͨ������ת��

volatile unsigned int results[4];


int main(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
  P6SEL = 0x0F;                             // Enable A/D channel inputs���� a/d ͨ������
  ADC12CTL0 = ADC12ON+ADC12MSC+ADC12SHT0_2; // Turn on ADC12, set sampling time �� adc12�趨����ʱ��
  ADC12CTL1 = ADC12SHP+ADC12CONSEQ_1;       // Use sampling timer, single sequence ʹ��ȡ����ʱ��+����ͨ������ת��
  ADC12MCTL0 = ADC12INCH_0;                 // ref+=AVcc, channel = A0    (P6.0)
  ADC12MCTL1 = ADC12INCH_1;                 // ref+=AVcc, channel = A1   (P6.1)
  ADC12MCTL2 = ADC12INCH_2;                 // ref+=AVcc, channel = A2  (P6.2)
  ADC12MCTL3 = ADC12INCH_3+ADC12EOS;        // ref+=AVcc, channel = A3, end seq  ADCͨ��3(P6.3)  +��λ��ʾ���һ��ͨ��ת�����*.
  ADC12IE = 0x08;                           // Enable ADC12IFG.3
  ADC12CTL0 |= ADC12ENC;                    // Enable conversions

  while(1)
  {
    ADC12CTL0 |= ADC12SC;                   //����ת�� Start convn - software trigger

    __bis_SR_register(LPM4_bits + GIE);     // Enter LPM4, Enable interrupts ����͹���ģʽ4 + ��ȫ���ж�
  }
}

#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
                                    // Vector 12:  ADC12IFG3
    results[0] = ADC12MEM0;                 // Move results, IFG is cleared
    results[1] = ADC12MEM1;                 // Move results, IFG is cleared
    results[2] = ADC12MEM2;                 // Move results, IFG is cleared
    results[3] = ADC12MEM3;                 // Move results, IFG is cleared
    __bic_SR_register_on_exit(LPM4_bits);   // Exit active CPU, SET BREAKPOINT HERE
}
