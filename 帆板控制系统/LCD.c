#include "msp430g2553.h"
#define uchar unsigned char
#define uint unsigned int

#define rw_out P2DIR|=BIT0
#define rw_h P2OUT|=BIT0
#define rw_l P2OUT&=~BIT0
#define en_out P2DIR|=BIT2
#define en_h P2OUT|=BIT2
#define en_l P2OUT&=~BIT2

/*函数声明*/
void delay_1ms();
void delay_nms();
void PortConfig(void);
uint Strlen(uchar *p);
void WriteBytes(uchar *p);
void SerialWriteData(uchar send);
void SerialWriteCmd(uchar send);
void init_lcd();
void display_12864(uchar hang, uchar lie, uchar *p);
void Write_CGRAM(uchar *p,uchar cg,uchar y,uchar x);
 
void PortConfig(void)
{
/*引脚初始化*/
 P1DIR = 0xFF;
 P1SEL = 0x00;
}


uint Strlen(uchar *p)
{
/*返回字符串长度*/
uint i;
for(i=0;*(p+i);i++);
return i;
}

void WriteBytes(uchar *p)
{
uchar i,j,temp;
        en_out;
        rw_out;
        en_l;
for(j=0;j<3;j++)
{
  temp=p[j];
  for(i=0;i<8;i++)
  {
   if(temp & 0x80)
                          rw_h;
                        else rw_l;
   en_h;
   delay_nms(1);
   en_l;
   temp<<=1;
  }
}
rw_h;
}
void SerialWriteData(uchar send)
{
uchar data[3];
data[0]=0xfa ;     //1111 1010向液晶写数据
data[1]=send & 0xf0;      //取高4位
data[2]=(send<<4)&0xf0;       //取低4位
WriteBytes(data);
}
void SerialWriteCmd(uchar send)
{
uchar data[3];
data[0]=0xf8 ;                    //1111 1000向液晶写命令
data[1]=send & 0xf0;      //取高4位
data[2]=(send<<4) & 0xf0;       //取低4位
WriteBytes(data); 
}

void init_lcd()
{
 SerialWriteCmd(0x30);
 SerialWriteCmd(0x30);
 SerialWriteCmd(0x02);
 SerialWriteCmd(0x0C); //显示开，关闭游标和游标位置
 SerialWriteCmd(0x06);
 SerialWriteCmd(0x01); //清除显示，等待命令
}

void display_12864(uchar hang, uchar lie, uchar *p)
{
/*输入行,列,字符串并显示*/
// uchar i;
 switch (hang)
 {
 case 1:
 SerialWriteCmd(0x80 + lie);
 break;
 case 2:
 SerialWriteCmd(0x90 + lie);
 break;
 case 3:
 SerialWriteCmd(0x88 + lie);
 break;
 case 4:
 SerialWriteCmd(0x98 + lie);
 break;
 }
           while(*p!='\0')
          {
            SerialWriteData(*p);
                p++;
            }
}
 
void Write_CGRAM(uchar *p,uchar cg,uchar y,uchar x)
{
 /*这个是自定义字符显示*/
 /**p为自定义字符数组,大小为32*/
 /*cg为放入的CGRAM地址,共4个可选*/
 /*y为显示的行*/
 /*x为显示的列*/
 switch (cg)
 {
 case 1:
 SerialWriteCmd(0x40);
 break;
 case 2:
 SerialWriteCmd(0x50);
 break;
 case 3:
 SerialWriteCmd(0x60);
 break;
 case 4:
 SerialWriteCmd(0x70);
 break;
 }
 for (unsigned char i = 0; i <32; i++)
 {
 SerialWriteData(p[i]);
 } 
 switch (y)
 {
 case 1:
 SerialWriteCmd(0x80 + x);
 break;
 case 2:
 SerialWriteCmd(0x90 + x);
 break;
 case 3:
 SerialWriteCmd(0x88 + x);
 break;
 case 4:
 SerialWriteCmd(0x98 + x);
 break;
 }
 SerialWriteData(0x00); 
 switch (cg)
 {
 case 1:
 SerialWriteData(0x00);
 break;
 case 2:
 SerialWriteData(0x02);
 break;
 case 3:
 SerialWriteData(0x04);
 break;
 case 4:
 SerialWriteData(0x06);
 break;
 }
}
void delay_1ms(void)
{
        unsigned int i;
        for (i=0;i<1140;i++);
}

void delay_nms(unsigned int n)
{
        unsigned int i=0;
        for (i=0;i<n;i++)
                delay_1ms();
}

//void main(void)
//{
//
//  WDTCTL = WDTPW + WDTHOLD;
// 
//  while(1)
//  {
//  init_lcd();
//    display_12864(1,0,"  全国大学生  ");
//    display_12864(2,0,"电子设计竞赛  ");
//  }
//}
