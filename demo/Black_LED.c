/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author
  * @version V1.0
  * @date    2019-08-04
  * @brief   led应用函数接口
  ******************************************************************************
  */

#include "Black_LED.h"

void LED0_GPIO_Config(void)
{
     P1DIR|=BIT0;             //P1.0-P1.5方向为输出，BITX的定义在msp430.h
     P1OUT&=~BIT0;
}

void LED1_GPIO_Config(void)
{
     P4DIR|=BIT7;             //P1.0-P1.5方向为输出，BITX的定义在msp430.h
     P4OUT&=~BIT7;
}

void LED0_STATE(unsigned int n)
{
    switch(n)
    {
      case 2:P1OUT ^= BIT0;break;
      case 1:P1OUT |= BIT0;break;
      case 0:P1OUT &=~BIT0;break;
      default:P1OUT&=~BIT0;break;
    }
}

void LED1_STATE(unsigned int n)
{
    switch(n)
    {
      case 2:P4OUT ^= BIT7;break;
      case 1:P4OUT |= BIT7;break;
      case 0:P4OUT &=~BIT7;break;
      default:P4OUT&=~BIT7;break;
    }
}



/*********************************************END OF FILE**********************/
