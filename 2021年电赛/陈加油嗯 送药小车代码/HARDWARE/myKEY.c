/*
key1-2:PA2、PA3
*/
#include <stm32f10x.h>
#include "Delay.h"

extern int mode;

#define KEY2_UP GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==1
#define KEY2_DOWN GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==0

#define KEY1_UP GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==1
#define KEY1_DOWN GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0


void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void KEY_number(void)
{
	if(KEY1_DOWN)
	{
		delay_ms(15);
		while(KEY1_DOWN);//当key为放开时，执行下面的语句
		delay_ms(15);
		mode++;
		if(mode==4) mode=1;
	}
	
	if(KEY2_DOWN)
	{
		delay_ms(15);
		while(KEY2_DOWN);//当key为放开时，执行下面的语句
		delay_ms(15);
		mode--;
		if(mode==0) mode=3;
	}
}
