#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED0 PAout(8)	
#define LED1 PDout(2)

void LED_Init(void);//��ʼ��
void Beep_Init(void);
void Key_Init(void);
	 				    
#endif
