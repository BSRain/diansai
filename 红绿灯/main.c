#include <msp430f5529.h>
#include "shumaguan.h"
#include "shumaguan2.h"
#include <stdio.h>
#include <string.h>
#include <intrinsics.h>
#include <stdint.h>
#include <intrinsics.h>
#include <stdint.h>
#include "FLASH.h"
#define CPU_F ((double)1035000)
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define _delay_cycles __delay_cycles

#define r1     P7OUT ^= BIT0;//红
#define y1     P3OUT ^= BIT5;//黄
#define g1     P3OUT ^= BIT6;//绿
#define g2     P6OUT ^= BIT0;//绿
#define y2     P6OUT ^= BIT3;//黄
#define r2     P6OUT ^= BIT2;//红

#define TRIGGER_PIN BIT1   // P6.1 
#define ECHO_PIN BIT3  // P1.3 
#define LED_PIN BIT0   // P1.0 
#define DISTANCE_THRESHOLD 10  // cm
#define MEASURE_INTERVAL 2048  // ~250 ms


void mie();
void mie2();
int morenshijian=20;
int shijian;
unsigned int k=0;
unsigned int LastCCR1;		//上一次高电平持续的时间，和距离正相关
unsigned int dat[20];		//通过数组备份采集到的数据
#include <intrinsics.h>
#include <stdint.h>				
int shuflag=0;
int cnt=9;
int flag=1;
int cflag=0;
int anjianflag=1;
int stoi(char *str)
{
  int size=2;
  int num=0;
  if(str[1]=='0')
    size=1;
  for(int i=0;i<size;i++)
  {
    num=num*10+str[i]-'0';
  }
  return num;
}

void triggerMeasurement() 
{
	// Start trigger
	P6OUT |= TRIGGER_PIN;

	// Wait a small amount of time with trigger high, > 10us required (~10 clock cycles at 1MHz MCLK)
	__delay_cycles(10);

	// End trigger
	P6OUT &= ~TRIGGER_PIN;
}

