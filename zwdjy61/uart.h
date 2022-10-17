
#ifndef UART_H_
#define UART_H_
unsigned char Re_buf[11],counter;
unsigned char sign;
void Uart_Init();
void Uart_Init(){

  P4SEL = BIT5+BIT4;                        // P3.4,5 = USCI_A0 TXD/RXD
  UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA1CTL1 |= UCSSEL_2;                     // SMCLK
  UCA1BR0 = 8;                              // 1MHz 9600 (see User's Guide)
  UCA1BR1 = 0;                              // 1MHz 9600
  UCA1MCTL = UCBRS_6 + UCBRF_0;   // Modln UCBRSx=0, UCBRFx=0,
                                            // over sampling
  UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

}


#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)

{

  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    //while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
    Re_buf[counter]= UCA1RXBUF;                  // TX -> RXed character
    if(counter==0&&Re_buf[0]!=0x55) return; 
    counter++;
    if(counter==11) 
    {
        counter=0; 
        sign=1;    
    }
    break;
  case 4:break;                             
  default: break;
  }
}
#endif /* UART_H_ */
