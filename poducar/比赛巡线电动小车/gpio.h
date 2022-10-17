#ifndef __gpio
#define __gpio
/*************************************************/
/*     ����io�� P6.0~P6.7  ����   ���ܣ����     */
/*************************************************/
void GPIO6_Pin_0(unsigned int Pi)//����P6.0��
{
  #define Gpio6_Pin_0 P6OUT=(P6OUT&0Xfe)|Pi//1111 1110
  Gpio6_Pin_0;
}

void GPIO6_Pin_1(unsigned int Pi)//����P6.1��
{
  #define Gpio6_Pin_1 P6OUT=(P6OUT&0Xfd)|(Pi<<=1)//1111 1101
  Gpio6_Pin_1;
}

void GPIO6_Pin_2(unsigned int Pi)//����P6.2��
{
  #define Gpio6_Pin_2 P6OUT=(P6OUT&0Xfb)|(Pi<<=2)//1111 1011
  Gpio6_Pin_2;
}

void GPIO6_Pin_3(unsigned int Pi)//����P6.3��
{
  #define Gpio6_Pin_3 P6OUT=(P6OUT&0Xf7)|(Pi<<=3)//1111 0111
  Gpio6_Pin_3;                                   
}

void GPIO6_Pin_4(unsigned int Pi)//����P6.4��
{
  #define Gpio6_Pin_4 P6OUT=(P6OUT&0Xef)|(Pi<<=4)//1110 1111
  Gpio6_Pin_4;
}

void GPIO6_Pin_5(unsigned int Pi)//����P6.5��
{
  #define Gpio6_Pin_5 P6OUT=(P6OUT&0Xdf)|(Pi<<=5)//1101 1111
  Gpio6_Pin_5;
}

void GPIO6_Pin_6(unsigned int Pi)//����P6.6��
{
  #define Gpio6_Pin_6 P6OUT=(P6OUT&0Xbf)|(Pi<<=6)//1011 1111
  Gpio6_Pin_6;
}

void GPIO6_Pin_7(unsigned int Pi)//����P6.7��
{
  #define Gpio6_Pin_7 P6OUT=(P6OUT&0X7f)|(Pi<<=7)//0111 1111
  Gpio6_Pin_7;
}

/*************************************************/
/*     ����io�� P5.0~P5.7  ����   ���ܣ����     */
/*************************************************/
void GPIO5_Pin_0(unsigned int Pi)//����P5.0��
{
  #define Gpio5_Pin_0 P5OUT=(P5OUT&0Xfe)|Pi//1111 1110
  Gpio5_Pin_0;
}

void GPIO5_Pin_1(unsigned int Pi)//����P5.1��
{
  #define Gpio5_Pin_1 P5OUT=(P5OUT&0Xfd)|(Pi<<=1)//1111 1101
  Gpio5_Pin_1;
}

void GPIO5_Pin_2(unsigned int Pi)//����P5.2��
{
  #define Gpio5_Pin_2 P5OUT=(P5OUT&0Xfb)|(Pi<<=2)//1111 1011
  Gpio5_Pin_2;
}

void GPIO5_Pin_3(unsigned int Pi)//����P5.3��
{
  #define Gpio5_Pin_3 P5OUT=(P5OUT&0Xf7)|(Pi<<=3)//1111 0111
  Gpio5_Pin_3;                                   
}

void GPIO5_Pin_4(unsigned int Pi)//����P5.4��
{
  #define Gpio5_Pin_4 P5OUT=(P5OUT&0Xef)|(Pi<<=4)//1110 1111
  Gpio5_Pin_4;
}

void GPIO5_Pin_5(unsigned int Pi)//����P5.5��
{
  #define Gpio5_Pin_5 P5OUT=(P5OUT&0Xdf)|(Pi<<=5)//1101 1111
  Gpio5_Pin_5;
}

void GPIO5_Pin_6(unsigned int Pi)//����P5.6��
{
  #define Gpio5_Pin_6 P5OUT=(P5OUT&0Xbf)|(Pi<<=6)//1011 1111
  Gpio5_Pin_6;
}

void GPIO5_Pin_7(unsigned int Pi)//����P5.7��
{
  #define Gpio5_Pin_7 P5OUT=(P5OUT&0X7f)|(Pi<<=7)//0111 1111
  Gpio5_Pin_7;
}

/*************************************************/
/*     ����io�� P4.0~P4.7  ����   ���ܣ����     */
/*************************************************/
void GPIO4_Pin_0(unsigned int Pi)//����P4.0��
{
  #define Gpio4_Pin_0 P4OUT=(P4OUT&0Xfe)|Pi//1111 1110
  Gpio4_Pin_0;
}

