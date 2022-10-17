// control.c
#include <msp430f5529.h> 
#define CPU_F ((double)1035000)
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))//毫秒
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))//微秒
#define _delay_cycles __delay_cycles
#define  PUL1P  P2OUT |= BIT0 //puls
#define  PUL1M  P2OUT &= ~BIT0 //minus
#define  PUL2P P2OUT |= BIT2 
#define  PUL2M P2OUT &= BIT2
void Delay_nms(int n)
{
  for(short i=0;i<=n;i++)
    delay_ms(1);
}
void Delay_nus(int n)
{
  for(short i=0;i<=n;i++)
    delay_us(1);
}
void init_one()
{
  P2DIR |= BIT0;
  /*逻辑电平控制电机正反转*/
  P2DIR |=  BIT6;// + BIT4;
  P2OUT |=  BIT6;//控制正反
  //P1OUT &= ~BIT4;//接地
}
void init_two()
{
  P2DIR |= BIT2;
  /*逻辑电平控制电机正反转*/
  P2DIR |=  BIT3; //+ BIT4;
  P2OUT |=  BIT3;//控制正反
  //P1OUT &= ~BIT4;//接地
}
void cycle1_min_f(int time)//3.3 10mm
{
  P2OUT |=  BIT6;//收
  for( int x = 0; x <=34; x++) 
  { //一圈200个脉冲    两个pul为一个脉冲    1个脉冲就是1.8°
    P2OUT |= BIT0;
    Delay_nus(time/2);                //修改微秒值可以调速
    P2OUT &= ~BIT0;
    Delay_nus(time/2);
  }
}
void cycle1_min_s(int time)//3.3 10mm
{
  P2OUT &= ~ BIT6;//放
  for( int x = 0; x <=34; x++) 
  { //一圈200个脉冲    两个pul为一个脉冲    1个脉冲就是1.8°
    P2OUT |= BIT0;
    Delay_nus(time/2);                //修改微秒值可以调速
    P2OUT &= ~BIT0;
    Delay_nus(time/2);
  }
}
void cycle2_min_f(int time)//n为angle
{
  P2OUT |=  BIT3;//放
  for( int x = 0; x <=34; x++) 
  { //一圈200个脉冲    两个pul为一个脉冲    1个脉冲就是1.8°
    P2OUT |= BIT2; 
    Delay_nus(time/2);               //修改微秒值可以调速
    P2OUT &= ~BIT2; 
    Delay_nus(time/2);
  }
}
void cycle2_min_s(int time)//n为angle
{
  P2OUT &=~  BIT3;//收
  for( int x = 0; x <=34; x++) 
  { //一圈200个脉冲    两个pul为一个脉冲    1个脉冲就是1.8°
    P2OUT |= BIT2; 
    Delay_nus(time/2);               //修改微秒值可以调速
    P2OUT &= ~BIT2; 
    Delay_nus(time/2);
  }
}
void cycle1_choose_f(int n,int time)//n为angle
{
  P2OUT |=  BIT6;//收
  for( int x = 0; x <=n; x++) 
  { //一圈200个脉冲    两个pul为一个脉冲    1个脉冲就是1.8°
    P2OUT |= BIT0; 
    Delay_nus(time/2);               //修改微秒值可以调速
    P2OUT &= ~BIT0; 
    Delay_nus(time/2);
  }
}
void cycle1_choose_b(int n,int time)//n为angle
{
  P2OUT &= ~ BIT6;
  for( int x = 0; x <=n; x++) 
  { //一圈200个脉冲    两个pul为一个脉冲    1个脉冲就是1.8°
    P2OUT |= BIT0; 
    Delay_nus(time/2);                //修改微秒值可以调速
    P2OUT &= ~BIT0; 
    Delay_nus(time/2);                
  }
}
void cycle2_choose_f(int n,int time)//n为angle
{
  P2OUT |=  BIT3;
  for( int x = 0; x <=n; x++) 
  { //一圈200个脉冲    两个pul为一个脉冲    1个脉冲就是1.8°
    P2OUT |= BIT2; 
    Delay_nus(time/2);               //修改微秒值可以调速
    P2OUT &= ~BIT2; 
    Delay_nus(time/2);
  }
}
void cycle2_choose_b(int n,int time)//n为angle
{
  P2OUT &= ~ BIT3;
  for( int x = 0; x <=n; x++) 
  { //一圈200个脉冲    两个pul为一个脉冲    1个脉冲就是1.8°
    P2OUT |= BIT2; 
    Delay_nus(time/2);                //修改微秒值可以调速
    P2OUT &= ~BIT2; 
    Delay_nus(time/2);                
  }
}
void left(int n,int time)
{
  P2OUT &= ~ BIT6;
  P2OUT &= ~ BIT3;
  for( int x = 0; x <=n; x++) 
  { //一圈200个脉冲    两个pul为一个脉冲    1个脉冲就是1.8°
    P2OUT |= BIT0;
    P2OUT |= BIT2;
    Delay_nus(time/2);                //修改微秒值可以调速
    P2OUT &= ~BIT0;
    P2OUT &= ~BIT2;
    Delay_nus(time/2);                
  }
}
void right(int n,int time)
{
  P2OUT |=  BIT6;//右边收
  P2OUT |=  BIT3;//左边放
  for( int x = 0; x <=n; x++) 
  { //一圈200个脉冲    两个pul为一个脉冲    1个脉冲就是1.8°
    P2OUT |= BIT0; 
    P2OUT |= BIT2; 
    Delay_nus(time/2);               //修改微秒值可以调速
    
    P2OUT &= ~BIT0; 
    P2OUT &= ~BIT2;
    Delay_nus(time/2);
  }
  
}
void up(int n,int time)
{
  P2OUT |=  BIT6;//收
  P2OUT &= ~ BIT3;//收
  for( int x = 0; x <=n; x++) 
  { //一圈200个脉冲    两个pul为一个脉冲    1个脉冲就是1.8°
    P2OUT |= BIT0; 
    P2OUT |= BIT2;
    Delay_nus(time/2);               //修改微秒值可以调速
    P2OUT &= ~BIT0; 
    P2OUT &= ~BIT2;
    Delay_nus(time/2);
  }
  
}
void down(int n,int time)
{
  P2OUT &=~  BIT6;//放
  P2OUT |=  BIT3;//放
  for( int x = 0; x <=n; x++) 
  { //一圈200个脉冲    两个pul为一个脉冲    1个脉冲就是1.8°
    P2OUT |= BIT0; 
    P2OUT |= BIT2;
    Delay_nus(time/2);               //修改微秒值可以调速
    P2OUT &= ~BIT0; 
    P2OUT &= ~BIT2;
    Delay_nus(time/2);
  }
}