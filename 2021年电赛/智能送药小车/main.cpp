#include "MSP430G2553.h"
#include "stdint.h"
/*��������*/
void PWMout1();
void PWMout2();
void time();

P1DIR |= BIT1;//RED
P1DIR |= BIT2;//GREEN

void time()
{
      /*��ʼ��P1.6Ϊ����*/
    P1DIR &= ~BIT6;
    /*����ʱ��ԴΪSMCLK*/
    TA1CTL |= TASSEL1;
    /*���ù���ģʽΪUp&Down*/
    TA1CTL |= MC0|MC1;
  
}
void PWMout1(int A)
{

    /*����TA1CCR0������*/
    TA1CCR0 = 5000;
    /*����TA1CCR2ռ�ձ�*/
    TA1CCR2 = A;//ռ�ձ�(TACCR0 - TACCR2) / TACCR0,Ƶ��=SMCLK/(TACCR0+1)/2
    /*����Ϊ�Ƚ�ģʽ*/
    TA1CCTL0 &= ~CAP;
    TA1CCTL2 &= ~CAP;
    /*���ñȽ����ģʽ*/
    TA1CCTL2 |= OUTMOD_6;
    /*����IO����*/
    P2SEL |= BIT1;
    P2DIR |= BIT1;
    
    /*�߼���ƽ���Ƶ������ת*/
    P1DIR |=  BIT3 + BIT4;
    P1OUT |=  BIT3;
    P1OUT &= ~BIT4;
}
void PWMout2(int B)
{
    
    /*����TA1CCR0Ϊ0x00FF*/
    TA1CCR0 = 5000;
    /*����TA1CCR2Ϊ0x00FF*/
    TA1CCR1 = B;//ռ�ձ�(TACCR0 - TACCR2) / TACCR0,Ƶ��=SMCLK/(TACCR0+1)/2
    /*����Ϊ�Ƚ�ģʽ*/
    TA1CCTL0 &= ~CAP;
    TA1CCTL1 &= ~CAP;
    /*���ñȽ����ģʽ*/
    TA1CCTL1 |= OUTMOD_6;

        /*����IO����*/
    P2SEL |= BIT5;
    P2DIR |= BIT5;
    
     /*�߼���ƽ���Ƶ������ת*/
    P2DIR |= BIT3 + BIT4;
    P2OUT |= BIT3;
    P2OUT &= ~BIT4;
  
}

void move(void)
{
    P1OUT |=  BIT3;
    P1OUT &= ~BIT4;
    P2OUT |= BIT3;
    P2OUT &= ~BIT4;
}
void turn_left(void)
{
    PWMout1(3000);
    PWMout2(3000);
    P1OUT &= ~BIT3;
    P1OUT |=BIT4;
    __delay_cycles(400000);
}
void turn_right(void)
{
    PWMout1(3000);
    PWMout2(3000);
    P2OUT &= ~BIT3;
    P2OUT |=BIT4;
    __delay_cycles(400000);
}

void main( void )
{
   // Stop watchdog timer to prevent time out reset
       WDTCTL = WDTPW + WDTHOLD;
    /*����DCOƵ��Ϊ1MHz*/
    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
    time();
    PWMout1(2000);
    PWMout2(2000);
    __delay_cycles(1500000);
    turn_right();
    PWMout1(2000);
    PWMout2(2000);
    move();
    __delay_cycles(800000);
    PWMout1(5000);
    PWMout2(5000);
    P1OUT |= BIT1;
    __delay_cycles(1000000);
    
    PWMout1(2000);
    PWMout2(2000);
    turn_left();
    __delay_cycles(800000);
    PWMout1(3000);
    PWMout2(3000);
    P2OUT &= ~BIT3;
    P2OUT |=BIT4;
    __delay_cycles(400000);
    P2OUT |=BIT3;
    P2OUT &= ~BIT4;
    PWMout1(2000);
    PWMout2(2000);
    __delay_cycles(1000000);
    
    __delay_cycles(2000000);
    
    PWMout1(2000);
    PWMout2(2000);
    P1OUT &= ~BIT3;
    P2OUT &= ~BIT3;
    P1OUT |= BIT4;
    P2OUT |= BIT4;
    __delay_cycles(1000000);
    
    PWMout1(3000);
    PWMout2(3000);
    P2OUT |= BIT3;
    P2OUT &= ~BIT4;
    __delay_cycles(400000);
    P2OUT |= BIT4;
    P2OUT &= ~BIT3;
     PWMout1(2000);
    PWMout2(2000);
    __delay_cycles(1000000);
    PWMout1(5000);
    PWMout2(5000);
    P1OUT |= BIT2;
    
}

