#include <msp430f5529.h> 
#define CPU_F ((double)1035000)
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define _delay_cycles __delay_cycles
int sum=0;                                  //全局变量
int main(void)
{
  volatile unsigned int i;                  //局部变量
                //设置timerA相关参数
  WDTCTL = WDTPW+WDTHOLD;                   // 关闭看门狗                          
  P2DIR |= BIT0;                            //P2.0输出PWM波
  P2SEL |=BIT0;
  TA1CCTL1 = OUTMOD_7;
  TA1CTL = TASSEL_2 + MC_1 +TACLR+ID_2;
  TA1EX0 = TAIDEX_5;
  TA1CCR0 = 1800;

    
  while(1)                                       // 采用switch，每一个case值对应一个状态
  {
    TA1CCR1 = 10;
    delay_ms(1000);
    TA1CCR1 = 20;
    delay_ms(1000);
    TA1CCR1 = 30;
    delay_ms(1000);
    TA1CCR1 = 40;
    delay_ms(1000);
    TA1CCR1 = 50;
    delay_ms(1000);
    TA1CCR1 = 0;
    
  }

}