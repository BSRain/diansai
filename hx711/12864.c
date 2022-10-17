#include "msp430.h"
typedef unsigned char uchar;
typedef unsigned int  uint;

#define cyDDR P6DIR|=0X78
#define cyPORT P6OUT|=0X78//3 4 5 6ê?3?
#define CS1 P6OUT|=BIT3//????D?o?
#define CS0 P6OUT&=~BIT3
#define SID1 P6OUT|=BIT4//êy?YD?o?
#define SID0 P6OUT&=~BIT4
#define SCLK1 P6OUT|=BIT5//ê±?óD?o?
#define SCLK0 P6OUT&=~BIT5
#define CH0 P6OUT&=~BIT6//′?DD????D?o?15
//延时
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
/*************D′?üá?******************/
void Write_Com(uchar com)//?Write?one?byte?command?to?LCD12864
{
  CS1;
  sendbyte(0xf8);//D′??á?
  Delay_Nus(600);//èy??×??ú????ò??¨òaóD×?1?μ??óê±￡?·??òò×3???ê±Dò?êìa
                 //??êμ?á±?μ?ò2??óDì?′ó?êìa
  sendbyte(com & 0xf0);
  Delay_Nus(200);
  sendbyte((com<<4)&0xf0);
  Delay_Nus(2);
}
/***********D′′??úêy?Y********************/
void Write_Data(uchar dat)//Write one byte data to LCD12864
{
  sendbyte(0xfa);//D′êy?Y??á?
  Delay_Nus(600);
  sendbyte(dat&0xf0);//D′êy?Yμ???????
  Delay_Nus(200);
  sendbyte((dat<<4)&0xf0);//êy?Yμí????
  Delay_Nus(2);
}
/******************************/
void SET_XY(uchar x, uchar y)
{
  uchar Add =0;
  if(x==1)//μúò?DD
  {
    Add=0x80+y;}
  else if(x==2)//μú?tDD
  {
    Add =0x90+y;
  }
  else if (x==3)//μúèyDD
  {
    Add=0x88+y;
  }
  else if(x==4)//μú??DD
  {
    Add=0x98+y;
  }
  Write_Com(Add);
  }
/************?òòo?§?3????D′1??×??úêy?Y************/
void WriteOneChar(uchar x,uchar y,uchar dat)
{
  SET_XY(x,y);
  Write_Data(dat);
}
/*************??ê?×?·?′?****************/
void Write_String(uchar x, uchar y,uchar *pt)
{
  SET_XY(x,y);
  while (*pt!='\0')
  {
    Write_Data(*pt++);
  }
}
/*************??ê?oo×?3ìDò***************/
void WriteChineseCharacters(uchar x,uchar y,uchar *pt, uchar n)
{
  uchar i;
  SET_XY(x,y);
  for(i=0;i<(n*2);i++)
  {
    Write_Data(*pt++);
  }
}
/************显示一个四位数**********/
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
/**********显示五位数******************/
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
/**********3?ê??ˉòo?§*****************/
void LCD12864_Init(void)
{
  cyDDR;//?àó|μ??????úéè???aê?3?
  Delay_Nms(100);//?óê±μè′yòo?§íê3é?′??
  Write_Com(0x30);//1|?üéè??:ò?′??í8??êy?Y,?ù±???á??ˉ???
  Delay_Nus(50);
  Write_Com(0x02);//DDRAMμ??·1é??
  Delay_Nus(50);
  Write_Com(0x0C);//??ê?éè?¨:?a??ê?,2???ê?1a±ê,2?×?μ±?°??ê???·′°×éá?ˉ
  Delay_Nus(50);
  Write_Com(0x01);//???á￡???DDRAMμ????·??êy?÷μ÷???a?°00H?±
  Delay_Nus(50);
  Write_Com(0x06);//1|?üéè??￡?μ?éè?¨:??ê?×?·?/1a±ê′ó×óμ?óòò???,DDRAMμ??·?ó1
  Delay_Nus(50);
}
/*void main()
{
  WDTCTL=WDTPW+WDTHOLD;
  LCD12864_Init();
  _NOP();
  CH0;//′?DD
  Write_Number(1,1,2016);//??ê?ò???????êy
  WriteChineseCharacters(1,3,"?ê",1);
  Write_String(1,4,"08??04è?");
  WriteOneChar(2,1,'a');//D′ò???×??úμ?êy?Y
  WriteChineseCharacters(3,0,"1t1t1t1t",4);
  Write_String(4,1,"LiuJunyu");//??ê?×?·?′?
}*/