//cryfucns.h
#include <stdio.h>
//typedef unsigned int uint;
//typedef unsigned char uchar;

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
 
void Send(unsigned char type, unsigned char transdata);
void Ini_Lcd(void);
void Clear_GDRAM(void);
void Disp_HZ(unsigned char addr, const unsigned char* pt, unsigned char num);
void Draw_PM(const unsigned char* ptr);
void Draw_TX(unsigned char Yaddr, unsigned char Xaddr, const unsigned char* dp);
void Disp_SZ(unsigned char addr, unsigned char shuzi);
void Send(uchar type, uchar transdata);
 
 
void ShowIMG(const uchar* ptr, uchar h, uchar w);
void Str2IN1(char a1, char a2);
void LCD_ShowChar(char x, char y, char ch1, char ch2);
void LCD_ShowStr(char x, char y, char* ch);
void LCD_ClearPic(void);                                //ͼƬ����
void LCD_ShowCharCH(char x, char y, char* CH);
void LCD_ShowStrCH(char x, char y, char* ch, char num);
 
 
/*************************
 1  GND
 2  VCC
 3  �Աȶ�
 4  CSƬѡ
 5  SID������������
 6  CLK����ʱ��
 15 PSB(Lѡ�񴮿�ģʽ��
 17 RST��λ
 19 A����+5V
 20 K����0V
 *************************/