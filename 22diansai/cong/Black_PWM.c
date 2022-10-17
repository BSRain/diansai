/*
 * Black_PWM.c
 *
 *  Created on: 2020年10月8日
 *      Author: PC
 */

#include "Black_PWM.h"

void PWM0_Config(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // 关闭看门狗
    P2DIR |=BIT0;              // 设置 P1.2为输出
    P2SEL |=BIT0;              // 设置 P1.2为TA0.2输出
    TA1CCR0 = PWMPeriod;           // 设置PWM 周期
    TA1CCTL1 = OUTMOD_7;           // 设置PWM 输出模式为：7 - PWM复位/置位模式，即输出电平在TAR的值等于CCR2时复位为0，当TAR的值等于CCR0时置位为1，改变CCR2，从而产生PWM。其实模式2也可以
    TA1CTL= TASSEL_2 +MC_1+TACLR;    // 设置TIMERA的时钟源为SMCLK, 计数模式为up,到CCR0再自动从0开始计数
    TA1CCR1=0;
}
int  PWM0_State(unsigned int n)
{
    unsigned int v = 0;
    v = (int)(PWMPeriod*(n/100.0));
    TA1CCR1=v;
    return 0;
}
void PWM1_Config(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // 关闭看门狗
    P1DIR |=BIT2;              // 设置 P1.3为输出
    P1SEL |=BIT2;              // 设置 P1.3为TA0.2输出
    TA0CCR0 = PWMPeriod;           // 设置PWM 周期
    TA0CCTL1 = OUTMOD_7;           // 设置PWM 输出模式为：7 - PWM复位/置位模式，即输出电平在TAR的值等于CCR2时复位为0，当TAR的值等于CCR0时置位为1，改变CCR2，从而产生PWM。其实模式2也可以
    TA0CTL= TASSEL_2 +MC_1+TACLR;    // 设置TIMERA的时钟源为SMCLK, 计数模式为up,到CCR0再自动从0开始计数
    TA0CCR1=0;
}
//从机的1.3口穿了
int PWM1_State(unsigned int n)
{
    unsigned int v;
    v = (int)(PWMPeriod*(n/100.0));
    TA0CCR1=v;
    return 0;
}
void PWM2_Config(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // 关闭看门狗
    P1DIR |=BIT4;              // 设置 P1.4为输出
    P1SEL |=BIT4;              // 设置 P1.4为TA0.2输出
    TA0CCR0 = PWMPeriod;           // 设置PWM 周期
    TA0CCTL3 = OUTMOD_7;           // 设置PWM 输出模式为：7 - PWM复位/置位模式，即输出电平在TAR的值等于CCR2时复位为0，当TAR的值等于CCR0时置位为1，改变CCR2，从而产生PWM。其实模式2也可以
    TA0CTL= TASSEL_2 +MC_1+TACLR;    // 设置TIMERA的时钟源为SMCLK, 计数模式为up,到CCR0再自动从0开始计数
    TA0CCR3=0;
}
int PWM2_State(unsigned int n)
{
    unsigned int v = 0;
    v = (int)(PWMPeriod*(n/100.0));
    TA0CCR3=v;
    return 0;
}
void PWM3_Config(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // 关闭看门狗
    P1DIR |=BIT5;              // 设置 P1.3为输出
    P1SEL |=BIT5;              // 设置 P1.3为TA0.2输出
    TA0CCR0 = PWMPeriod;           // 设置PWM 周期
    TA0CCTL4 = OUTMOD_7;           // 设置PWM 输出模式为：7 - PWM复位/置位模式，即输出电平在TAR的值等于CCR2时复位为0，当TAR的值等于CCR0时置位为1，改变CCR2，从而产生PWM。其实模式2也可以
    TA0CTL= TASSEL_2 +MC_1+TACLR;    // 设置TIMERA的时钟源为SMCLK, 计数模式为up,到CCR0再自动从0开始计数
    TA0CCR4=0;
}
int PWM3_State(unsigned int n)
{
    unsigned int v = 0;
    v = (int)(PWMPeriod*(n/100.0));
    TA0CCR4=v;
    return 0;
}

