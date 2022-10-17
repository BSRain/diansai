/*
 * Black_PWM.c
 *
 *  Created on: 2020��10��8��
 *      Author: PC
 */

#include "Black_PWM.h"

void PWM0_Config(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // �رտ��Ź�
    P1DIR |=BIT2;              // ���� P1.3Ϊ���
    P1SEL |=BIT2;              // ���� P1.3ΪTA0.2���
    TA0CCR0 = PWMPeriod;           // ����PWM ����
    TA0CCTL1 = OUTMOD_7;           // ����PWM ���ģʽΪ��7 - PWM��λ/��λģʽ���������ƽ��TAR��ֵ����CCR2ʱ��λΪ0����TAR��ֵ����CCR0ʱ��λΪ1���ı�CCR2���Ӷ�����PWM����ʵģʽ2Ҳ����
    TA0CTL= TASSEL_2 +MC_1;    // ����TIMERA��ʱ��ԴΪSMCLK, ����ģʽΪup,��CCR0���Զ���0��ʼ����
    TA0CCR1=0;
}
int PWM0_State(unsigned int n)
{
    unsigned int v = 0;
    v = (n*PWMPeriod)/100;
    TA0CCR1=v;
    return 0;
}
void PWM1_Config(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // �رտ��Ź�
    P1DIR |=BIT3;              // ���� P1.3Ϊ���
    P1SEL |=BIT3;              // ���� P1.3ΪTA0.2���
    TA0CCR0 = PWMPeriod;           // ����PWM ����
    TA0CCTL2 = OUTMOD_7;           // ����PWM ���ģʽΪ��7 - PWM��λ/��λģʽ���������ƽ��TAR��ֵ����CCR2ʱ��λΪ0����TAR��ֵ����CCR0ʱ��λΪ1���ı�CCR2���Ӷ�����PWM����ʵģʽ2Ҳ����
    TA0CTL= TASSEL_2 +MC_1;    // ����TIMERA��ʱ��ԴΪSMCLK, ����ģʽΪup,��CCR0���Զ���0��ʼ����
    //TA0CCR2=0;
}
int PWM1_State(unsigned int n)
{
    unsigned int v;
    v = (n*PWMPeriod)/100;
    TA0CCR2=v;
    return 0;
}
void PWM2_Config(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // �رտ��Ź�
    P1DIR |=BIT4;              // ���� P1.3Ϊ���
    P1SEL |=BIT4;              // ���� P1.3ΪTA0.2���
    TA0CCR0 = PWMPeriod;           // ����PWM ����
    TA0CCTL3 = OUTMOD_7;           // ����PWM ���ģʽΪ��7 - PWM��λ/��λģʽ���������ƽ��TAR��ֵ����CCR2ʱ��λΪ0����TAR��ֵ����CCR0ʱ��λΪ1���ı�CCR2���Ӷ�����PWM����ʵģʽ2Ҳ����
    TA0CTL= TASSEL_2 +MC_1;    // ����TIMERA��ʱ��ԴΪSMCLK, ����ģʽΪup,��CCR0���Զ���0��ʼ����
    TA0CCR3=0;
}
int PWM2_State(unsigned int n)
{
    unsigned int v = 0;
    v = (n*PWMPeriod)/100;
    TA0CCR3=v;
    return 0;
}
void PWM3_Config(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // �رտ��Ź�
    P1DIR |=BIT5;              // ���� P1.3Ϊ���
    P1SEL |=BIT5;              // ���� P1.3ΪTA0.2���
    TA0CCR0 = PWMPeriod;           // ����PWM ����
    TA0CCTL4 = OUTMOD_7;           // ����PWM ���ģʽΪ��7 - PWM��λ/��λģʽ���������ƽ��TAR��ֵ����CCR2ʱ��λΪ0����TAR��ֵ����CCR0ʱ��λΪ1���ı�CCR2���Ӷ�����PWM����ʵģʽ2Ҳ����
    TA0CTL= TASSEL_2 +MC_1;    // ����TIMERA��ʱ��ԴΪSMCLK, ����ģʽΪup,��CCR0���Զ���0��ʼ����
    TA0CCR4=0;
}
int PWM3_State(unsigned int n)
{
    unsigned int v = 0;
    v = (n*PWMPeriod)/100;
    TA0CCR4=v;
    return 0;
}