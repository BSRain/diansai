//数码管.c
#include <msp430f5529.h>
#include "shumaguan.h"
#define a       P2OUT &= ~ BIT0;
#define b       P2OUT &= ~ BIT2;
#define c       P7OUT &= ~ BIT4;       
#define d       P3OUT &= ~ BIT0;
#define e       P3OUT &= ~ BIT1;
#define f       P2OUT &= ~ BIT6;
#define g       P2OUT &= ~ BIT3;
#define dp      P8OUT &= ~ BIT1;
void mie()
{      
  P2OUT |= BIT0,
  P2OUT |= BIT2,
  P7OUT |= BIT4,
  P3OUT |= BIT0,
  P3OUT |= BIT1,
  P2OUT |= BIT6,
  P2OUT |= BIT3,
  P8OUT |= BIT1;
}
void shumaguan_init()
{
  //COM端持续输出高电平；
  P2DIR |= BIT5;
  P2OUT |= BIT5;
  //      初始化各端口
  P2DIR |= BIT0;//A
  P2DIR |= BIT2;//B
  P7DIR |= BIT4;//C
  P3DIR |= BIT0;//D
  P3DIR |= BIT1;//E     
  P2DIR |= BIT6;//F
  P2DIR |= BIT3;//G
  P8DIR |= BIT1;//DP

  P2OUT |= BIT0;
  P2OUT |= BIT2;
  P7OUT |= BIT4;
  P3OUT |= BIT0;
  P3OUT |= BIT1;
  P2OUT |= BIT6;
  P2OUT |= BIT3;
  P8OUT |= BIT1;


   /*  P1OUT |=BIT1;//A
       P1OUT |=BIT2;//B
       P1OUT |=BIT3;//C
       P1OUT |=BIT4;//D
       P1OUT |=BIT5;//E
       P1OUT |=BIT7;//F
       P2OUT |=BIT1;//G
       P2OUT |=BIT2;//DP
  */
}
void zero ()
{
        mie();
       a;
       b;
       c;
       d;
       e;
       f;
       //__delay_cycles(1000000);

}
void one()
{
    mie();
      b;
      c;
      //__delay_cycles(1000000);

}
void two()
{
  mie();
       a;
       b;
       d;
       e;
       g;
       dp;
       //__delay_cycles(1000000);

}
void three()
{
  mie();
       a;
       b;
       c;
       d;
       g;
       dp;
       //__delay_cycles(1000000);

}
void four()
{
  mie();
       b;
       c;
       f;
       g;
       dp;
       //__delay_cycles(1000000);

}
void five()
{
  mie();
       a;
       c;
       d;
       f;
       g;
       dp;
       //__delay_cycles(1000000);

}
void six()
{
  mie();
       c;
       d;
       e;
       f;
       g;
       dp;
       //__delay_cycles(1000000);

}
void seven()
{
  mie();
       a;
       b;
       c;
       dp;
       //__delay_cycles(1000000);

}
void eight()
{
    mie();
       a;
       b;
       c;
       d;
       e;
       f;
       g;

       //__delay_cycles(1000000);
}
void nine()
{
  mie();
       a;
       b;
       c;
       f;
       g;
       dp;
       //__delay_cycles(1000000);

}
  /*while (1)
    {
        
       int i=10;
       i--;
       switch(i){
       case 0:{
       P1OUT &= ~BIT1;//A
       P1OUT &= ~BIT2;//B
       P1OUT &= ~BIT3;//C
       P1OUT &= ~BIT4;//D
       P1OUT &= ~BIT5;//E
       P1OUT &= ~BIT7;//F
       P2OUT &= ~BIT2;//DP
       __delay_cycles(1000000);
       P1OUT |=BIT1;//A
       P1OUT |=BIT2;//B
       P1OUT |=BIT3;//C
       P1OUT |=BIT4;//D
       P1OUT |=BIT5;//E
       P1OUT |=BIT7;//F
       P2OUT |=BIT1;//G
       P2OUT |=BIT2;//DP
       }
  
       case 1:{

       P1OUT &= ~BIT2;//B
       P1OUT &= ~BIT3;//C
       __delay_cycles(1000000);
       P1OUT |=BIT1;//A
       P1OUT |=BIT2;//B
       P1OUT |=BIT3;//C
       P1OUT |=BIT4;//D
       P1OUT |=BIT5;//E
       P1OUT |=BIT7;//F
       P2OUT |=BIT1;//G
       P2OUT |=BIT2;//DP
       
       }
       case 2:{
       P1OUT &= ~BIT1;//A
       P1OUT &= ~BIT2;//B
       
       P1OUT &= ~BIT4;//D
       P1OUT &= ~BIT5;//E
      
       P2OUT &= ~BIT1;//G
       P2OUT &= ~BIT2;//DP
       __delay_cycles(1000000);
        P1OUT |=BIT1;//A
       P1OUT |=BIT2;//B
       P1OUT |=BIT3;//C
       P1OUT |=BIT4;//D
       P1OUT |=BIT5;//E
       P1OUT |=BIT7;//F
       P2OUT |=BIT1;//G
       P2OUT |=BIT2;//DP
         }
       case 3:{
       P1OUT &= ~BIT1;//A
       P1OUT &= ~BIT2;//B
       P1OUT &= ~BIT3;//C
       P1OUT &= ~BIT4;//D
       P2OUT &= ~BIT1;//G
       P2OUT &= ~BIT2;//DP
       __delay_cycles(1000000);
        P1OUT |=BIT1;//A
       P1OUT |=BIT2;//B
       P1OUT |=BIT3;//C
       P1OUT |=BIT4;//D
       P1OUT |=BIT5;//E
       P1OUT |=BIT7;//F
       P2OUT |=BIT1;//G
       P2OUT |=BIT2;//DP
         }
       case 4:{
       
       P1OUT &= ~BIT2;//B
       P1OUT &= ~BIT3;//C
       P1OUT &= ~BIT7;//F
       P2OUT &= ~BIT1;//G
       P2OUT &= ~BIT2;//DP
       __delay_cycles(1000000);
        P1OUT |=BIT1;//A
       P1OUT |=BIT2;//B
       P1OUT |=BIT3;//C
       P1OUT |=BIT4;//D
       P1OUT |=BIT5;//E
       P1OUT |=BIT7;//F
       P2OUT |=BIT1;//G
       P2OUT |=BIT2;//DP
         }
       case 5:{
       
       P1OUT &= ~BIT1;//A
       P1OUT &= ~BIT3;//C
       P1OUT &= ~BIT4;//D
       P1OUT &= ~BIT7;//F
       P2OUT &= ~BIT1;//G
       P2OUT &= ~BIT2;//DP
       __delay_cycles(1000000);
        P1OUT |=BIT1;//A
       P1OUT |=BIT2;//B
       P1OUT |=BIT3;//C
       P1OUT |=BIT4;//D
       P1OUT |=BIT5;//E
       P1OUT |=BIT7;//F
       P2OUT |=BIT1;//G
       P2OUT |=BIT2;//DP
         }
       case 6:{
       
       P1OUT &= ~BIT3;//C
       P1OUT &= ~BIT4;//D
       P1OUT &= ~BIT5;//E
       P1OUT &= ~BIT7;//F
       P2OUT &= ~BIT1;//G
       P2OUT &= ~BIT2;//DP
       __delay_cycles(1000000);
       P1OUT |=BIT1;//A
       P1OUT |=BIT2;//B
       P1OUT |=BIT3;//C
       P1OUT |=BIT4;//D
       P1OUT |=BIT5;//E
       P1OUT |=BIT7;//F
       P2OUT |=BIT1;//G
       P2OUT |=BIT2;//DP
         }
       case 7:{
       
       P1OUT &= ~BIT1;//A
       P1OUT &= ~BIT2;//B
       P1OUT &= ~BIT3;//C
       P2OUT &= ~BIT2;//DP
       __delay_cycles(1000000);
        P1OUT |=BIT1;//A
       P1OUT |=BIT2;//B
       P1OUT |=BIT3;//C
       P1OUT |=BIT4;//D
       P1OUT |=BIT5;//E
       P1OUT |=BIT7;//F
       P2OUT |=BIT1;//G
       P2OUT |=BIT2;//DP
         }
       case 8:{
       
       P1OUT &= ~BIT1;//A
       P1OUT &= ~BIT2;//B
       P1OUT &= ~BIT3;//C
       P1OUT &= ~BIT4;//D
       P1OUT &= ~BIT5;//E
       P1OUT &= ~BIT7;//F
       P2OUT &= ~BIT1;//G
       P2OUT &= ~BIT2;//DP
       __delay_cycles(1000000);
        P1OUT |=BIT1;//A
       P1OUT |=BIT2;//B
       P1OUT |=BIT3;//C
       P1OUT |=BIT4;//D
       P1OUT |=BIT5;//E
       P1OUT |=BIT7;//F
       P2OUT |=BIT1;//G
       P2OUT |=BIT2;//DP
         }
       case 9:{
       
       P1OUT &= ~BIT1;//A
       P1OUT &= ~BIT2;//B
       P1OUT &= ~BIT3;//C
       P1OUT &= ~BIT7;//F
       P2OUT &= ~BIT1;//G
       P2OUT &= ~BIT2;//DP
       __delay_cycles(1000000);
        P1OUT |=BIT1;//A
       P1OUT |=BIT2;//B
       P1OUT |=BIT3;//C
       P1OUT |=BIT4;//D
       P1OUT |=BIT5;//E
       P1OUT |=BIT7;//F
       P2OUT |=BIT1;//G
       P2OUT |=BIT2;//DP
         }
    }
    }
    }*/