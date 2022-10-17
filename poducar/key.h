//key.h
#ifndef KEY_H_
#define KEY_H_

#include <msp430.h>

void KEY0_GPIO_Config(void);
void KEY1_GPIO_Config(void);
int KEY0_STATE();
int KEY1_STATE();
#endif /* KEY_H_ */