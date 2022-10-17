#include "HX711.h"
#include <msp430f5529.h>
//#include <stdio.h>
long HX711_Buffer = 0;
long Weight_Maopi = 0;    // 上电时毛皮质量
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

//延时函数
void delayus(uint t)
{
//_nop_();
//_nop_();
  while(t--);
}
// I/O配置
void PORT_Init(void)
{
  P1SEL &=~ BIT2;
  P1DIR &=~ BIT2;//SDA
  P2DIR |= BIT2;//SCK
}
//p2.2 SCK
//p1.2 DT
//读取HX711
unsigned long HX711_Read(void) //增益128
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


unsigned int get_pizhong() //获取皮重，秤盘重量
{
  uint j;
  // ulong hx711_dat;
  for(j=0;j<5;j++)
  delayus(4);
  hx711_dat=HX711_Read(); //HX711AD转换数据处理
  temp=(uint)(hx711_dat/429.4967296); //缩放long数据为int型，方便处理
  
  //temp=temp-19672;
  //temp=temp/0.897;
  
  return (temp);
}


long HX711_Get_Weight()
{
 HX711_Buffer = HX711_Read();    ///读取此时的传感器输出值
 Weight_Shiwu = HX711_Buffer;     ///将传感器的输出值储存
 Weight_Shiwu = Weight_Shiwu - Weight_Maopi; //获取实物的AD采样数值。
 Weight_Shiwu = (long)((float)Weight_Shiwu/429.4967296 );    //AD值转换为重量（g）
 return Weight_Shiwu; 
}