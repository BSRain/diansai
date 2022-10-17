/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author
  * @version V1.0
  * @date    2019-08-04
  * @brief   led应用函数接口
  ******************************************************************************
  */

#include "Black_GPIO.h"

void OUTPUT_GPIO_Config(unsigned int x,unsigned int y)
{
    if( x == 1 )
    {
    switch(y)
     {
      case 0:
          P1DIR |= BIT0;
          P1OUT |= BIT0;
          P1OUT &=~BIT0;
      break;
      case 1:
          P1DIR |= BIT1;
          P1OUT |= BIT1;
          P1OUT &=~BIT1;
      break;
      case 2:
          P1DIR |= BIT2;
          P1OUT |= BIT2;
          P1OUT &=~BIT2;
      break;
      case 3:
          P1DIR |= BIT3;
          P1OUT |= BIT3;
          P1OUT &=~BIT3;
      break;
      case 4:
          P1DIR |= BIT4;
          P1OUT |= BIT4;
          P1OUT &=~BIT4;
      break;
      case 5:
          P1DIR |= BIT5;
          P1OUT |= BIT5;
          P1OUT &=~BIT5;
      break;
      case 6:
          P1DIR |= BIT6;
          P1OUT |= BIT6;
          P1OUT &=~BIT6;
      break;
      case 7:
          P1DIR |= BIT7;
          P1OUT |= BIT7;
          P1OUT &=~BIT7;
      break;
      default:
          P1DIR = 0xff;
          P1OUT = 0x00;
      break;
     }
    }
    else if( x == 2 )
    {
        switch(y)
         {
          case 0:
              P2DIR |= BIT0;
              P2OUT |= BIT0;
              P2OUT &=~BIT0;
          break;
          case 1:
              P2DIR |= BIT1;
              P2OUT |= BIT1;
              P2OUT &=~BIT1;
          break;
          case 2:
              P2DIR |= BIT2;
              P2OUT |= BIT2;
              P2OUT &=~BIT2;
          break;
          case 3:
              P2DIR |= BIT3;
              P2OUT |= BIT3;
              P2OUT &=~BIT3;
          break;
          case 4:
              P2DIR |= BIT4;
              P2OUT |= BIT4;
              P2OUT &=~BIT4;
          break;
          case 5:
              P2DIR |= BIT5;
              P2OUT |= BIT5;
              P2OUT &=~BIT5;
          break;
          case 6:
              P2DIR |= BIT6;
              P2OUT |= BIT6;
              P2OUT &=~BIT6;
          break;
          case 7:
              P2DIR |= BIT7;
              P2OUT |= BIT7;
              P2OUT &=~BIT7;
          break;
          default:
              P2DIR = 0xff;
              P2OUT = 0x00;
          break;
         }
     }
     else if( x == 3)
        {
            switch(y)
             {
              case 0:
                  P3DIR |= BIT0;
                  P3OUT |= BIT0;
                  P3OUT &=~BIT0;
              break;
              case 1:
                  P3DIR |= BIT1;
                  P3OUT |= BIT1;
                  P3OUT &=~BIT1;
              break;
              case 2:
                  P3DIR |= BIT2;
                  P3OUT |= BIT2;
                  P3OUT &=~BIT2;
              break;
              case 3:
                  P3DIR |= BIT3;
                  P3OUT |= BIT3;
                  P3OUT &=~BIT3;
              break;
              case 4:
                  P3DIR |= BIT4;
                  P3OUT |= BIT4;
                  P3OUT &=~BIT4;
              break;
              case 5:
                  P3DIR |= BIT5;
                  P3OUT |= BIT5;
                  P3OUT &=~BIT5;
              break;
              case 6:
                  P3DIR |= BIT6;
                  P3OUT |= BIT6;
                  P3OUT &=~BIT6;
              break;
              case 7:
                  P3DIR |= BIT7;
                  P3OUT |= BIT7;
                  P3OUT &=~BIT7;
              break;
              default:
                  P3DIR = 0xff;
                  P3OUT = 0x00;
              break;
             }
         }
     else if( x == 4)
        {
            switch(y)
             {
              case 0:
                  P4DIR |= BIT0;
                  P4OUT |= BIT0;
                  P4OUT &=~BIT0;
              break;
              case 1:
                  P4DIR |= BIT1;
                  P4OUT |= BIT1;
                  P4OUT &=~BIT1;
              break;
              case 2:
                  P4DIR |= BIT2;
                  P4OUT |= BIT2;
                  P4OUT &=~BIT2;
              break;
              case 3:
                  P4DIR |= BIT3;
                  P4OUT |= BIT3;
                  P4OUT &=~BIT3;
              break;
              case 4:
                  P4DIR |= BIT4;
                  P4OUT |= BIT4;
                  P4OUT &=~BIT4;
              break;
              case 5:
                  P4DIR |= BIT5;
                  P4OUT |= BIT5;
                  P4OUT &=~BIT5;
              break;
              case 6:
                  P4DIR |= BIT6;
                  P4OUT |= BIT6;
                  P4OUT &=~BIT6;
              break;
              case 7:
                  P4DIR |= BIT7;
                  P4OUT |= BIT7;
                  P4OUT &=~BIT7;
              break;
              default:
                  P4DIR = 0xff;
                  P4OUT = 0x00;
              break;
             }
         }
     else if( x == 5)
        {
            switch(y)
             {
              case 0:
                  P5DIR |= BIT0;
                  P5OUT |= BIT0;
                  P5OUT &=~BIT0;
              break;
              case 1:
                  P5DIR |= BIT1;
                  P5OUT |= BIT1;
                  P5OUT &=~BIT1;
              break;
              case 2:
                  P5DIR |= BIT2;
                  P5OUT |= BIT2;
                  P5OUT &=~BIT2;
              break;
              case 3:
                  P5DIR |= BIT3;
                  P5OUT |= BIT3;
                  P5OUT &=~BIT3;
              break;
              case 4:
                  P5DIR |= BIT4;
                  P5OUT |= BIT4;
                  P5OUT &=~BIT4;
              break;
              case 5:
                  P5DIR |= BIT5;
                  P5OUT |= BIT5;
                  P5OUT &=~BIT5;
              break;
              case 6:
                  P5DIR |= BIT6;
                  P5OUT |= BIT6;
                  P5OUT &=~BIT6;
              break;
              case 7:
                  P5DIR |= BIT7;
                  P5OUT |= BIT7;
                  P5OUT &=~BIT7;
              break;
              default:
                  P5DIR = 0xff;
                  P5OUT = 0x00;
              break;
             }
         }
     else if( x == 6)
        {
            switch(y)
             {
              case 0:
                  P6DIR |= BIT0;
                  P6OUT |= BIT0;
                  P6OUT &=~BIT0;
              break;
              case 1:
                  P6DIR |= BIT1;
                  P6OUT |= BIT1;
                  P6OUT &=~BIT1;
              break;
              case 2:
                  P6DIR |= BIT2;
                  P6OUT |= BIT2;
                  P6OUT &=~BIT2;
              break;
              case 3:
                  P6DIR |= BIT3;
                  P6OUT |= BIT3;
                  P6OUT &=~BIT3;
              break;
              case 4:
                  P6DIR |= BIT4;
                  P6OUT |= BIT4;
                  P6OUT &=~BIT4;
              break;
              case 5:
                  P6DIR |= BIT5;
                  P6OUT |= BIT5;
                  P6OUT &=~BIT5;
              break;
              case 6:
                  P6DIR |= BIT6;
                  P6OUT |= BIT6;
                  P6OUT &=~BIT6;
              break;
              case 7:
                  P6DIR |= BIT7;
                  P6OUT |= BIT7;
                  P6OUT &=~BIT7;
              break;
              default:
                  P6DIR = 0xff;
                  P6OUT = 0x00;
              break;
             }
         }
     else if( x == 7)
        {
            switch(y)
             {
              case 0:
                  P7DIR |= BIT0;
                  P7OUT |= BIT0;
                  P7OUT &=~BIT0;
              break;
              case 1:
                  P7DIR |= BIT1;
                  P7OUT |= BIT1;
                  P7OUT &=~BIT1;
              break;
              case 2:
                  P7DIR |= BIT2;
                  P7OUT |= BIT2;
                  P7OUT &=~BIT2;
              break;
              case 3:
                  P7DIR |= BIT3;
                  P7OUT |= BIT3;
                  P7OUT &=~BIT3;
              break;
              case 4:
                  P7DIR |= BIT4;
                  P7OUT |= BIT4;
                  P7OUT &=~BIT4;
              break;
              case 5:
                  P7DIR |= BIT5;
                  P7OUT |= BIT5;
                  P7OUT &=~BIT5;
              break;
              case 6:
                  P7DIR |= BIT6;
                  P7OUT |= BIT6;
                  P7OUT &=~BIT6;
              break;
              case 7:
                  P7DIR |= BIT7;
                  P7OUT |= BIT7;
                  P7OUT &=~BIT7;
              break;
              default:
                  P7DIR = 0xff;
                  P7OUT = 0x00;
              break;
             }
         }
     else if( x == 8)
        {
            switch(y)
             {
              case 0:
                  P8DIR |= BIT0;
                  P8OUT |= BIT0;
                  P8OUT &=~BIT0;
              break;
              case 1:
                  P8DIR |= BIT1;
                  P8OUT |= BIT1;
                  P8OUT &=~BIT1;
              break;
              case 2:
                  P8DIR |= BIT2;
                  P8OUT |= BIT2;
                  P8OUT &=~BIT2;
              break;
              default:
                  P8DIR = 0xff;
                  P8OUT = 0x00;
              break;
             }
         }

}




