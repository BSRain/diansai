/*
 * Black_USART.h
 *
 *  Created on: 2020年10月8日
 *      Author: PC
 */

#ifndef GO_BLACK_USART_H_
#define GO_BLACK_USART_H_

#include <msp430.h>

void USART0_Config(void);
void USART1_Config(void);
void send0_buf(unsigned char *ptr);    //发送字符串
void send1_buf(unsigned char *ptr);    //发送字符串

#endif /* GO_BLACK_USART_H_ */
