#include "msp430.h"
//#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define CPU_F ((double)1035000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 
int cnt=0;
void senfchar(char s)
{
  UCA0TXBUF=s;
  while(!(UCA0IFG&UCTXIFG));
}

void sendstring(unsigned char *p)
{
  while(*p!='\0')
  {
    while(!(UCA0IFG&UCTXIFG));
    UCA0TXBUF=*p++;
  }
}

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  //  WDTCTL = WDT_ADLY_250;
  //  SFRIE1 |= WDTIE;
  //  unsigned char i;
  //  UCSCTL1&=~XT2OFF;                 //打开XT2振荡器
  //  UCSCTL2|=SELM1+SELS1;              //MC LK为8MHZ，SMCLK为8MHZ
  //  do{
  //    SFRIFG1&=~OFIFG;                   //清楚振荡器错误标志
  //    for(i=0;i<100;i++)
  //       _NOP();
  //  }
  //  while((SFRIFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
  //  SFRIFG1&=~OFIFG; 
  //  
  //  _EINT();
  
  //  P5SEL |= BIT2|BIT3; //将IO配置为XT2功能  
  //  UCSCTL6 &= ~XT2OFF; //使能XT2  
  //  
  //  UCSCTL4 = UCSCTL4&(~(SELA_7))|SELA_1; //先将ACLK配置为VLOCLK  
  //  UCSCTL3 |= SELREF_2;                  //将REFCLK配置为REFCLK  
  //  
  //  while (SFRIFG1 & OFIFG){  
  //    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);         // 清除三类时钟标志位  
  //                              // 这里需要清除三种标志位，因为任何一种  
  //                              // 标志位都会将OFIFG置位  
  //    SFRIFG1 &= ~OFIFG;                                  // 清除时钟错误标志位  
  //  }  
  //  UCSCTL4 = UCSCTL4&(~(SELS_7|SELM_7))|SELS_5|SELM_5;     //将SMCLK和MCLK时钟源配置为XT2  
  
//  P3SEL = BIT3+BIT4;                        // P3.4,5 = USCI_A0 TXD/RXD
//  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
//  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
//  UCA0BR0 = 6;                              // 1MHz 9600 (see User's Guide)
//  UCA0BR1 = 0;                              // 1MHz 9600
//  //UCA0MCTL = UCBRS_5 + UCBRF_3 + UCOS16;   // Modln UCBRSx=0, UCBRFx=0,
//  UCA0MCTL = UCBRS_0+ UCBRF_13 + UCOS16;                                         // over sampling
//  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
//  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
  //sendstring("hello world\r\n");
  
  //__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
  //  __no_operation();                         // For debugger
  
  P2DIR|=BIT6;
  P2DIR |=BIT3;
  
  P1DIR |=BIT2;
  P1OUT |=BIT2;
  
  P1IE |= BIT6;
  P1IES |= BIT6;
  //P1IES &= ~BIT5;
  P1IFG &= ~BIT6;
  P1REN |=  BIT6;
  P1OUT |=  BIT6;
__bis_SR_register(GIE); 
  while(1)
  {
    P2OUT|=BIT6;
    P2OUT&=~BIT3;
    P1OUT |=BIT2;
    delay_ms(1000);
    P2OUT&=~BIT6;
    P2OUT&=~BIT3;
    delay_ms(1000);
    //sendstring("hello world\r\n");
    
    
  }
  
}
//// Echo back RXed character, confirm TX buffer is ready first
//#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
//#pragma vector=USCI_A0_VECTOR
//__interrupt void USCI_A0_ISR(void)
//#elif defined(__GNUC__)
//void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
//#else
//#error Compiler not supported!
//#endif
//{
//  switch(__even_in_range(UCA0IV,4))
//  {
//  case 0:break;                             // Vector 0 - no interrupt
//  case 2:                                   // Vector 2 - RXIFG
//    while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
//    UCA0TXBUF = UCA0RXBUF;                  // TX -> RXed character
//    break;
//  case 4:break;                             // Vector 4 - TXIFG
//  default: break;
//  }
//}

#pragma vector = PORT1_VECTOR    //S1中断服务函数
__interrupt void P1_ISR()
{
	
          cnt++;

	P1IFG &=~ BIT6;
}