void OUTPUT_GPIO_State(unsigned int x,unsigned int y,unsigned int z)
{
if( z == 1 )
 {
    if( x == 1 )
       {
       switch(y)
        {
         case 0:P1OUT |= BIT0;break;
         case 1:P1OUT |= BIT1;break;
         case 2:P1OUT |= BIT2;break;
         case 3:P1OUT |= BIT3;break;
         case 4:P1OUT |= BIT4;break;
         case 5:P1OUT |= BIT5;break;
         case 6:P1OUT |= BIT6;break;
         case 7:P1OUT |= BIT7;break;
        }
       }
       else if( x == 2 )
       {
           switch(y)
            {
           case 0:P2OUT |= BIT0;break;
           case 1:P2OUT |= BIT1;break;
           case 2:P2OUT |= BIT2;break;
           case 3:P2OUT |= BIT3;break;
           case 4:P2OUT |= BIT4;break;
           case 5:P2OUT |= BIT5;break;
           case 6:P2OUT |= BIT6;break;
           case 7:P2OUT |= BIT7;break;
            }
       }
       else if( x == 3 )
       {
           switch(y)
            {
           case 0:P3OUT |= BIT0;break;
           case 1:P3OUT |= BIT1;break;
           case 2:P3OUT |= BIT2;break;
           case 3:P3OUT |= BIT3;break;
           case 4:P3OUT |= BIT4;break;
           case 5:P3OUT |= BIT5;break;
           case 6:P3OUT |= BIT6;break;
           case 7:P3OUT |= BIT7;break;
            }
       }
       else if( x == 4 )
       {
           switch(y)
            {
           case 0:P4OUT |= BIT0;break;
           case 1:P4OUT |= BIT1;break;
           case 2:P4OUT |= BIT2;break;
           case 3:P4OUT |= BIT3;break;
           case 4:P4OUT |= BIT4;break;
           case 5:P4OUT |= BIT5;break;
           case 6:P4OUT |= BIT6;break;
           case 7:P4OUT |= BIT7;break;
            }
       }
       else if( x == 5 )
       {
           switch(y)
            {
           case 0:P5OUT |= BIT0;break;
           case 1:P5OUT |= BIT1;break;
           case 2:P5OUT |= BIT2;break;
           case 3:P5OUT |= BIT3;break;
           case 4:P5OUT |= BIT4;break;
           case 5:P5OUT |= BIT5;break;
           case 6:P5OUT |= BIT6;break;
           case 7:P5OUT |= BIT7;break;
            }
       }
       else if( x == 6 )
       {
           switch(y)
            {
           case 0:P6OUT |= BIT0;break;
           case 1:P6OUT |= BIT1;break;
           case 2:P6OUT |= BIT2;break;
           case 3:P6OUT |= BIT3;break;
           case 4:P6OUT |= BIT4;break;
           case 5:P6OUT |= BIT5;break;
           case 6:P6OUT |= BIT6;break;
           case 7:P6OUT |= BIT7;break;
            }
       }
       else if( x == 7 )
       {
           switch(y)
            {
           case 0:P7OUT |= BIT0;break;
           case 1:P7OUT |= BIT1;break;
           case 2:P7OUT |= BIT2;break;
           case 3:P7OUT |= BIT3;break;
           case 4:P7OUT |= BIT4;break;
           case 5:P7OUT |= BIT5;break;
           case 6:P7OUT |= BIT6;break;
           case 7:P7OUT |= BIT7;break;
            }
       }
       else if( x == 8 )
       {
           switch(y)
            {
           case 0:P8OUT |= BIT0;break;
           case 1:P8OUT |= BIT1;break;
           case 2:P8OUT |= BIT2;break;
            }
       }
   }
 else
 {
     if( x == 1 )
        {
            switch(y)
             {
              case 0:P1OUT &=~ BIT0;break;
              case 1:P1OUT &=~ BIT1;break;
              case 2:P1OUT &=~ BIT2;break;
              case 3:P1OUT &=~ BIT3;break;
              case 4:P1OUT &=~ BIT4;break;
              case 5:P1OUT &=~ BIT5;break;
              case 6:P1OUT &=~ BIT6;break;
              case 7:P1OUT &=~ BIT7;break;
             }
        }
     else if( x == 2 )
         {
             switch(y)
             {
              case 0:P2OUT &=~ BIT0;break;
              case 1:P2OUT &=~ BIT1;break;
              case 2:P2OUT &=~ BIT2;break;
              case 3:P2OUT &=~ BIT3;break;
              case 4:P2OUT &=~ BIT4;break;
              case 5:P2OUT &=~ BIT5;break;
              case 6:P2OUT &=~ BIT6;break;
              case 7:P2OUT &=~ BIT7;break;
             }
         }
     else if( x == 3 )
         {
             switch(y)
             {
              case 0:P3OUT &=~ BIT0;break;
              case 1:P3OUT &=~ BIT1;break;
              case 2:P3OUT &=~ BIT2;break;
              case 3:P3OUT &=~ BIT3;break;
              case 4:P3OUT &=~ BIT4;break;
              case 5:P3OUT &=~ BIT5;break;
              case 6:P3OUT &=~ BIT6;break;
              case 7:P3OUT &=~ BIT7;break;
             }
         }
     else if( x == 4 )
         {
             switch(y)
             {
              case 0:P4OUT &=~ BIT0;break;
              case 1:P4OUT &=~ BIT1;break;
              case 2:P4OUT &=~ BIT2;break;
              case 3:P4OUT &=~ BIT3;break;
              case 4:P4OUT &=~ BIT4;break;
              case 5:P4OUT &=~ BIT5;break;
              case 6:P4OUT &=~ BIT6;break;
              case 7:P4OUT &=~ BIT7;break;
             }
         }
     else if( x == 5 )
         {
             switch(y)
             {
              case 0:P5OUT &=~ BIT0;break;
              case 1:P5OUT &=~ BIT1;break;
              case 2:P5OUT &=~ BIT2;break;
              case 3:P5OUT &=~ BIT3;break;
              case 4:P5OUT &=~ BIT4;break;
              case 5:P5OUT &=~ BIT5;break;
              case 6:P5OUT &=~ BIT6;break;
              case 7:P5OUT &=~ BIT7;break;
             }
         }
     else if( x == 6 )
         {
             switch(y)
             {
              case 0:P6OUT &=~ BIT0;break;
              case 1:P6OUT &=~ BIT1;break;
              case 2:P6OUT &=~ BIT2;break;
              case 3:P6OUT &=~ BIT3;break;
              case 4:P6OUT &=~ BIT4;break;
              case 5:P6OUT &=~ BIT5;break;
              case 6:P6OUT &=~ BIT6;break;
              case 7:P6OUT &=~ BIT7;break;
             }
         }
     else if( x == 7 )
         {
             switch(y)
             {
              case 0:P7OUT &=~ BIT0;break;
              case 1:P7OUT &=~ BIT1;break;
              case 2:P7OUT &=~ BIT2;break;
              case 3:P7OUT &=~ BIT3;break;
              case 4:P7OUT &=~ BIT4;break;
              case 5:P7OUT &=~ BIT5;break;
              case 6:P7OUT &=~ BIT6;break;
              case 7:P7OUT &=~ BIT7;break;
             }
         }
     else if( x == 8 )
         {
             switch(y)
             {
              case 0:P8OUT &=~ BIT0;break;
              case 1:P8OUT &=~ BIT1;break;
              case 2:P8OUT &=~ BIT2;break;
             }
         }
  }
}



