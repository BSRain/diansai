#include "DS18B20.h"
#include "MSP430f5529.h"

volatile float g_fTempatureValue = 0.0;
/*******************************************
��������: B20_Init
��    ��: ��λDS18B20
��    ��: ��
����ֵ  : ��
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
��������: B20_ReadByte
��    ��: ��ȡһ���ֽڵ�����
��    ��: ��
����ֵ  : data--���ص�һ���ֽ�����
********************************************/
uint8_t B20_ReadByte(void)
{
 uint8_t i,data,bi;
 for(i=0;i < 8;i++)     //λ����ֵ
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
 return data;     //��������һ���ֽڷ���
}
/*******************************************
��������: B20_WriteByte
��    ��: д��һ���ֽڵ�����
��    ��: data--Ҫд�������
����ֵ  : ��
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
��������: B20_Read_temp
��    ��: ��ȡ�¶�ֵ
��    ��: ��
����ֵ  : �¶�ֵ(��λ�����϶�)
********************************************/
float B20_Read_temp(void)

{
 uint8_t templ,temph;
 uint16_t temp;
 float tempature;
 B20_Init();       //��ʼ����ÿ��д����ӳ�ʼ����ʼ
 B20_WriteByte(0xcc);    //����ROM
 B20_WriteByte(0x44);    //�����¶�ת��
 B20_Init();       //��ʼ����ÿ��д����ӳ�ʼ����ʼ
 B20_WriteByte(0xcc);    //����ROM
 B20_WriteByte(0xbe);    //���Ĵ���
 templ=B20_ReadByte();   //���¶ȵ��ֽ�
 temph=B20_ReadByte();   //���¶ȸ��ֽ�
 if(((uint32_t)temph*256+templ) > 63488)//(1111 1xxx xxxx xxxx)>63488������63488�Ķ��Ǹ���
 {
   temph |= (1<<4);        //���¶ȱ�־λ
   temp =~(temph*256+templ)+1;//ȡ����1
 }
 else
 {
   temph &=~(1<<4);       //���¶ȱ�־λ
   temp = (temph*256+templ);
 }
 temp *= 0.625;
 tempature = temp*0.1; //���������¶�ת��
 return ((temp==0x0352) ? 0.0f : tempature);   //��������ʱΪ85���Ը�Ϊ0,�����¶�ֵ
//���öϵ��ڵ��Դ��ڲ鿴�¶�ֵtempature
}