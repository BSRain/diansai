#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
float anglex,angley,anglez;
unsigned char RX_flag;
unsigned char uart1_RX[60];
unsigned char count=0;
void delay(int ms)        //��ʱ����
{
	for(int i=0;i<ms;i++)
	for(int j=0;j<240;j++);
}
void senfchar(char s)
{
    UCA1TXBUF=s;
    while(!(UCA0IFG&UCTXIFG));
}

void sendstring(unsigned char *p)
{
    while(*p!='\0')
    {
        while(!(UCA0IFG&UCTXIFG));
        UCA1TXBUF=*p++;
        delay(50);
    }
}
void initClockTo16MHz()
{
    UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
    UCSCTL4 |= SELA_2;                        // Set ACLK = REFO
    __bis_SR_register(SCG0);                  // Disable the FLL control loop
    UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
    UCSCTL1 = DCORSEL_5;                      // Select DCO range 16MHz operation
    UCSCTL2 = FLLD_0 + 487;                   // Set DCO Multiplier for 16MHz
                                              // (N + 1) * FLLRef = Fdco
                                              // (487 + 1) * 32768 = 16MHz
                                              // Set FLL Div = fDCOCLK
    __bic_SR_register(SCG0);                  // Enable the FLL control loop
 
    // Worst-case settling time for the DCO when the DCO range bits have been
    // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
    // UG for optimization.
    // 32 x 32 x 16 MHz / 32,768 Hz = 500000 = MCLK cycles for DCO to settle
    __delay_cycles(500000);//
    // Loop until XT1,XT2 & DCO fault flag is cleared
    do
    {
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG); // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                          // Clear fault flags
    }while (SFRIFG1&OFIFG);                         // Test oscillator fault flag
}

void UART_Init( char Parity, char DataBits, char StopBits)
{
  P4SEL = BIT4 + BIT5;                        // P4.4,5 = USCI_A0 TXD/RXD
  UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA1CTL1 |= UCSSEL_2;                     // SMCLK 25mhz
  UCA1BR0 = 8;                              // 16MHz 115200 (see User's Guide)
  UCA1BR1 = 0;                              // 16MHz 115200
  UCA1MCTL = UCBRS_0 + UCBRF_11 + UCOS16;   // Modln UCBRSx=0, UCBRFx=0,
  
//  ------------------------------����У��λ------------------------------
  switch (Parity)
  {
  case 'n':
  case 'N':
    UCA1CTL0 &= ~UCPEN;
    break;          //��У��
  case 'p':
  case 'P':
    UCA1CTL0 |= UCPEN + UCPAR;
    break;    //żУ��
  case 'o':
  case 'O':
    UCA1CTL0 |= UCPEN;
    break;          //��У��
    //default:  return (0);                                 //��������
  }
  //------------------------------��������λ------------------------------
  switch (DataBits)
  {
  case 7:
  case '7':
    UCA1CTL0 |= UC7BIT;
    break;         //7λ����λ
  case 8:
  case '8':
    UCA1CTL0 &= ~UC7BIT;
    break;         //8λ����λ
    //default:  return (0);                                 //��������
  }
  //------------------------------����ֹͣλ------------------------------
  switch (StopBits)
  {
  case 1:
  case '1':
    UCA1CTL0 &= ~UCSPB;
    break;          //1λֹͣλ
  case 2:
  case '2':
    UCA1CTL0 |= UCSPB;
    break;          //2λֹͣλ
    //default:  return (0);                                 //��������
  }
  
  // over sampling
  UCA1CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
  UCA1IE |= UCRXIE; //�����ж�
  UCA1IE |= UCTXIE; //���������ж�
  
}

void data_receive(unsigned char data)
{
  
  if(data == 0x55 && RX_flag ==0)
  {
    RX_flag = 1;
  }
  else if(count >= 11 && RX_flag == 1)
  {
    RX_flag = 0;
    if(uart1_RX[1] == 0x53)//�Ƕ�
    {
      anglex = ((float)((int)uart1_RX[3]<<8 |(int)uart1_RX[2])) /32768.0*180;
      angley = ((float)((int)uart1_RX[5]<<8 |(int)uart1_RX[4])) /32768.0*180;
      anglez = ((float)((int)uart1_RX[7]<<8 |(int)uart1_RX[6])) /32768.0*180;
      unsigned char anglexx[sizeof(anglex)];
      sprintf((char *)anglexx,"%.3f\n",anglex);
      sendstring(anglexx);
      __delay_cycles(1000000);
      //UCA1TXBUF=(char)anglex; //�ѽǶȷ���ȥ�����������߷����˾Ͱ�anglezǿ��ת��Ϊ�ַ�
      //printf("%lf\n",anglex);
      unsigned char angleyy[sizeof(angley)];
      sprintf((char *)angleyy,"%.3f\n",angley);
      sendstring(angleyy);
      __delay_cycles(1000000);
      //UCA1TXBUF=(char)angley; //�ѽǶȷ���ȥ�����������߷����˾Ͱ�anglezǿ��ת��Ϊ�ַ�
      //printf("%lf\n",angley);
      unsigned char anglezz[sizeof(anglez)];
      sprintf((char *)anglezz,"%.3f\n",anglez);
      sendstring(anglezz);
      __delay_cycles(1000000);
      //UCA1TXBUF=(char)anglez; //�ѽǶȷ���ȥ�����������߷����˾Ͱ�anglezǿ��ת��Ϊ�ַ�
      //printf("%lf\n",anglez);
    }
    count=0;
    memset(uart1_RX,0,sizeof(uart1_RX));
  }
  
  if(RX_flag == 1)
  {
    uart1_RX[count] = data;
    
    if (count == 1 && data != 0x53)
    {
      RX_flag =0;
      count=0;
      memset(uart1_RX,0,sizeof(uart1_RX));
    }
    else
      count ++;
  }
}

int main(void) {
  
  WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer
  initClockTo16MHz();                        //����ϵͳʱ��Ϊ16Mhz,�����ô��ڲ�����ȷ�շ�
  UART_Init('n',8,1);
  _EINT();//�������ж�
  
  while(1){
    
  }
  
}

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch (__even_in_range(UCA1IV, 4))
  {
  case 0:
    break;                 // Vector 0 - no interrupt
  case 2:                       // Vector 2 - RXIFG
    data_receive(UCA1RXBUF);
    break;
  case 4:                       // Vector 4 - TXIFG
    
    break;
  default:
    break;
  }
}