void GPIO4_Pin_1(unsigned int Pi)//����P4.1��
{
  #define Gpio4_Pin_1 P4OUT=(P4OUT&0Xfd)|(Pi<<=1)//1111 1101
  Gpio4_Pin_1;
}

void GPIO4_Pin_2(unsigned int Pi)//����P4.2��
{
  #define Gpio4_Pin_2 P4OUT=(P4OUT&0Xfb)|(Pi<<=2)//1111 1011
  Gpio4_Pin_2;
}

void GPIO4_Pin_3(unsigned int Pi)//����P4.3��
{
  #define Gpio4_Pin_3 P4OUT=(P4OUT&0Xf7)|(Pi<<=3)//1111 0111
  Gpio4_Pin_3;                                   
}

void GPIO4_Pin_4(unsigned int Pi)//����P4.4��
{
  #define Gpio4_Pin_4 P4OUT=(P4OUT&0Xef)|(Pi<<=4)//1110 1111
  Gpio4_Pin_4;
}

void GPIO4_Pin_5(unsigned int Pi)//����P4.5��
{
  #define Gpio4_Pin_5 P4OUT=(P4OUT&0Xdf)|(Pi<<=5)//1101 1111
  Gpio4_Pin_5;
}

void GPIO4_Pin_6(unsigned int Pi)//����P4.6��
{
  #define Gpio4_Pin_6 P4OUT=(P4OUT&0Xbf)|(Pi<<=6)//1011 1111
  Gpio4_Pin_6;
}

void GPIO4_Pin_7(unsigned int Pi)//����P4.7��
{
  #define Gpio4_Pin_7 P4OUT=(P4OUT&0X7f)|(Pi<<=7)//0111 1111
  Gpio4_Pin_7;
}

/*************************************************/
/*     ����io�� P3.0~P3.7  ����   ���ܣ����     */
/*************************************************/
void GPIO3_Pin_0(unsigned int Pi)//����P3.0��
{
  #define Gpio3_Pin_0 P3OUT=(P3OUT&0Xfe)|Pi//1111 1110
  Gpio3_Pin_0;
}

void GPIO3_Pin_1(unsigned int Pi)//����P3.1��
{
  #define Gpio3_Pin_1 P3OUT=(P3OUT&0Xfd)|(Pi<<=1)//1111 1101
  Gpio3_Pin_1;
}

void GPIO3_Pin_2(unsigned int Pi)//����P3.2��
{
  #define Gpio3_Pin_2 P3OUT=(P3OUT&0Xfb)|(Pi<<=2)//1111 1011
  Gpio3_Pin_2;
}

void GPIO3_Pin_3(unsigned int Pi)//����P3.3��
{
  #define Gpio3_Pin_3 P3OUT=(P3OUT&0Xf7)|(Pi<<=3)//1111 0111
  Gpio3_Pin_3;                                   
}

void GPIO3_Pin_4(unsigned int Pi)//����P3.4��
{
  #define Gpio3_Pin_4 P3OUT=(P3OUT&0Xef)|(Pi<<=4)//1110 1111
  Gpio3_Pin_4;
}

void GPIO3_Pin_5(unsigned int Pi)//����P3.5��
{
  #define Gpio3_Pin_5 P3OUT=(P3OUT&0Xdf)|(Pi<<=5)//1101 1111
  Gpio3_Pin_5;
}

void GPIO3_Pin_6(unsigned int Pi)//����P3.6��
{
  #define Gpio3_Pin_6 P3OUT=(P3OUT&0Xbf)|(Pi<<=6)//1011 1111
  Gpio3_Pin_6;
}

void GPIO3_Pin_7(unsigned int Pi)//����P3.7��
{
  #define Gpio3_Pin_7 P3OUT=(P3OUT&0X7f)|(Pi<<=7)//0111 1111
  Gpio3_Pin_7;
}

/*************************************************/
/*     ����io�� P2.0~P2.7  ����   ���ܣ����     */
/*************************************************/
void GPIO2_Pin_0(unsigned int Pi)//����P2.0��
{
  #define Gpio2_Pin_0 P2OUT=(P2OUT&0Xfe)|Pi//1111 1110
  Gpio2_Pin_0;
}

void GPIO2_Pin_1(unsigned int Pi)//����P2.1��
{
  #define Gpio2_Pin_1 P2OUT=(P2OUT&0Xfd)|(Pi<<=1)//1111 1101
  Gpio2_Pin_1;
}

