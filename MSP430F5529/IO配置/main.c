#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR |= BIT0;                            // Set P1.0 to output direction
  P2REN |= BIT1;                //I/O方向默认输入 PxREN 置1使能上/下拉电阻  置0关闭上/下拉电阻
  P2OUT |= BIT1;                //配置PxREN 寄存器后，PxOUT 置1为上拉电阻； 置0为下拉电阻；

  while (1)                                 // Test P2.1
  {
    if (P2IN & BIT1)
       P1OUT &= ~BIT0;                      // if P2.1 set, set P2.1
    else
      P1OUT |= BIT0;                        // else reset
  }

}