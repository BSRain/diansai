#include "msp430.h"
#include "hx711.h"
//****************************************************
//��ʱ����
//****************************************************
void Delay_hx711_us(unsigned int t)
{
	//_nop_();
	//_nop_();
    while(t--);
}
// I/O����
void PORT_Init(void)
{
    P2SEL &=~ (BIT0+BIT2);
    P2DIR &=~ BIT0;//SDA ��ȡ
    P2DIR |=  BIT2;//SCK ʱ��
}
//****************************************************
//
//****************************************************
unsigned long HX711_Read(void)	//
{
	unsigned long count; 
	unsigned char i; 
  	P2OUT |= BIT2; //SDA
	Delay_hx711_us(4);
  	P2OUT &=~ BIT0;//SCK=0
  	count=0; 
  	while( P2IN&BIT2 ); //SDA�ȴ�
  	for(i=0;i<24;i++)
	{ 
          
          P2OUT |= BIT0; //SCK
          count=count<<1; 
	P2OUT &=~ BIT0;//SCK
 	if(P2IN&BIT2)//SDA
	count++;   
        } 
        
 	P2OUT |= BIT0; //SCK
        count=count^800000;
	P2OUT &=~ BIT0;//SCK
        Delay_hx711_us(4);
       // P5OUT |= BIT1; //SCK
	//Delay_hx711_us(4);
	//P5OUT &=~ BIT1;//SCK
        count=count>>8;
	return(count);
}