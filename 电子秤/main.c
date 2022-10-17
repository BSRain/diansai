#include "msp430.h"
#include "stdint.h"
#include "HX711.h"
#include "cryfucns.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <cstdio>
#include "keys.h"
#include "HC-sr04.c"
/***********************系统延时*******************/
#define CPU_F ((double)1035000)
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
 
#define List0   0x80
#define List1   0x90
#define List2   0x88
#define List3   0x98

double  shu = 0;

int zhenshu=0;
int zongshu=0;
int zhegebi;
int diergebi;
int disangebi;

char* itoa(int num,char* str,int radix)
{
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//索引表
    unsigned unum;//存放要转换的整数的绝对值,转换的整数可能是负数
    int i=0,j,k;//i用来指示设置字符串相应位，转换之后i其实就是字符串的长度；转换后顺序是逆序的，有正负的情况，k用来指示调整顺序的开始位置;j用来指示调整顺序时的交换。
 
    //获取要转换的整数的绝对值
    if(radix==10&&num<0)//要转换成十进制数并且是负数
    {
        unum=(unsigned)-num;//将num的绝对值赋给unum
        str[i++]='-';//在字符串最前面设置为'-'号，并且索引加1
    }
    else unum=(unsigned)num;//若是num为正，直接赋值给unum
 
    //转换部分，注意转换后是逆序的
    do
    {
        str[i++]=index[unum%(unsigned)radix];//取unum的最后一位，并设置为str对应位，指示索引加1
        unum/=radix;//unum去掉最后一位
 
    }while(unum);//直至unum为0退出循环
 
    str[i]='\0';//在字符串最后添加'\0'字符，c语言字符串以'\0'结束。
 
    //将顺序调整过来
    if(str[0]=='-') k=1;//如果是负数，符号不用调整，从符号后面开始调整
    else k=0;//不是负数，全部都要调整
 
    char temp;//临时变量，交换两个值时用到
    for(j=k;j<=(i-1)/2;j++)//头尾一一对称交换，i其实就是字符串的长度，索引最大值比长度少1
    {
        temp=str[j];//头部赋值给临时变量
        str[j]=str[i-1+k-j];//尾部赋值给头部
        str[i-1+k-j]=temp;//将临时变量的值(其实就是之前的头部值)赋给尾部
    }
 
    return str;//返回转换后的字符串
 
}

void PORT_Init(void);
long HX711_Get_Weight();
void keyboard_init();
unsigned int get_pizhong();
char scanKey();


int main()
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  HC_SR04Init();			//初始化超声模块HC_SR04
  TimeGetInit();			//初始化计数捕获引脚，模式等
  _EINT();
  Ini_Lcd();
  PORT_Init();
  char szDst[10] = "0";
  char zongjia[10] = "0";
  char danjia[10]="0";
  keyboard_init();
  char cc;
  int dan=0;
  int temp=0;
  __enable_interrupt ();
  int zero=19666;
  memset(danjia,'\0',sizeof(danjia));
  memset(szDst,'\0',sizeof(szDst));
  memset(zongjia,'\0',sizeof(zongjia));
  while(1)
  { 
 
        shu=HX711_Get_Weight()-zero;
        shu=shu/0.897;
        zhenshu=(int)shu-temp;
        itoa(zhenshu,szDst,10);
        itoa(zongshu,zongjia,10);
        Disp_HZ(List0,"重量为",0);
        Disp_SZ(List0+3,szDst[0]);
        Disp_SZ(List0+4,szDst[1]);
        Disp_SZ(List0+5,szDst[2]);
        Disp_SZ(List0+6,szDst[3]);

        Disp_HZ(List1,"单价为",0);
        Disp_SZ(List1+3,danjia[0]);
        Disp_SZ(List1+4,danjia[1]);
        Disp_SZ(List1+5,danjia[2]);
        
        Disp_HZ(List2,"总价为",0);
        Disp_SZ(List2+3,zongjia[0]);
        Disp_SZ(List2+4,zongjia[1]);
        Disp_SZ(List2+5,zongjia[2]);
        Disp_SZ(List2+6,zongjia[3]);
        
        
        if(cc=scanKey())
        {
          if(cc=='A')
          {
            //delay_ms(30000);
            //Send(0, 0x01);
            //LCD_ClearPic();
            memset(szDst,'\0',sizeof(szDst));
            memset(zongjia,'\0',sizeof(zongjia));
          }
          else if(cc=='B')
          {
            dan=0;
            memset(danjia,'\0',sizeof(danjia));
            memset(szDst,'\0',sizeof(szDst));
            memset(zongjia,'\0',sizeof(zongjia));
          }
          else if(cc=='C')
          {
            shu=HX711_Get_Weight()-zero;
            shu=shu/0.897;
            zhenshu=(int)shu;
            itoa(zhenshu,szDst,10);
            temp=zhenshu;
            Disp_HZ(List0,"重量为",0);
            Disp_SZ(List0+3,szDst[0]);
            Disp_SZ(List0+4,szDst[1]);
            Disp_SZ(List0+5,szDst[2]);
            Disp_SZ(List0+6,szDst[3]);

            Disp_HZ(List1,"单价为",0);
            Disp_SZ(List1+3,danjia[0]);
            Disp_SZ(List1+4,danjia[1]);
            Disp_SZ(List1+5,danjia[2]);
            
            Disp_HZ(List2,"总价为",0);
            Disp_SZ(List2+3,zongjia[0]);
            Disp_SZ(List2+4,zongjia[1]);
            Disp_SZ(List2+5,zongjia[2]); 
            memset(szDst,'\0',sizeof(szDst));
          }
          else if(cc=='D')
          {
            temp=0;
            memset(szDst,'\0',sizeof(szDst));
          }
          else if(cc=='#')
          {
            zhegebi=danjia[0]-48;
            diergebi=danjia[1]-48;
            disangebi=danjia[2]-48;
            zongshu=zhegebi*zhenshu*100+diergebi*zhenshu*10+disangebi*zhenshu;
          }
          else if(cc=='*')
          {
            zongshu=0;
            memset(zongjia,'\0',sizeof(zongjia));
          }
          else
          {
            danjia[dan]=scanKey();
            dan++;
          }
        }
        
      delay_ms(1000);

        
  }
}

