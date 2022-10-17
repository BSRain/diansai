/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author
  * @version V1.0
  * @date    2019-08-04
  * @brief   led应用函数接口
  ******************************************************************************
  */

#include "Black_USART.h"
#include "GO/Black_DELAY.h"

void USART0_Config(void)
{
    //串口初始化
     P3SEL    |=  BIT3+BIT4 ;                        // P5.6,7 = USCI_A1 TXD/RXD
     UCA0CTL1 |=  UCSWRST;                      // **Put state machine in reset**
     UCA0CTL1 |=  UCSSEL_1;                     // ACLK
     UCA0BR0   =  0x03;                         // 32768Hz 9600 波特率为9600
     UCA0BR1   =  0x00;                             // 32768Hz 9600
     UCA0MCTL |=  UCBRS_3 + UCBRF_0;           // Modulation UCBRSx=1, UCBRFx=0
     UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
     UCA0IE   |=  UCRXIE;                       // Enable USCI_A1 RX interrupt 使能中断
}


void USART1_Config(void)
{
    //串口初始化
     P4SEL    |=  BIT4+BIT5 ;                        // P5.6,7 = USCI_A1 TXD/RXD
     UCA1CTL1 |=  UCSWRST;                      // **Put state machine in reset**
     UCA1CTL1 |=  UCSSEL_1;                     // ACLK
     UCA1BR0   =  0x03;                         // 32768Hz 9600 波特率为9600
     UCA1BR1   =  0x00;                             // 32768Hz 9600
     UCA1MCTL |=  UCBRS_3 + UCBRF_0;           // Modulation UCBRSx=1, UCBRFx=0
     UCA1CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
     UCA1IE   |=  UCRXIE;                       // Enable USCI_A1 RX interrupt 使能中断
}

void send0_buf(unsigned char *ptr)    //发送字符串
{
    while(*ptr != '\0')
    {
        while(!(UCA0IFG & UCTXIFG));
        UCA0TXBUF = *ptr;
        ptr++;
        delay(10);
    }
}
void send1_buf(unsigned char *ptr)    //发送字符串
{
    while(*ptr != '\0')
    {
        while(!(UCA1IFG & UCTXIFG));
        UCA1TXBUF = *ptr;
        ptr++;
        delay(10);
    }
}

