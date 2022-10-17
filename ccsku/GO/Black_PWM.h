/*
 * Black_PWM.h
 *
 *  Created on: 2020年10月8日
 *      Author: PC
 */

#ifndef GO_BLACK_PWM_H_
#define GO_BLACK_PWM_H_

#include <msp430.h>

#define PWMPeriod  1500 //PWM周期
void PWM0_Config(void);
void PWM1_Config(void);
void PWM2_Config(void);
void PWM3_Config(void);
int PWM0_State(unsigned int n);
int PWM1_State(unsigned int n);
int PWM2_State(unsigned int n);
int PWM3_State(unsigned int n);

#endif /* GO_BLACK_PWM_H_ */
