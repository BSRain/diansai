#include <msp430.h>//序列通道单词转换

volatile unsigned int results[4];


int main(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
  P6SEL = 0x0F;                             // Enable A/D channel inputs启用 a/d 通道输入
  ADC12CTL0 = ADC12ON+ADC12MSC+ADC12SHT0_2; // Turn on ADC12, set sampling time 打开 adc12设定采样时间
  ADC12CTL1 = ADC12SHP+ADC12CONSEQ_1;       // Use sampling timer, single sequence 使用取样定时器+序列通道单次转化
  ADC12MCTL0 = ADC12INCH_0;                 // ref+=AVcc, channel = A0    (P6.0)
  ADC12MCTL1 = ADC12INCH_1;                 // ref+=AVcc, channel = A1   (P6.1)
  ADC12MCTL2 = ADC12INCH_2;                 // ref+=AVcc, channel = A2  (P6.2)
  ADC12MCTL3 = ADC12INCH_3+ADC12EOS;        // ref+=AVcc, channel = A3, end seq  ADC通道3(P6.3)  +置位表示最后一个通道转换完成*.
  ADC12IE = 0x08;                           // Enable ADC12IFG.3
  ADC12CTL0 |= ADC12ENC;                    // Enable conversions

  while(1)
  {
    ADC12CTL0 |= ADC12SC;                   //启动转换 Start convn - software trigger

    __bis_SR_register(LPM4_bits + GIE);     // Enter LPM4, Enable interrupts 进入低功耗模式4 + 开全局中断
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
