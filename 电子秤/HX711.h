#ifndef HX711_H_
#define HX711_H_
#include "msp430.h"
#include "stdint.h"
//#define SCK_L       P1OUT   &= ~BIT0
//#define SCK_H       P1OUT   |= BIT0
//#define SDA_IN      (P1IN & BIT1)
//#define GapValue    116.18
typedef void HX711;

  
    // 一些变量
   

    // 功能函数
    void init(uint8_t SCK, uint8_t DT, int Value);
    unsigned long HX711_Read(void);
    long Get_Weight(void);

    
    // 两个io端口




#endif 
