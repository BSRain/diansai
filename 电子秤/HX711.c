#include "HX711.h"
#include <msp430f5529.h>
//#include <stdio.h>
long HX711_Buffer = 0;
long Weight_Maopi = 0;    // �ϵ�ʱëƤ����
long Weight_Shiwu = 0;
int GapValue = 0;
uint8_t HX711_SCK;
uint8_t HX711_DT;  
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
//unsigned long count;
int j;
int weight=0;
char buffer[200];
double money;
uint temp;
ulong a;
uint aa;
ulong hx711_dat;
uint get;

//��ʱ����
void delayus(uint t)
{
//_nop_();
//_nop_();
  while(t--);
}
// I/O����
void PORT_Init(void)
{
  P1SEL &=~ BIT2;
  P1DIR &=~ BIT2;//SDA
  P2DIR |= BIT2;//SCK
}
//p2.2 SCK
//p1.2 DT
//��ȡHX711
unsigned long HX711_Read(void) //����128
{
  unsigned long count;
  unsigned char i;
  P1OUT |= BIT2; //SDA
  delayus(1);
  P2OUT &= ~BIT2;//SCK
  delayus(1);
  count=0;
  //while( P1IN&BIT2 ); //SDA
  for(i=0;i<24;i++)
  {
    P2OUT |= BIT2; //SCK
    delayus(1);
    count=count<<1;
    P2OUT &=~ BIT2;//SCK
    if(P1IN&BIT2)//SDA
    count++;
  }
  P2OUT |= BIT2; //SCK
  delayus(1);
  count=count^0x800000;
  delayus(1);
  P2OUT &=~ BIT2;//SCK
  delayus(1);
  return(count);
}


unsigned int get_pizhong() //��ȡƤ�أ���������
{
  uint j;
  // ulong hx711_dat;
  for(j=0;j<5;j++)
  delayus(4);
  hx711_dat=HX711_Read(); //HX711ADת�����ݴ���
  temp=(uint)(hx711_dat/429.4967296); //����long����Ϊint�ͣ����㴦��
  
  //temp=temp-19672;
  //temp=temp/0.897;
  
  return (temp);
}


long HX711_Get_Weight()
{
 HX711_Buffer = HX711_Read();    ///��ȡ��ʱ�Ĵ��������ֵ
 Weight_Shiwu = HX711_Buffer;     ///�������������ֵ����
 Weight_Shiwu = Weight_Shiwu - Weight_Maopi; //��ȡʵ���AD������ֵ��
 Weight_Shiwu = (long)((float)Weight_Shiwu/429.4967296 );    //ADֵת��Ϊ������g��
 return Weight_Shiwu; 
}