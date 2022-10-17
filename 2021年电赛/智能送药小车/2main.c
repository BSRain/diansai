#include "MSP430G2553.h"
#include "stdint.h"
/*��������*/
void PWMout1();
void PWMout2();
void time();

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
void xundao()
{  if(!(P1IN & BIT0)==1)
   { 
     turn_right();
   }
   else
   {
      move();
   }
   
   
   if(!(P1IN & BIT1)==1)
   {
     turn_left();
   }
   else
   {
     move();
   }
   if(P1IN & BIT1 &&P1IN & BIT0 && P1IN & BIT6 && P1IN & BIT1   )
   {P1OUT &= ~BIT3;
    P1OUT &= ~BIT6;
    P1OUT &= ~BIT7;
    P1OUT &= ~BIT4;
    while(1){};
   }

}

void main( void )
{
   // Stop watchdog timer to prevent time out reset
       WDTCTL = WDTPW + WDTHOLD;
   __bis_SR_register(GIE);
    time();
    PWMout1(2000);
    PWMout2(2000);
    xundao();
}
#pragma vector = PORT2_VECTOR
__interrupt void Port1(void)
{
if(!(P1IN & BIT0)==1)
   { 
     __delay_cycles(10000);
     turn_right();
   }
   else
   {
      move();
   }
   
   
   if(!(P1IN & BIT1)==1)
   {
     __delay_cycles(10000);
     turn_left();
   }
   else
   {
     move();
   }
   if((P1IN & BIT1) && (P1IN & BIT0) && (P1IN & BIT6) && (P1IN & BIT1) )
   {P1OUT &= ~BIT3;
    P1OUT &= ~BIT4;
    P2OUT &= ~BIT3;
    P2OUT &= ~BIT4;
   }


}
  
