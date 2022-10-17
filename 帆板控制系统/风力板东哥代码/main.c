#include "msp430g2553.h"
#include "stdint.h"
int flag = 0;
void time(void){
  
 //矫正时钟源DCO
DCOCTL = CALDCO_1MHZ;
BCSCTL1 = CALBC1_1MHZ;
//SMCLK 时钟源和分频系数
BCSCTL2 &= ~SELS;
BCSCTL2 &= ~(DIVS0|DIVS1) ;

}
void UART(void)
{
//进行配置 
UCA0CTL1 |= UCSWRST;
//设置为UART模式
UCA0CTL0 &= ~UCSYNC;
//设置时钟源
UCA0CTL1 |= BIT7;
//设置波特率9600
UCA0BR0 = 0X68;
UCA0BR1 = 0X00;
UCA0MCTL|= BIT1;
// 端口复用
P1SEL |= BIT1 + BIT2;
P1SEL2 |= BIT1 + BIT2;

UCA0CTL1 &= ~UCSWRST;
}

void FS(uint8_t*pbuff,uint8_t num)
{

  for(int cnt=0;cnt<num;cnt++)
  {
    while(UCA0STAT & BIT0);
   UCA0TXBUF = *(pbuff + cnt);
  }

}

void PrintNumber(uint16_t num)
{
  uint8_t buff[6];
  buff[5] = '\n';
  for(int i = 0;i<5;i++){
  buff[4-i] = (uint8_t)(num%10 + '0');
  num = num/10;
  }
  FS(buff,6);
}
//初始化ADC
void InitADC(void){
  //设置时钟位MCLK 分频为2分频
ADC10CTL1 |= ADC10SSEL_2;

ADC10CTL1 |= ADC10DIV_0;
//设置ADC基准源
ADC10CTL0 |= SREF_1;
ADC10CTL0 |= REF2_5V;
ADC10CTL0 |= REFON;
//设置采样保存数据64CLK
ADC10CTL0 |= ADC10SHT_3;
//设置采样率200K
ADC10CTL0 &= ~ADC10SR;
//选择输入通道
ADC10CTL1 |= INCH_1;
//ADC10AE0 |= BIT2;


//打开ADC
ADC10CTL0 |= ADC10ON;

}

int GET(void)
{
ADC10CTL0 |= ADC10SC|ENC;

while(ADC10CTL1&ADC10BUSY);
return ADC10MEM;
}

//PID
float  PID_Loc(int SetValat,int ActualValat)
{float PID_Loc=0;
float Kp= 10,Ki=10,Kd=0;
float E=0,E1=0;
float Esum=0;
E = SetValat - ActualValat;
if(E>40){Esum+=0;}
else{Esum += E;}
PID_Loc = Kp*E+(Ki*Esum)+Kd*(E-E1);
E1=E;

return PID_Loc;
}
//PWM波使用
void Set(int S)
{

TA1CTL |= TASSEL1;
TA1CTL |= MC_1;
TA1CCR0 = 20000;
TA1CCR1 = S;
TA1CCTL0 &= ~CAP;
TA1CCTL1 &= ~CAP;

TA1CCTL1 |= OUTMOD_6;
//端口复用
P2SEL |= BIT1;
P2DIR |= BIT1;

P1DIR |= BIT6+BIT7;
P1OUT |= BIT6;
P1OUT &= ~BIT7;
}
void key(void)
{
//灯泡初始化
//P1DIR |= BIT5;
//P2DIR |= BIT0;
//P1OUT &= ~BIT5;
//P2OUT &= ~BIT0;
//蜂鸣器初始化
P2DIR |= BIT5;
P2OUT &= ~BIT5;
//按键初始化
  P1DIR &= ~BIT4;
  P1REN |= BIT4;
  P1OUT |= BIT4;
  P2DIR &= ~BIT2+~BIT4+~BIT5;
  P2REN |= BIT2+BIT4+BIT5;
  P2OUT |= BIT2+BIT4+BIT5;
  // 中断使能 
P1IE |= BIT4;
P1IES |= BIT4;
P1IFG &= ~BIT4;
P2IE |= BIT4;
P2IES |= BIT4;
P2IFG &= ~BIT4;
__bis_SR_register(GIE);
}
 
float x = 30;

void main( void )
{
  

  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
time();
key();
UART();
InitADC();
while(1){
if(flag==0){
while(1)
{
 if(!(P2IN & BIT2)==1)
{
  P1DIR |= BIT0;
  P1OUT ^= BIT0;
 x+=50;
 __delay_cycles(50000);
}
 if(!(P2IN & BIT5)==1)
{ P1DIR |= BIT0;
  P1OUT ^= BIT0;
 x-=50;
 __delay_cycles(50000);
}
 __delay_cycles(50000);
 if(flag==1)
 {break;}
}
}
if(flag){
   float y=x;
//  if(y<=45&&y>=30){y+=5;}
//  else if (y<=55){y = y*1.6;}
//  else if(y==60){y=120;}
  int adcvalue = 0;
   int s = 4500;
   float p = 0;
   int angle = 1058-y/0.23;
while(1){
  
  for(int i = 0;i<30;i++){
   adcvalue += GET();}
  adcvalue = adcvalue/30;
  
  p = PID_Loc(angle,adcvalue);
  
  
  Set(s+p);
  __delay_cycles(500000);
  

  if(flag==0)
{break;}
  }
}
}
}
#pragma vector =PORT1_VECTOR
__interrupt void  Port1(void)
{
  P1DIR |= BIT0;
  P1OUT |= BIT0;
  if(P1IFG & BIT4){
    flag=1;
  P1IFG &= ~BIT4;
  
  }
  
  
}
#pragma vector =PORT2_VECTOR
__interrupt void  Port2(void){
 P1OUT &= ~BIT0;


if(P2IFG & BIT4){
    flag=0;
  P2IFG &= ~BIT4;
  
  }

}
