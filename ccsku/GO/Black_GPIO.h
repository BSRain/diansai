/*
 * Black_GPIO.h
 *
 *  Created on: 2020Äê10ÔÂ7ÈÕ
 *      Author: PC
 */

#ifndef GO_BLACK_GPIO_H_
#define GO_BLACK_GPIO_H_

#include <msp430.h>

void OUTPUT_GPIO_Config(unsigned int x,unsigned int y);
void OUTPUT_GPIO_State(unsigned int x,unsigned int y,unsigned int z);
void INPUT_GPIO_Config(unsigned int x,unsigned int y);

#endif /* GO_BLACK_GPIO_H_ */
