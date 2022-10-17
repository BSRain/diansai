/*ADC_12单通道单次转换实现功能当采集电压>0.5V灯亮,电压<0.5灯灭*/
#include <msp430f5529.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  ADC12CTL0 = ADC12SHT02 + ADC12ON;         // Sampling time, ADC12 on  ADC12采样保持 +  打开ADC12内核
  ADC12CTL1 = ADC12SHP;                     // Use sampling timer 用取样计时器
  ADC12IE = 0x01;                           // Enable interrupt
  ADC12CTL0 |= ADC12ENC;                    //使能转换
  P6SEL |= 0x01;                            // P6.0 ADC option select开启P6.1特殊功能寄存器
  P1DIR |= 0x01;                            // P1.0 output

  while (1)
  {
    ADC12CTL0 |= ADC12SC;                   // Start sampling/conversion启动转换

    __bis_SR_register(LPM0_bits + GIE);     // LPM0, ADC12_ISR will force exit
  }
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
  switch(__even_in_range(ADC12IV,34))
  {
  case  0: break;                           // Vector  0:  No interrupt
  case  2: break;                           // Vector  2:  ADC overflow
  case  4: break;                           // Vector  4:  ADC timing overflow
  case  6:                                  // Vector  6:  ADC12IFG0
    if (ADC12MEM0 >= 0x7ff)                 // ADC12MEM = A0 > 0.5AVcc?
      P1OUT |= BIT0;                        // P1.0 = 1
    else
      P1OUT &= ~BIT0;                       // P1.0 = 0

    __bic_SR_register_on_exit(LPM0_bits);   // Exit active CPU
  case  8: break;                           // Vector  8:  ADC12IFG1
  case 10: break;                           // Vector 10:  ADC12IFG2
  case 12: break;                           // Vector 12:  ADC12IFG3
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
