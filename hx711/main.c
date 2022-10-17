#include "msp430.h"
#include "12864.c"
#include "12864.h"
#include "hx711.h"
#include "hx711.c"
#include "msp430f5529.h"
void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;// Stop watchdog timer to prevent time out reset
  LCD12864_Init();
  _NOP();
    CH0;//´®¿Ú
  WriteChineseCharacters(1,0,"ÖØÁ¿£º      g",7);
   uint a;
  unsigned long i,sum;
  while(1)
  {
     
     for(a=0;a<30;a++)
    {
      sum+=HX711_Read();
    }
     i=sum/30;
    

    i=HX711_Read();
    Write_5Number(1,3,i);
  }
}
