#include "MSP430G2553.h"
void main()
{
  WDTCTL = WDTPW + WDTHOLD;\
    //COM端持续输出高电平；
  P2DIR |= BIT5;
  P2OUT |= BIT5;
  
  //      初始化各端口
P1DIR |= BIT1;//A
P1DIR |= BIT2;//B
P1DIR |= BIT3;//C
P1DIR |= BIT4;//D
P1DIR |= BIT5;//E
P1DIR |= BIT7;//F
P2DIR |= BIT1;//G
P2DIR |= BIT2;//DP

P1OUT |= BIT1;
P1OUT |= BIT2;
P2OUT |= BIT3;
P1OUT |= BIT4;
P1OUT |= BIT5;
P1OUT |= BIT7;
P2OUT |= BIT1;
P2OUT |= BIT2;


      /* P1OUT |=IT1;//A
       P1OUT |=BIT2;//B
       P1OUT |=BIT3;//C
       P1OUT |=BIT4;//D
       P1OUT |=BIT5;//E
       P1OUT |=BIT7;//F
       P2OUT |=BIT1;//G
       P2OUT |=IT2;//DP
*/


  while (1)
    {
       int i=-1;
       i++;
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
    }