void INPUT_GPIO_Config(unsigned int x,unsigned int y)
{
    if( x == 1 )
    {
    switch(y)
     {
      case 0:
          P1DIR &=~ BIT0;
          P1REN |=  BIT0;
      break;
      case 1:
          P1DIR &=~ BIT1;
          P1REN |=  BIT1;
      break;
      case 2:
          P1DIR &=~ BIT2;
          P1REN |=  BIT2;
      break;
      case 3:
          P1DIR &=~ BIT3;
          P1REN |=  BIT3;
      break;
      case 4:
          P1DIR &=~ BIT4;
          P1REN |=  BIT4;
      break;
      case 5:
          P1DIR &=~ BIT5;
          P1REN |=  BIT5;
      break;
      case 6:
          P1DIR &=~ BIT6;
          P1REN |=  BIT6;
      break;
      case 7:
          P1DIR &=~ BIT7;
          P1REN |=  BIT7;
      break;
      default:
          P1DIR = 0xff;
          P1OUT = 0x00;
      break;
     }
    }
    else if( x == 2 )
    {
        switch(y)
         {
          case 0:
              P2DIR &=~ BIT0;
              P2REN |=  BIT0;
          break;
          case 1:
              P2DIR &=~ BIT1;
              P2REN |=  BIT1;
          break;
          case 2:
              P2DIR &=~ BIT2;
              P2REN |=  BIT2;
          break;
          case 3:
              P2DIR &=~ BIT3;
              P2REN |=  BIT3;
          break;
          case 4:
              P2DIR &=~ BIT4;
              P2REN |=  BIT4;
          break;
          case 5:
              P2DIR &=~ BIT5;
              P2REN |=  BIT5;
          break;
          case 6:
              P2DIR &=~ BIT6;
              P2REN |=  BIT6;
          break;
          case 7:
              P2DIR &=~ BIT7;
              P2REN |=  BIT7;
          break;
          default:
              P2DIR = 0xff;
              P2OUT = 0x00;
          break;
         }
     }
     else if( x == 3)
        {
            switch(y)
             {
              case 0:
                  P3DIR &=~ BIT0;
                  P3REN |=  BIT0;
              break;
              case 1:
                  P3DIR &=~ BIT1;
                  P3REN |=  BIT1;
              break;
              case 2:
                  P3DIR &=~ BIT2;
                  P3REN |=  BIT2;
              break;
              case 3:
                  P3DIR &=~ BIT3;
                  P3REN |=  BIT3;
              break;
              case 4:
                  P3DIR &=~ BIT4;
                  P3REN |=  BIT4;
              break;
              case 5:
                  P3DIR &=~ BIT5;
                  P3REN |=  BIT5;
              break;
              case 6:
                  P3DIR &=~ BIT6;
                  P3REN |=  BIT6;
              break;
              case 7:
                  P3DIR &=~ BIT7;
                  P3REN |=  BIT7;
              break;
              default:
                  P3DIR = 0xff;
                  P3OUT = 0x00;
              break;
             }
         }
     else if( x == 4)
        {
            switch(y)
             {
              case 0:
                  P4DIR &=~ BIT0;
                  P4REN |=  BIT0;
              break;
              case 1:
                  P4DIR &=~ BIT1;
                  P4REN |=  BIT1;
              break;
              case 2:
                  P4DIR &=~ BIT2;
                  P4REN |=  BIT2;
              break;
              case 3:
                  P4DIR &=~ BIT3;
                  P4REN |=  BIT3;
              break;
              case 4:
                  P4DIR &=~ BIT4;
                  P4REN |=  BIT4;
              break;
              case 5:
                  P4DIR &=~ BIT5;
                  P4REN |=  BIT5;
              break;
              case 6:
                  P4DIR &=~ BIT6;
                  P4REN |=  BIT6;
              break;
              case 7:
                  P4DIR &=~ BIT7;
                  P4REN |=  BIT7;
              break;
              default:
                  P4DIR = 0xff;
                  P4OUT = 0x00;
              break;
             }
         }
     else if( x == 5)
        {
            switch(y)
             {
              case 0:
                  P5DIR &=~ BIT0;
                  P5REN |=  BIT0;
              break;
              case 1:
                  P5DIR &=~ BIT1;
                  P5REN |=  BIT1;
              break;
              case 2:
                  P5DIR &=~ BIT2;
                  P5REN |=  BIT2;
              break;
              case 3:
                  P5DIR &=~ BIT3;
                  P5REN |=  BIT3;
              break;
              case 4:
                  P5DIR &=~ BIT4;
                  P5REN |=  BIT4;
              break;
              case 5:
                  P5DIR &=~ BIT5;
                  P5REN |=  BIT5;
              break;
              case 6:
                  P5DIR &=~ BIT6;
                  P5REN |=  BIT6;
              break;
              case 7:
                  P5DIR &=~ BIT7;
                  P5REN |=  BIT7;
              break;
              default:
                  P5DIR = 0xff;
                  P5OUT = 0x00;
              break;
             }
         }
     else if( x == 6)
        {
            switch(y)
             {
              case 0:
                  P6DIR &=~ BIT0;
                  P6REN |=  BIT0;
              break;
              case 1:
                  P6DIR &=~ BIT1;
                  P6REN |=  BIT1;
              break;
              case 2:
                  P6DIR &=~ BIT2;
                  P6REN |=  BIT2;
              break;
              case 3:
                  P6DIR &=~ BIT3;
                  P6REN |=  BIT3;
              break;
              case 4:
                  P6DIR &=~ BIT4;
                  P6REN |=  BIT4;
              break;
              case 5:
                  P6DIR &=~ BIT5;
                  P6REN |=  BIT5;
              break;
              case 6:
                  P6DIR &=~ BIT6;
                  P6REN |=  BIT6;
              break;
              case 7:
                  P6DIR &=~ BIT7;
                  P6REN |=  BIT7;
              break;
              default:
                  P6DIR = 0xff;
                  P6OUT = 0x00;
              break;
             }
         }
     else if( x == 7)
        {
            switch(y)
             {
              case 0:
                  P7DIR &=~ BIT0;
                  P7REN |=  BIT0;
              break;
              case 1:
                  P7DIR &=~ BIT1;
                  P7REN |=  BIT1;
              break;
              case 2:
                  P7DIR &=~ BIT2;
                  P7REN |=  BIT2;
              break;
              case 3:
                  P7DIR &=~ BIT3;
                  P7REN |=  BIT3;
              break;
              case 4:
                  P7DIR &=~ BIT4;
                  P7REN |=  BIT4;
              break;
              case 5:
                  P7DIR &=~ BIT5;
                  P7REN |=  BIT5;
              break;
              case 6:
                  P7DIR &=~ BIT6;
                  P7REN |=  BIT6;
              break;
              case 7:
                  P7DIR &=~ BIT7;
                  P7REN |=  BIT7;
              break;
              default:
                  P7DIR = 0xff;
                  P7OUT = 0x00;
              break;
             }
         }
     else if( x == 8)
        {
            switch(y)
             {
              case 0:
                  P8DIR &=~ BIT0;
                  P8REN |=  BIT0;
              break;
              case 1:
                  P8DIR &=~ BIT1;
                  P8REN |=  BIT1;
              break;
              case 2:
                  P8DIR &=~ BIT2;
                  P8REN |=  BIT2;
              break;
              default:
                  P8DIR = 0xff;
                  P8OUT = 0x00;
              break;
             }
         }

}




/*********************************************END OF FILE**********************/
