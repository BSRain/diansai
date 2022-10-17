//key.c
#include "key.h"
void KEY0_GPIO_Config(void)
{
    P1DIR &=~ BIT1;
    P1REN |=  BIT1;
}
void KEY1_GPIO_Config(void)
{
    P2DIR &=~ BIT1;
    P2REN |=  BIT1;
}
int KEY0_STATE()
{
    int i;
    if(P1IN & BIT1)
    {
      for(i=0;i<12000;i++); // 延时消抖
      if(P1IN & BIT1)
      {
          return 0;
      }
    }
    return 1;
}

int KEY1_STATE()
{
    int i;
    if(P2IN & BIT1)
    {
      for(i=0;i<12000;i++); // 延时消抖
      if(P2IN & BIT1)
      {
          return 0;
      }
    }
    return 1;
}