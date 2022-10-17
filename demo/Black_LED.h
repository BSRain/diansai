/*
 * Black_LED.h
 *
 *  Created on: 2020Äê10ÔÂ7ÈÕ
 *      Author: PC
 */


#ifndef GO_BLACK_LED_H_
#define GO_BLACK_LED_H_

#include <msp430.h>

void LED0_GPIO_Config(void);
void LED1_GPIO_Config(void);
void LED0_STATE(unsigned int n);
void LED1_STATE(unsigned int n);

#endif /* GO_BLACK_LED_H_ */
