#include "DS18B20.h"
#include "MSP430f5529.h"

volatile float g_fTempatureValue = 0.0;
/*******************************************
函数名称: B20_Init
功    能: 复位DS18B20
参    数: 无
返回值  : 无
********************************************/
void B20_Init(void)
{
  P1DIR |= BIT0;
  P1OUT &= ~BIT0;
  DELAY_US(600);
  P1OUT |= BIT0;
  DELAY_US(50);
  P1DIR &= ~BIT0;
  P1REN |= BIT0;
  P1OUT |= BIT0;

  while(P1IN&BIT0);
  while(!(P1IN&BIT0));

}
/*******************************************
函数名称: B20_ReadByte
功    能: 读取一个字节的数据
参    数: 无
返回值  : data--返回的一个字节数据
********************************************/
uint8_t B20_ReadByte(void)
{
 uint8_t i,data,bi;
 for(i=0;i < 8;i++)     //位计数值
 {
   P1DIR |= BIT0;
   P1OUT &= ~BIT0;
   DELAY_US(1);
   P1OUT |= BIT0;
   DELAY_US(5);
   P1DIR&=~BIT0;
   P1REN |= BIT0;
   P1OUT |= BIT0;
   bi = (P1IN&BIT0);
   data = (data>>1)|(bi<<7);
    DELAY_US(48);
 }
 return data;     //将读到的一个字节返回
}
/*******************************************
函数名称: B20_WriteByte
功    能: 写入一个字节的数据
参    数: data--要写入的数据
返回值  : 无
********************************************/
void B20_WriteByte(uint8_t data)
{
 uint8_t i;
 for(i = 0;i < 8;i++)
 {
   P1DIR |= BIT0;
   P1OUT &= ~BIT0;
   DELAY_US(1);
   if(data&0x01)
   {
     P1OUT |= BIT0;
   }
   else
   {
     P1OUT &= ~BIT0;
   }
    DELAY_US(40);
   P1OUT |= BIT0;
   DELAY_US(1);
   data >>= 1;

 }
}
/*******************************************
函数名称: B20_Read_temp
功    能: 读取温度值
参    数: 无
返回值  : 温度值(单位：摄氏度)
********************************************/
float B20_Read_temp(void)

{
 uint8_t templ,temph;
 uint16_t temp;
 float tempature;
 B20_Init();       //初始化，每次写命令都从初始化开始
 B20_WriteByte(0xcc);    //跳过ROM
 B20_WriteByte(0x44);    //启动温度转换
 B20_Init();       //初始化，每次写命令都从初始化开始
 B20_WriteByte(0xcc);    //跳过ROM
 B20_WriteByte(0xbe);    //读寄存器
 templ=B20_ReadByte();   //读温度低字节
 temph=B20_ReadByte();   //读温度高字节
 if(((uint32_t)temph*256+templ) > 63488)//(1111 1xxx xxxx xxxx)>63488，大于63488的都是负数
 {
   temph |= (1<<4);        //负温度标志位
   temp =~(temph*256+templ)+1;//取反加1
 }
 else
 {
   temph &=~(1<<4);       //正温度标志位
   temp = (temph*256+templ);
 }
 temp *= 0.625;
 tempature = temp*0.1; //数字量与温度转化
 return ((temp==0x0352) ? 0.0f : tempature);   //开机启动时为85所以改为0,返回温度值
//设置断点在调试窗口查看温度值tempature
}