void GPIO2_Pin_2(unsigned int Pi)//����P2.2��
{
  #define Gpio2_Pin_2 P2OUT=(P2OUT&0Xfb)|(Pi<<=2)//1111 1011
  Gpio2_Pin_2;
}

void GPIO2_Pin_3(unsigned int Pi)//����P2.3��
{
  #define Gpio2_Pin_3 P2OUT=(P2OUT&0Xf7)|(Pi<<=3)//1111 0111
  Gpio2_Pin_3;                                   
}

void GPIO2_Pin_4(unsigned int Pi)//����P2.4��
{
  #define Gpio2_Pin_4 P2OUT=(P2OUT&0Xef)|(Pi<<=4)//1110 1111
  Gpio2_Pin_4;
}

void GPIO2_Pin_5(unsigned int Pi)//����P2.5��
{
  #define Gpio2_Pin_5 P2OUT=(P2OUT&0Xdf)|(Pi<<=5)//1101 1111
  Gpio2_Pin_5;
}

void GPIO2_Pin_6(unsigned int Pi)//����P2.6��
{
  #define Gpio2_Pin_6 P2OUT=(P2OUT&0Xbf)|(Pi<<=6)//1011 1111
  Gpio2_Pin_6;
}

void GPIO2_Pin_7(unsigned int Pi)//����P2.7��
{
  #define Gpio2_Pin_7 P2OUT=(P2OUT&0X7f)|(Pi<<=7)//0111 1111
  Gpio2_Pin_7;
}

/*************************************************/
/*     ����io�� P1.0~P1.7  ����   ���ܣ����     */
/*************************************************/
void GPIO1_Pin_0(unsigned int Pi)//����P1.0��
{
  #define Gpio1_Pin_0 P6OUT=(P1OUT&0Xfe)|Pi//1111 1110
  Gpio1_Pin_0;
}

void GPIO1_Pin_1(unsigned int Pi)//����P1.1��
{
  #define Gpio1_Pin_1 P1OUT=(P1OUT&0Xfd)|(Pi<<=1)//1111 1101
  Gpio1_Pin_1;
}

void GPIO1_Pin_2(unsigned int Pi)//����P1.2��
{
  #define Gpio1_Pin_2 P1OUT=(P1OUT&0Xfb)|(Pi<<=2)//1111 1011
  Gpio1_Pin_2;
}

void GPIO1_Pin_3(unsigned int Pi)//����P1.3��
{
  #define Gpio1_Pin_3 P1OUT=(P1OUT&0Xf7)|(Pi<<=3)//1111 0111
  Gpio1_Pin_3;                                   
}

void GPIO1_Pin_4(unsigned int Pi)//����P1.4��
{
  #define Gpio1_Pin_4 P1OUT=(P1OUT&0Xef)|(Pi<<=4)//1110 1111
  Gpio1_Pin_4;
}

void GPIO1_Pin_5(unsigned int Pi)//����P1.5��
{
  #define Gpio1_Pin_5 P1OUT=(P1OUT&0Xdf)|(Pi<<=5)//1101 1111
  Gpio1_Pin_5;
}

void GPIO1_Pin_6(unsigned int Pi)//����P1.6��
{
  #define Gpio1_Pin_6 P1OUT=(P1OUT&0Xbf)|(Pi<<=6)//1011 1111
  Gpio1_Pin_6;
}