void shijian_init()
{
  char ans[2]={'2','0'};
  read_flash_char1(0x1800,ans,2);
  morenshijian=stoi(ans);
}
int main( void )
{
  // Stop watchdog timer to prevent time out reset
   WDTCTL = WDTPW + WDTHOLD;

        shijian_init();
    P1IE |= BIT1;
  	P1IES |= BIT1;
  	P1IFG &= ~BIT1;
  	P1REN |=  BIT1;
  	P1OUT |=  BIT1;
    
  // Configure trigger pin, low to start
	P6DIR |= TRIGGER_PIN;
	P6OUT &= ~TRIGGER_PIN;

	// Configure LED, off to start
	P1DIR |= LED_PIN;
	P1OUT &= ~LED_PIN;

	// Configure echo pin as capture input to TA0CCR2
	P1DIR &= ~ECHO_PIN;
	P1SEL |= ECHO_PIN;

	// Set up TA0 to capture in CCR2 on both edges from P1.3 (echo pin)
	TA0CCTL2 = CM_3 | CCIS_0 | SCS | CAP | CCIE;

	// Set up TA0 to compare CCR0 (measure interval)
	TA0CCR0 = MEASURE_INTERVAL;
	TA0CCTL0 = CCIE;

	// Set up TA0 with ACLK / 4 = 8192 Hz
	TA0CTL = TASSEL__ACLK | ID__4 | MC__CONTINUOUS | TACLR;

	uint16_t lastCount = 0;
	uint32_t distance = 20;
  
  shumaguan_init();
  shumaguan_init2();

  /*TA0CCTL0 = CCIE;
  P1DIR |= BIT0;
  TA0CCR0 = 32768;//延时1s  
  TA0CTL = TASSEL_1 + MC_1 + TACLR;*/
   /*UART的初始化*/
    P3SEL |= BIT3+BIT4;//TX 3.4 RX3.3
    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL_2; 
    UCA0MCTL |= UCBRS_1 + UCBRF_0;
    UCA0BR0 = 9;
    UCA0BR1 = 0;
    UCA0CTL1 &= ~UCSWRST;
    UCA0IE |= UCRXIE;
    //灯初始化
    //主
    P3DIR |= BIT6;
    P3OUT &= ~ BIT6;
    P3DIR |= BIT5;
    P3OUT &= ~ BIT5;
    P7DIR |= BIT0;
    P7OUT &= ~BIT0;
    //副
    P6DIR |= BIT0;
    P6DIR |= BIT3;
    P6DIR |= BIT2;
    P6OUT &= ~ BIT0;//绿
    P6OUT &= ~ BIT3;//黄
    P6OUT &= ~ BIT2;//红
  shijian=morenshijian;
  __bis_SR_register(LPM0_bits + GIE);
  //__enable_interrupt();
  P6OUT |= BIT0; //绿2
  P7OUT |= BIT0; //红1
  int chaoshenbo=0;
  while(1)
  {
    //hong->huang->lv
    if(cflag==0)
    {     
          shijian-=1;
          shuflag=0;
          mie(),mie2();

          if(shijian<=9)
         {
          shuflag=1;
         }
          if(shuflag)
        {
          if(shijian==9)nine(),nine2();
          if(shijian==8)eight(),eight2();
          if(shijian==7)seven(),seven2();
          if(shijian==6)six(),six2();
          if(shijian==5)five(),five2();
          if(shijian==4)four(),four2();
          if(shijian==3)three(),three2();
          if(shijian==2)two(),two2();
          if(shijian==1)one(),one2();
          if(shijian==0)zero(),zero2();
          cnt-=1;
        }
        if(shijian==5&&flag%2==1)
        {
          r1;
          g2;
          y1;
          y2;
        }
        else if(shijian==5&&flag%2==0)
        {
          r2;
          g1;
          y1;
          y2;
        }
        if(shijian==0&&flag%2==1)
        {
          y1;
          y2;
          g1;
          r2;
        shijian=morenshijian;
        cnt=9;
        flag+=1;
        }
        else if(shijian==0&&flag%2==0)
        {
          y1;
          y2;
          g2;
          r1;
          shijian=morenshijian;
          cnt=9;
          flag+=1;
        }
        
        delay_ms(1000);
        
    }
    else if(cflag==1)
    {

      P3OUT &= ~BIT6;//绿1
      P7OUT |= BIT0;//红1
      P3OUT &= ~BIT5;//黄1
      P6OUT &= ~BIT2;//红2
      P6OUT |= BIT0;//绿2
      P6OUT &= ~BIT1;//黄2
      shijian=morenshijian;
    }
    else if(cflag==2)
    {
      P3OUT |= BIT6;//绿1
      P7OUT &= ~BIT0;//红1
      P3OUT &= ~BIT5;//黄1
      P6OUT |= BIT2;//红2
      P6OUT &= ~ BIT0;//绿
      P6OUT &= ~ BIT1;//黄2
      shijian=morenshijian;
    }

    triggerMeasurement();

		// Wait for echo start
		__low_power_mode_3();

		lastCount = TA0CCR2;

		// Wait for echo end
		__low_power_mode_3();

		distance = TA0CCR2 - lastCount;
		distance *= 34000;
		distance >>= 14;  // division by 16384 (2 ^ 14)
                //int chaoshenbo=0;
		if (distance > DISTANCE_THRESHOLD&&chaoshenbo)
		{
			// Turn on LED
                        // 有车
			P1OUT |= LED_PIN;
                        if(anjianflag==1)
                        cflag=1;
                        else if(anjianflag==2)
                        cflag=2;
		}
		else if(chaoshenbo&&distance <= DISTANCE_THRESHOLD)
		{
			// Turn off LED
                        //无车
			P1OUT &= ~LED_PIN;
                        cflag=0;
		}
                chaoshenbo=1;
		// Wait for the next measure interval tick
		__low_power_mode_3();

  }
  //__bis_SR_register(LPM0_bits + GIE);
  
}

char data[5];
#pragma vector=USCI_A0_VECTOR  // 进入中断向量地址
__interrupt void USCI_A0_ISR(void)
{
  //static char Buff = 0;
  while (!(UCA0IFG&UCRXIFG));
   int tp=0;
  while(tp!=5)
  {
    //Buff = UCA0RXBUF;
    data[tp]=UCA0RXBUF;
    tp++;
    __delay_cycles(1505);
  }
  if(data[0]=='t')
  {
    char shudata[2];
    shudata[0]=data[1];
    if(data[2]<='9'&&data[2]>='0')
    shudata[1]=data[2];
    else
      shudata[1]='0';
    morenshijian=stoi(shudata);
    shijian=morenshijian;
    write_flash_char(0x1800,shudata,2);
  }
  else if(data[0]=='c')
  {
    if(data[1]=='1')
    {cflag=1;}
    else if(data[1]=='2')
    {cflag=2;}
    else if(data[1]=='0')
    {cflag=0;}
  }
  __disable_interrupt();
  UCA0IFG &= ~UCRXIFG;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void) {
	// Measure interval tick
	__low_power_mode_off_on_exit();
	TA0CCR0 += MEASURE_INTERVAL;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR (void) {
	// Echo pin state toggled
	__low_power_mode_off_on_exit();
	TA0IV = 0;
}
int anjiancnt=1;
#pragma vector = PORT1_VECTOR    //S1中断服务函数
__interrupt void P1_ISR()
{
	if(P1IFG & BIT1)
	{
		while((P1IN & BIT1)==0);
                if(anjiancnt==1)
		anjianflag=2,anjiancnt=2;
                else
                anjianflag=1,anjiancnt=1;
                
	}
	P1IFG &=~ BIT1;
}