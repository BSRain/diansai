#include "MSP430G2553.h"
#include "stdint.h"
/*函数声明*/
void PWMout1();
void PWMout2();
void time();

P1DIR |= BIT1;//RED
P1DIR |= BIT2;//GREEN

void time()
{
      /*初始化P1.6为输入*/
    P1DIR &= ~BIT6;
    /*设置时钟源为SMCLK*/
    TA1CTL |= TASSEL1;
    /*设置工作模式为Up&Down*/
    TA1CTL |= MC0|MC1;
  
}
void PWMout1(int A)
{

    /*设置TA1CCR0的周期*/
    TA1CCR0 = 5000;
    /*设置TA1CCR2占空比*/
    TA1CCR2 = A;//占空比(TACCR0 - TACCR2) / TACCR0,频率=SMCLK/(TACCR0+1)/2
    /*设置为比较模式*/
    TA1CCTL0 &= ~CAP;
    TA1CCTL2 &= ~CAP;
    /*设置比较输出模式*/
    TA1CCTL2 |= OUTMOD_6;
    /*设置IO复用*/
    P2SEL |= BIT1;
    P2DIR |= BIT1;
    
    /*逻辑电平控制电机正反转*/
    P1DIR |=  BIT3 + BIT4;
    P1OUT |=  BIT3;
    P1OUT &= ~BIT4;
}
void PWMout2(int B)
{
    
    /*设置TA1CCR0为0x00FF*/
    TA1CCR0 = 5000;
    /*设置TA1CCR2为0x00FF*/
    TA1CCR1 = B;//占空比(TACCR0 - TACCR2) / TACCR0,频率=SMCLK/(TACCR0+1)/2
    /*设置为比较模式*/
    TA1CCTL0 &= ~CAP;
    TA1CCTL1 &= ~CAP;
    /*设置比较输出模式*/
    TA1CCTL1 |= OUTMOD_6;

        /*设置IO复用*/
    P2SEL |= BIT5;
    P2DIR |= BIT5;
    
     /*逻辑电平控制电机正反转*/
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
    /*配置DCO频率为1MHz*/
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

