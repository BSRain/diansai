/*
 * Black_PWM.h
 *
 *  Created on: 2020年10月8日
 *      Author: PC
 */

#ifndef GO_BLACK_PWM_H_
#define GO_BLACK_PWM_H_

#include <msp430.h>


#define PWMPeriod  2000 //PWM周期 
void PWM0_Config(void);//2.0
void PWM1_Config(void);//1.3
void PWM2_Config(void);//1.4
void PWM3_Config(void);//1.5
int PWM0_State(unsigned int n);
int PWM1_State(unsigned int n);
int PWM2_State(unsigned int n);
int PWM3_State(unsigned int n);


#endif /* GO_BLACK_PWM_H_ */
