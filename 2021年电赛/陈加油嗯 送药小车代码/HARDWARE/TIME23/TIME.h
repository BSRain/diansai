#ifndef __TIME_H
#define __TIME_H	
#include "string.h"
#include "sys.h"
#include "usart.h"
#include "mpu6050.h"
#include "OLED.h"

extern float Pitch,Roll,Yaw;

void TIM1_Getsample_Int(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
	
void Encoder_Init_TIM2(u16 arr,u16 psc);
int Read_Encoder_TIM2(void);
void Encoder_Init_TIM4(u16 arr,u16 psc);
int Read_Encoder_TIM4(void);

#endif
