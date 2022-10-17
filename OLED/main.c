#include"MSP430.h"
#include"12864.h"
void delay(int ms)
{
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
   for(jj=0;jj<500;jj++);  
       
} 

void main(void) 
{
  //unsigned char i=0;    
  WDTCTL = WDTPW + WDTHOLD;//ֹͣ���Ź�
  P6DIR |=BIT0+BIT1+BIT2+BIT3;  
  LCD_Init();
  while(1)
  {
      LCD_Fill(0xff);
      Draw_LibLogo();      
      delay(1000); 
      LCD_Fill(0x00);       //����
      delay(10); 
      LCD_P14x16Ch(36,0,0); //��    //������14*16�Ĵ�С����x����ľ�����14
      LCD_P14x16Ch(50,0,1); //��
      LCD_P14x16Ch(64,0,2); //��
      LCD_P14x16Ch(78,0,3); //��   
      LCD_P8x16Str(48,4,"YangFeng");
      LCD_P6x8Str(34,7,"2015-01-02");
      delay(200);   
      LCD_CLS();
     // Draw_BMP(0,0,128,64);  //ͼƬ��ʾ
      delay(2000);
      LCD_CLS();
    }
  }