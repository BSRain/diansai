#include <msp430.h>
#include "oled.h"
#include "type.h"
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    int inputModel = 0;
    u16 VolInput = 89;
    u16 VolOutput = 1246;
    int cnt=0;
    OLED_Init();
    OLED_Clear();
    P1DIR |= BIT0;
    P1OUT &=~BIT0;
    P1OUT &= ~BIT0;
//    OLED_ShowString(0,0,"input model:",8);
//    OLED_ShowString(0,0,"V  in",8);
//    OLED_ShowString(7,4,"V out",8);
//    OLED_ShowString(7,5,"I  in",8);
    //OLED_ShowString(7,6,"I out",8);
    //OLED_ShowString(8,6,"жа",16);
    while(1)
    {
      OLED_ShowChinese(0,0,0,16);
      OLED_ShowChinese(16,0,1,16);
      OLED_ShowChinese(0,4,2,16);
      OLED_ShowChinese(0,6,3,16);
      OLED_ShowNum(16,2,P1OUT,2,16);
      P1OUT ^=BIT0;
      if(P2IN & BIT5)
        cnt++;
      OLED_ShowNum(32,6,cnt,2,16);
      __delay_cycles(50000);
      
      /*if(inputModel==0)
        {
            OLED_ShowString(13*8-3,0,"0",8);
            inputModel=1;
        }
        else
            {
                OLED_ShowString(13*8-3,0,"1",8);
                inputModel=0;
            }
        Delay_1ms(800);
        OLED_ShowVI(50-1,3,VolInput,8);
        OLED_ShowVI(50-1,5,VolOutput,8);
        VolInput++;*/
    }
    
}

