#include <msp430.h>//序列通道多次转换

#define   Num_of_Results   8

volatile unsigned int A0results[Num_of_Results];
volatile unsigned int A1results[Num_of_Results];
volatile unsigned int A2results[Num_of_Results];
volatile unsigned int A3results[Num_of_Results];

int main(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer  关闭看门狗
  P6SEL = 0x0F;                             // Enable A/D channel inputs 初始化采集管脚口
  ADC12CTL0 = ADC12ON+ADC12MSC+ADC12SHT0_8; // Turn on ADC12, extend sampling time开启ADC12内核 + ADC12多次样本转换打开 + 采样保持
                                            // to avoid overflow of results   为了避免结果溢出
  ADC12CTL1 = ADC12SHP+ADC12CONSEQ_3;       // Use sampling timer, repeated sequence 用取样计时器+序列通道多次转换
  ADC12MCTL0 = ADC12INCH_0;                 // ref+=AVcc, channel = A0    3.3V
  ADC12MCTL1 = ADC12INCH_1;                 // ref+=AVcc, channel = A1
  ADC12MCTL2 = ADC12INCH_2;                 // ref+=AVcc, channel = A2
  ADC12MCTL3 = ADC12INCH_3+ADC12EOS;        // ref+=AVcc, channel = A3, end seq.通道(P6.3) + 置位表示最后一个通道转换完成*
  ADC12IE = 0x08;                           // Enable ADC12IFG.3 //中断使能
  ADC12CTL0 |= ADC12ENC;                    // Enable conversions使能转换
  ADC12CTL0 |= ADC12SC;                     // Start convn - software trigger 启动转换

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, Enable interrupts

}

#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
  static unsigned int index = 0;

  switch(__even_in_range(ADC12IV,34))
  {
  case  0: break;                           // Vector  0:  No interrupt
  case  2: break;                           // Vector  2:  ADC overflow
  case  4: break;                           // Vector  4:  ADC timing overflow
  case  6: break;                           // Vector  6:  ADC12IFG0
  case  8: break;                           // Vector  8:  ADC12IFG1
  case 10: break;                           // Vector 10:  ADC12IFG2
  case 12:                                  // Vector 12:  ADC12IFG3
    A0results[index] = ADC12MEM0;           // Move A0 results, IFG is cleared
    A1results[index] = ADC12MEM1;           // Move A1 results, IFG is cleared
    A2results[index] = ADC12MEM2;           // Move A2 results, IFG is cleared
    A3results[index] = ADC12MEM3;           // Move A3 results, IFG is cleared
    index++;                                // Increment results index, modulo; Set Breakpoint1 here

    if (index == 8)
    {
      (index = 0);
    }
  case 14: break;                           // Vector 14:  ADC12IFG4
  case 16: break;                           // Vector 16:  ADC12IFG5
  case 18: break;                           // Vector 18:  ADC12IFG6
  case 20: break;                           // Vector 20:  ADC12IFG7
  case 22: break;                           // Vector 22:  ADC12IFG8
  case 24: break;                           // Vector 24:  ADC12IFG9
  case 26: break;                           // Vector 26:  ADC12IFG10
  case 28: break;                           // Vector 28:  ADC12IFG11
  case 30: break;                           // Vector 30:  ADC12IFG12
  case 32: break;                           // Vector 32:  ADC12IFG13
  case 34: break;                           // Vector 34:  ADC12IFG14
  default: break;
  }
}
