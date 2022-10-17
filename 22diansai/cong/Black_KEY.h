/*
 * Black_KEY.h
 *
 *  Created on: 2020Äê10ÔÂ8ÈÕ
 *      Author: PC
 */

#ifndef GO_BLACK_KEY_H_
#define GO_BLACK_KEY_H_

#include <msp430.h>

void KEY0_GPIO_Config(void);
void KEY1_GPIO_Config(void);
int KEY0_STATE();
int KEY1_STATE();
#endif /* GO_BLACK_KEY_H_ */
