//shumaguan2.c
#include <msp430f5529.h>
#include "shumaguan2.h"
#define A       P2OUT &= ~ BIT4;
#define B       P1OUT &= ~ BIT5;
#define c2      P1OUT &= ~ BIT4;       
#define D       P6OUT &= ~ BIT4;
#define E       P1OUT &= ~ BIT2;
#define F       P4OUT &= ~ BIT3;
#define G       P4OUT &= ~ BIT0;
#define DP      P3OUT &= ~ BIT7;
void mie2()
{ 
  P2OUT |= BIT4,
  P1OUT |= BIT5,
  P1OUT |= BIT4,
  P6OUT |= BIT4,
  P1OUT |= BIT2,
  P4OUT |= BIT3,
  P4OUT |= BIT0,
  P3OUT |= BIT7;
}
void shumaguan_init2()
{
  //COM端持续输出高电平；
  P8DIR |= BIT2;
  P8OUT |= BIT2;
  //      初始化各端口
  P2DIR |= BIT4;//A
  P1DIR |= BIT5;//B
  P1DIR |= BIT4;//C
  P6DIR |= BIT4;//D
  P1DIR |= BIT2;//E     
  P4DIR |= BIT3;//F
  P4DIR |= BIT0;//G
  P3DIR |= BIT7;//DP

  P2OUT |= BIT4,
  P1OUT |= BIT5,
  P1OUT |= BIT4,
  P6OUT |= BIT4,
  P1OUT |= BIT2,
  P4OUT |= BIT3,
  P4OUT |= BIT0,
  P3OUT |= BIT7;


   /*  
       1OUT |=BIT1;//A
       P1OUT |=BIT2;//B
       P6OUT |=BIT4;//C
       P1OUT |=BIT4;//D
       P1OUT |=BIT5;//E
       P1OUT |=BIT7;//F
       P2OUT |=BIT1;//G
       P2OUT |=BIT2;//DP
  */
}
void zero2 ()
{
        mie2();
       A;
       B;
       c2;
       D;
       E;
       F;
       //__delay_cycles(1000000);

}
void one2()
{
      mie2();
      B;
      c2;
      //__delay_cycles(1000000);

}
void two2()
{
    mie2();
       A;
       B;
       D;
       E;
       G;
       DP;
       //__delay_cycles(1000000);

}
void three2()
{
    mie2();
       A;
       B;
       c2;
       D;
       G;
       DP;
       //__delay_cycles(1000000);

}
void four2()
{
    mie2();
       B;
       c2;
       F;
       G;
       DP;
       //__delay_cycles(1000000);

}
void five2()
{
    mie2();
       A;
       c2;
       D;
       F;
       G;
       DP;
       //__delay_cycles(1000000);

}
void six2()
{
    mie2();
       c2;
       D;
       E;
       F;
       G;
       DP;
       //__delay_cycles(1000000);

}
void seven2()
{
    mie2();
       A;
       B;
       c2;
       DP;
       //__delay_cycles(1000000);

}
void eight2()
{
    mie2();
       A;
       B;
       c2;
       D;
       E;
       F;
       G;
        DP;
       //__delay_cycles(1000000);
}
void nine2()
{
    mie2();
       A;
       B;
       c2;
       F;
       G;
       DP;
       //__delay_cycles(1000000);

}