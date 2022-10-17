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
/***********************ϵͳ��ʱ*******************/
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
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//������
    unsigned unum;//���Ҫת���������ľ���ֵ,ת�������������Ǹ���
    int i=0,j,k;//i����ָʾ�����ַ�����Ӧλ��ת��֮��i��ʵ�����ַ����ĳ��ȣ�ת����˳��������ģ��������������k����ָʾ����˳��Ŀ�ʼλ��;j����ָʾ����˳��ʱ�Ľ�����
 
    //��ȡҪת���������ľ���ֵ
    if(radix==10&&num<0)//Ҫת����ʮ�����������Ǹ���
    {
        unum=(unsigned)-num;//��num�ľ���ֵ����unum
        str[i++]='-';//���ַ�����ǰ������Ϊ'-'�ţ�����������1
    }
    else unum=(unsigned)num;//����numΪ����ֱ�Ӹ�ֵ��unum
 
    //ת�����֣�ע��ת�����������
    do
    {
        str[i++]=index[unum%(unsigned)radix];//ȡunum�����һλ��������Ϊstr��Ӧλ��ָʾ������1
        unum/=radix;//unumȥ�����һλ
 
    }while(unum);//ֱ��unumΪ0�˳�ѭ��
 
    str[i]='\0';//���ַ���������'\0'�ַ���c�����ַ�����'\0'������
 
    //��˳���������
    if(str[0]=='-') k=1;//����Ǹ��������Ų��õ������ӷ��ź��濪ʼ����
    else k=0;//���Ǹ�����ȫ����Ҫ����
 
    char temp;//��ʱ��������������ֵʱ�õ�
    for(j=k;j<=(i-1)/2;j++)//ͷβһһ�Գƽ�����i��ʵ�����ַ����ĳ��ȣ��������ֵ�ȳ�����1
    {
        temp=str[j];//ͷ����ֵ����ʱ����
        str[j]=str[i-1+k-j];//β����ֵ��ͷ��
        str[i-1+k-j]=temp;//����ʱ������ֵ(��ʵ����֮ǰ��ͷ��ֵ)����β��
    }
 
    return str;//����ת������ַ���
 
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
  
  HC_SR04Init();			//��ʼ������ģ��HC_SR04
  TimeGetInit();			//��ʼ�������������ţ�ģʽ��
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
        Disp_HZ(List0,"����Ϊ",0);
        Disp_SZ(List0+3,szDst[0]);
        Disp_SZ(List0+4,szDst[1]);
        Disp_SZ(List0+5,szDst[2]);
        Disp_SZ(List0+6,szDst[3]);

        Disp_HZ(List1,"����Ϊ",0);
        Disp_SZ(List1+3,danjia[0]);
        Disp_SZ(List1+4,danjia[1]);
        Disp_SZ(List1+5,danjia[2]);
        
        Disp_HZ(List2,"�ܼ�Ϊ",0);
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
            Disp_HZ(List0,"����Ϊ",0);
            Disp_SZ(List0+3,szDst[0]);
            Disp_SZ(List0+4,szDst[1]);
            Disp_SZ(List0+5,szDst[2]);
            Disp_SZ(List0+6,szDst[3]);

            Disp_HZ(List1,"����Ϊ",0);
            Disp_SZ(List1+3,danjia[0]);
            Disp_SZ(List1+4,danjia[1]);
            Disp_SZ(List1+5,danjia[2]);
            
            Disp_HZ(List2,"�ܼ�Ϊ",0);
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

