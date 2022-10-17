/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author
  * @version V1.0
  * @date    2019-08-04
  * @brief   led应用函数接口
  ******************************************************************************
  */

#include "Black_DELAY.h"
#define CPU_F ((double)1035000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 
void delay_nms(int n)
{
  for(int i=0;i<n;i++)
    delay_ms(1);
}
void delay_nus(int n)
{
  for(int i=0;i<n;i++)
    delay_us(1);
}
void delay(int ms)        //延时函数
{
    int i,j;
    for(i=0;i<ms;i++)
    for(j=0;j<240;j++);
}