void GPIO1_Pin_7(unsigned int Pi)//����P1.7��
{
  #define Gpio1_Pin_7 P1OUT=(P1OUT&0X7f)|(Pi<<=7)//0111 1111
  Gpio1_Pin_7;
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


/****************************************************************************/
/*    ����io�ڶ�����   ��0 ��1   Ĭ��:��1    P1.0~P1.7                      */
/****************************************************************************/
#define Read_GPIO1_Pin_0 (P1IN&0X01)//0000 0001   ��P1.0��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO1_Pin_1 (P1IN&0X02)//0000 0010   ��P1.1��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO1_Pin_2 (P1IN&0X04)//0000 0100   ��P1.2��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO1_Pin_3 (P1IN&0X08)//0000 1000   ��P1.3��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO1_Pin_4 (P1IN&0X10)//0001 0000   ��P1.4��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO1_Pin_5 (P1IN&0X20)//0010 0000   ��P1.5��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO1_Pin_6 (P1IN&0X40)//0100 0000   ��P1.6��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO1_Pin_7 (P1IN&0X80)//1000 0000   ��P1.7��ֵ �궨��  Ĭ�ϸߵ�ƽ


/****************************************************************************/
/*    ����io�ڶ�����   ��0 ��1   Ĭ��:��1    P2.0~P2.7                      */
/****************************************************************************/
#define Read_GPIO2_Pin_0 (P2IN&0X01)//0000 0001   ��P2.0��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO2_Pin_1 (P2IN&0X02)//0000 0010   ��P2.1��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO2_Pin_2 (P2IN&0X04)//0000 0100   ��P2.2��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO2_Pin_3 (P2IN&0X08)//0000 1000   ��P2.3��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO2_Pin_4 (P2IN&0X10)//0001 0000   ��P2.4��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO2_Pin_5 (P2IN&0X20)//0010 0000   ��P2.5��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO2_Pin_6 (P2IN&0X40)//0100 0000   ��P2.6��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO2_Pin_7 (P2IN&0X80)//1000 0000   ��P2.7��ֵ �궨��  Ĭ�ϸߵ�ƽ


/****************************************************************************/
/*    ����io�ڶ�����   ��0 ��1   Ĭ��:��1    P3.0~P3.7                      */
/****************************************************************************/
#define Read_GPIO3_Pin_0 (P3IN&0X01)//0000 0001   ��P3.0��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO3_Pin_1 (P3IN&0X02)//0000 0010   ��P3.1��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO3_Pin_2 (P3IN&0X04)//0000 0100   ��P3.2��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO3_Pin_3 (P3IN&0X08)//0000 1000   ��P3.3��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO3_Pin_4 (P3IN&0X10)//0001 0000   ��P3.4��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO3_Pin_5 (P3IN&0X20)//0010 0000   ��P3.5��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO3_Pin_6 (P3IN&0X40)//0100 0000   ��P3.6��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO3_Pin_7 (P3IN&0X80)//1000 0000   ��P3.7��ֵ �궨��  Ĭ�ϸߵ�ƽ


/****************************************************************************/
/*    ����io�ڶ�����   ��0 ��1   Ĭ��:��1    P4.0~P4.7                      */
/****************************************************************************/
#define Read_GPIO4_Pin_0 (P4IN&0X01)//0000 0001   ��P4.0��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO4_Pin_1 (P4IN&0X02)//0000 0010   ��P4.1��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO4_Pin_2 (P4IN&0X04)//0000 0100   ��P4.2��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO4_Pin_3 (P4IN&0X08)//0000 1000   ��P4.3��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO4_Pin_4 (P4IN&0X10)//0001 0000   ��P4.4��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO4_Pin_5 (P4IN&0X20)//0010 0000   ��P4.5��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO4_Pin_6 (P4IN&0X40)//0100 0000   ��P4.6��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO4_Pin_7 (P4IN&0X80)//1000 0000   ��P4.7��ֵ �궨��  Ĭ�ϸߵ�ƽ


/****************************************************************************/
/*    ����io�ڶ�����   ��0 ��1   Ĭ��:��1    P5.0~P5.7                      */
/****************************************************************************/
#define Read_GPIO5_Pin_0 (P5IN&0X01)//0000 0001   ��P5.0��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO5_Pin_1 (P5IN&0X02)//0000 0010   ��P5.1��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO5_Pin_2 (P5IN&0X04)//0000 0100   ��P5.2��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO5_Pin_3 (P5IN&0X08)//0000 1000   ��P5.3��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO5_Pin_4 (P5IN&0X10)//0001 0000   ��P5.4��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO5_Pin_5 (P5IN&0X20)//0010 0000   ��P5.5��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO5_Pin_6 (P5IN&0X40)//0100 0000   ��P5.6��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO5_Pin_7 (P5IN&0X80)//1000 0000   ��P5.7��ֵ �궨��  Ĭ�ϸߵ�ƽ


/****************************************************************************/
/*    ����io�ڶ�����   ��0 ��1   Ĭ��:��1    P6.0~P6.7                      */
/****************************************************************************/
#define Read_GPIO6_Pin_0 (P6IN&0X01)//0000 0001   ��P6.0��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO6_Pin_1 (P6IN&0X02)//0000 0010   ��P6.1��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO6_Pin_2 (P6IN&0X04)//0000 0100   ��P6.2��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO6_Pin_3 (P6IN&0X08)//0000 1000   ��P6.3��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO6_Pin_4 (P6IN&0X10)//0001 0000   ��P6.4��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO6_Pin_5 (P6IN&0X20)//0010 0000   ��P6.5��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO6_Pin_6 (P6IN&0X40)//0100 0000   ��P6.6��ֵ �궨��  Ĭ�ϸߵ�ƽ
#define Read_GPIO6_Pin_7 (P6IN&0X80)//1000 0000   ��P6.7��ֵ �궨��  Ĭ�ϸߵ�ƽ
#endif