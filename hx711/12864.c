#include "msp430.h"
typedef unsigned char uchar;
typedef unsigned int  uint;

#define cyDDR P6DIR|=0X78
#define cyPORT P6OUT|=0X78//3 4 5 6��?3?
#define CS1 P6OUT|=BIT3//????D?o?
#define CS0 P6OUT&=~BIT3
#define SID1 P6OUT|=BIT4//��y?YD?o?
#define SID0 P6OUT&=~BIT4
#define SCLK1 P6OUT|=BIT5//����?��D?o?
#define SCLK0 P6OUT&=~BIT5
#define CH0 P6OUT&=~BIT6//��?DD????D?o?15
//��ʱ
void Delay_Nus(uint t)
{
  while(t--)
  {
    _NOP();
  }
}
//
void Delay_1ms(void)
{
  uint i;
  for(i=1000;i>0;i--)
  {
    _NOP();
  }
}
//
void Delay_Nms(uint t)
{
  while(t--)
  {
    Delay_1ms();
  }
}
/************************/
void sendbyte(unsigned char zdata)
{
  unsigned int i;
  for(i=0;i<8;i++)
  {
    if((zdata<<i)&0x80)
    {
      SID1;
    }
    else
    {
      SID0;
    }
    SCLK1;
      _NOP();
      SCLK0;
  }
}
/*************D��?����?******************/
void Write_Com(uchar com)//?Write?one?byte?command?to?LCD12864
{
  CS1;
  sendbyte(0xf8);//D��??��?
  Delay_Nus(600);//��y??��??��????��??����a��D��?1?��??��������?��??������3???����D��?����a
                 //??����?����?��?��2??��D��?�䨮?����a
  sendbyte(com & 0xf0);
  Delay_Nus(200);
  sendbyte((com<<4)&0xf0);
  Delay_Nus(2);
}
/***********D���??����y?Y********************/
void Write_Data(uchar dat)//Write one byte data to LCD12864
{
  sendbyte(0xfa);//D�䨺y?Y??��?
  Delay_Nus(600);
  sendbyte(dat&0xf0);//D�䨺y?Y��???????
  Delay_Nus(200);
  sendbyte((dat<<4)&0xf0);//��y?Y�̨�????
  Delay_Nus(2);
}
/******************************/
void SET_XY(uchar x, uchar y)
{
  uchar Add =0;
  if(x==1)//�̨���?DD
  {
    Add=0x80+y;}
  else if(x==2)//�̨�?tDD
  {
    Add =0x90+y;
  }
  else if (x==3)//�̨���yDD
  {
    Add=0x88+y;
  }
  else if(x==4)//�̨�??DD
  {
    Add=0x98+y;
  }
  Write_Com(Add);
  }
/************?����o?��?3????D��1??��??����y?Y************/
void WriteOneChar(uchar x,uchar y,uchar dat)
{
  SET_XY(x,y);
  Write_Data(dat);
}
/*************??��?��?��?��?****************/
void Write_String(uchar x, uchar y,uchar *pt)
{
  SET_XY(x,y);
  while (*pt!='\0')
  {
    Write_Data(*pt++);
  }
}
/*************??��?oo��?3��D��***************/
void WriteChineseCharacters(uchar x,uchar y,uchar *pt, uchar n)
{
  uchar i;
  SET_XY(x,y);
  for(i=0;i<(n*2);i++)
  {
    Write_Data(*pt++);
  }
}
/************��ʾһ����λ��**********/
void Write_Number(uchar x,uchar y,uint Num)
{
  uchar temp[4];
  temp[0] =Num/1000;
  temp[1]=Num%1000/100;
  temp[2]=Num%100/10;
  temp[3]=Num%10;
  SET_XY(x,y);
  Write_Data('0'+temp[0]);
  Write_Data('0'+temp[1]);
  Write_Data('0'+temp[2]);
  Write_Data('0'+temp[3]);
}
/**********��ʾ��λ��******************/
void Write_5Number(uchar x,uchar y,uint Num)
{
  uchar temp[5];
  temp[0]=Num/10000;
  temp[1] =Num%10000/1000;
  temp[2]=Num%1000/100;
  temp[3]=Num%100/10;
  temp[4]=Num%10;
  SET_XY(x,y);
  Write_Data('0'+temp[0]);
  Write_Data('0'+temp[1]);
  Write_Data('0'+temp[2]);
  Write_Data('0'+temp[3]);
  Write_Data('0'+temp[4]);
}
/**********3?��??����o?��*****************/
void LCD12864_Init(void)
{
  cyDDR;//?����|��??????������???a��?3?
  Delay_Nms(100);//?�������̨���y��o?�쨪��3��?��??
  Write_Com(0x30);//1|?������??:��?��??��8??��y?Y,?����???��??��???
  Delay_Nus(50);
  Write_Com(0x02);//DDRAM��??��1��??
  Delay_Nus(50);
  Write_Com(0x0C);//??��?����?��:?a??��?,2???��?1a����,2?��?�̡�?��??��???����������?��
  Delay_Nus(50);
  Write_Com(0x01);//???����???DDRAM��????��??��y?�¦̡�???a?��00H?��
  Delay_Nus(50);
  Write_Com(0x06);//1|?������??��?��?����?��:??��?��?��?/1a�����䨮������?������???,DDRAM��??��?��1
  Delay_Nus(50);
}
/*void main()
{
  WDTCTL=WDTPW+WDTHOLD;
  LCD12864_Init();
  _NOP();
  CH0;//��?DD
  Write_Number(1,1,2016);//??��?��???????��y
  WriteChineseCharacters(1,3,"?��",1);
  Write_String(1,4,"08??04��?");
  WriteOneChar(2,1,'a');//D�䨰???��??����?��y?Y
  WriteChineseCharacters(3,0,"1t1t1t1t",4);
  Write_String(4,1,"LiuJunyu");//??��?��?��?��?
}*/