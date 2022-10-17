#include "stdio.h"
#include <msp430f5529.h>
#include <stdint.h>
#include <intrinsics.h>
#include "Black_All.h"
#include "oled.h"
#define TRIGGER_PIN BIT5    //2.5
#define ECHO_PIN BIT4  //2.4
#define LED_PIN BIT0   // P1.0
#define DISTANCE_THRESHOLD 20  // cm
#define MEASURE_INTERVAL 2048  // ~250 ms
uint16_t lastCount = 0;
uint32_t distance = 0;
int pwm1_L=40;
int pwm1_H=40;
int pwm1_go=26;

int pwm2_L=50;
int pwm2_H=50;
int pwm2_go=90;
int pwm2_delay=15;
int pwm2_fast=45;

int pwm4_L=40;
int pwm4_H=70;
int pwm4_go=90;

int mode=0;//ģʽ
char flag='0';//����
int cycle=0;//��תȦ��
int cn=0;//cn��ֹ��⵽������

void catinit(void)
{
  OUTPUT_GPIO_Config(8,2);//����
  OUTPUT_GPIO_Config(3,7);
  
  OUTPUT_GPIO_Config(2,2);//����
  OUTPUT_GPIO_Config(7,4);
  
  OUTPUT_GPIO_Config(2,6);//����
  OUTPUT_GPIO_Config(2,3);
  
  OUTPUT_GPIO_Config(4,0);//����
  OUTPUT_GPIO_Config(4,3);
  
  INPUT_GPIO_Config(6,0);
  INPUT_GPIO_Config(6,1);
  INPUT_GPIO_Config(6,2);
  INPUT_GPIO_Config(6,3);
}
void interrupt_two_key(void)
{
  
  P1DIR &= ~BIT1;
  P2DIR &= ~BIT1;          //����P1.1,P2.1��Ϊ����
  
  P1REN |= BIT1;
  P2REN |= BIT1;
  
  P1OUT |= BIT1;
  P2OUT |= BIT1;          //����P1.1,P2.1��������
  
  P1IES |= BIT1;
  P2IES |= BIT1;          //P1.1,P2.1�½��ش����ж�
  
  P1IE |= BIT1;
  P2IE |= BIT1;           //P1.1,P2.1�ж�ʹ��
  
  P1IFG &= ~BIT1;
  P2IFG &= ~BIT1;        //����жϱ�־λ
  
}
void triggerMeasurement() 
{
  // Start trigger
  P2OUT |= TRIGGER_PIN;
  
  // Wait a small amount of time with trigger high, > 10us required (~10 clock cycles at 1MHz MCLK)
  __delay_cycles(10);
  
  // End trigger
  P2OUT &= ~TRIGGER_PIN;
}
void hcsr04_init(void)//������ռ�� 1.2
{
  // Configure trigger pin, low to start
  P2DIR |= TRIGGER_PIN;
  P2OUT &= ~TRIGGER_PIN;
  
  // Configure LED, off to start
  P1DIR |= LED_PIN;
  P1OUT &= ~LED_PIN;
  
  // Configure echo pin as capture input to TA0CCR2
  P2DIR &= ~ECHO_PIN;
  P2SEL |= ECHO_PIN;
  
  // Set up TA0 to capture in CCR2 on both edges from P1.3 (echo pin)
  TA2CCTL1 = CM_3 | CCIS_0 | SCS | CAP | CCIE;
  
  // Set up TA0 to compare CCR0 (measure interval)
  TA2CCR0 = MEASURE_INTERVAL;
  TA2CCTL0 = CCIE;
  
  // Set up TA0 with ACLK / 4 = 8192 Hz
  TA2CTL = TASSEL__ACLK | ID__4 | MC__CONTINUOUS | TACLR;
}
void clock_init(void) 
{
  //��ʱ��B��ʼ��
  TB0CCTL0 = CCIE;
  TB0CCR0 = 32768/4;//��ʱ1s  
  TB0CTL = TASSEL_1 + MC_1 + TACLR;
}

void others_init(void)
{
  // ��������ʼ��
  P8DIR |= BIT1;
  P8OUT &=~ BIT1;
  //С�Ƴ�ʼ��
  P1DIR |= BIT0;
  P1OUT &=~ BIT0;
}
void front(void)
{
  OUTPUT_GPIO_State(2,2,1);//����
  OUTPUT_GPIO_State(7,4,0);
  OUTPUT_GPIO_State(4,0,1);//����
  OUTPUT_GPIO_State(4,3,0);
  
  
  OUTPUT_GPIO_State(2,6,1);//upzuo
  OUTPUT_GPIO_State(2,3,0);
  OUTPUT_GPIO_State(8,2,1);//downzuo
  OUTPUT_GPIO_State(3,7,0);
}

void left(void)
{
  OUTPUT_GPIO_State(2,2,1);//4.3����
  OUTPUT_GPIO_State(7,4,0);
  OUTPUT_GPIO_State(4,0,1);//4.3����
  OUTPUT_GPIO_State(4,3,0);
  
  
  OUTPUT_GPIO_State(2,6,0);//upzuo
  OUTPUT_GPIO_State(2,3,1);
  OUTPUT_GPIO_State(8,2,0);//zuo
  OUTPUT_GPIO_State(3,7,1);
}
void right(void)
{
  OUTPUT_GPIO_State(2,2,0);//����
  OUTPUT_GPIO_State(7,4,1);
  OUTPUT_GPIO_State(4,0,0);//����
  OUTPUT_GPIO_State(4,3,1);
  
  
  OUTPUT_GPIO_State(2,6,1);//upzuo
  OUTPUT_GPIO_State(2,3,0);
  OUTPUT_GPIO_State(8,2,1);//zuo
  OUTPUT_GPIO_State(3,7,0);
}
void back(void)
{
  OUTPUT_GPIO_State(2,2,0);//4.3����
  OUTPUT_GPIO_State(7,4,1);
  OUTPUT_GPIO_State(4,0,0);//4.3����
  OUTPUT_GPIO_State(4,3,1);
  
  
  OUTPUT_GPIO_State(2,6,0);//upzuo
  OUTPUT_GPIO_State(2,3,1);
  OUTPUT_GPIO_State(8,2,0);//zuo
  OUTPUT_GPIO_State(3,7,1);
}
void stop(void)
{
  OUTPUT_GPIO_State(2,2,0);//����
  OUTPUT_GPIO_State(7,4,0);
  OUTPUT_GPIO_State(4,0,0);//����
  OUTPUT_GPIO_State(4,3,0);
  
  
  OUTPUT_GPIO_State(2,6,0);//����
  OUTPUT_GPIO_State(2,3,0);
  OUTPUT_GPIO_State(8,2,0);//����
  OUTPUT_GPIO_State(3,7,0);
}
void you(int pwm1,int pwm0)
{ 
  PWM0_State(pwm1);//2.0����
  PWM1_State(pwm0);//1.3����
  PWM2_State(pwm1);//1.4����
  PWM3_State(pwm0);//1.5����
}

void zuo(int pwm1,int pwm0)
{
  
  PWM0_State(pwm0);
  PWM1_State(pwm1);
  PWM2_State(pwm0);
  PWM3_State(pwm1);
}
void qian(int pwm_go )
{
  PWM0_State(pwm_go );
  PWM1_State(pwm_go );
  PWM2_State(pwm_go );
  PWM3_State(pwm_go );
}
void ting(void)
{
  PWM0_State(0);
  PWM1_State(0);
  PWM2_State(0);
  PWM3_State(0);
}
void quansujianjin(void)
{
  PWM0_State(100);
  PWM1_State(100);
  PWM2_State(100);
  PWM3_State(100);
}

/*
�Ӻ���ǰ
6.3 6.2 6.1 


*/
void turn_left()
{
  while((P6IN & BIT2)==0)//�������ʱ������
  {
    left(); 
    zuo(pwm1_H,pwm1_L);
    cn=0;
  }
}
//ģʽ1
void xunji_1(void)//ȫ����ѭ��
{
  
  if(( (P6IN & BIT1)) && ((P6IN & BIT2) && (P6IN & BIT3)))//ȫ����
  {
    if(cn==0)
    {cycle+=1;cn=1;}

    
    front();
    qian(pwm1_go);
    P8OUT |=BIT1;
    delay_nms(100);
    P8OUT &=~ BIT1;
    
    
  }
  else if(((P6IN & BIT1)==0) && ((P6IN & BIT2)&&(P6IN & BIT3)))//���� ʶ����Ȧ
  {front();qian(pwm1_go);cn=0;} 
  else if((( (P6IN & BIT1) || (P6IN & BIT2))== 0) && (P6IN & BIT3))//��6.3��
  {
    while((P6IN & BIT2)==0)//�������ʱ������
    {
      left(); 
      zuo(pwm1_H,pwm1_L);
      cn=0;
    }
    //left(); zuo(pwm1_H,pwm1_L);   cn=0;delay_nms(120);
  }
  else if((( (P6IN & BIT2) || (P6IN & BIT3))== 0) && (P6IN & BIT1))//��6.1��
  {right();you(pwm1_H,pwm1_L);   cn=0;delay_nms(80);}
  else if((((P6IN & BIT1)  && (P6IN & BIT3))== 0) && (P6IN & BIT2))//��6.2�� 
  {front();qian(pwm1_go);cn=0;} 
  else 
  {front();qian(pwm1_go);}
}
//ģʽ��
void xunji_2(void)
{
  if(( (P6IN & BIT1)) && ((P6IN & BIT2) && (P6IN & BIT3)))//ȫ����
  {
    if(cn==0)
    {cycle+=1;cn=1;}
    
    front();
    P8OUT |=BIT1;
    delay_nms(50);
    P8OUT &=~ BIT1;
    qian(pwm2_go);
    
  }
  else if(((P6IN & BIT1)==0) && ((P6IN & BIT2)&&(P6IN & BIT3)))//���� ʶ����Ȧ
  {front();qian(pwm2_go);cn=0;} 
  else if((( (P6IN & BIT1) || (P6IN & BIT2))== 0) && (P6IN & BIT3))//��6.3��
  {
    
    while((P6IN & BIT2)==0)//�������ʱ������
    {
      left(); 
      zuo(pwm2_H,pwm2_L);
      cn=0;
    }
    
    //left(); zuo(pwm1_H,pwm1_L);   cn=0;delay_nms(120);
  }
  else if((( (P6IN & BIT2) || (P6IN & BIT3))== 0) && (P6IN & BIT1))//��6.1��
  {right();you(pwm2_H,pwm2_L);   cn=0;delay_nms(pwm2_delay);}
  else if((((P6IN & BIT1)  && (P6IN & BIT3))== 0) && (P6IN & BIT2))//��6.2�� 
  {front();qian(pwm2_go);cn=0;} 
  else {front();qian(pwm2_go);}
}
//ģʽ4
void xunji_4(void)//ȫ����ѭ��
{
  
  if(( (P6IN & BIT1)) && ((P6IN & BIT2) && (P6IN & BIT3)))//ȫ����
  {
    if(cn==0)
    {cycle+=1;cn=1;}
    if(cycle==1)
    {
      P8OUT |=BIT1;
      delay_nms(100);
      P8OUT &=~ BIT1;
    }
    front();
    qian(pwm4_go);
    
  }
  else if(((P6IN & BIT1)==0) && ((P6IN & BIT2)&&(P6IN & BIT3)))//���� ʶ����Ȧ
  {front();qian(pwm4_go);cn=0;} 
  else if((( (P6IN & BIT1) || (P6IN & BIT2))== 0) && (P6IN & BIT3))//��6.3��
  {
    while((P6IN & BIT2)==0)//�������ʱ������
    {
      left(); 
      zuo(pwm4_H,pwm4_L);
      cn=0;
    }
    //left(); zuo(pwm1_H,pwm1_L);   cn=0;delay_nms(120);
  }
  else if((( (P6IN & BIT2) || (P6IN & BIT3))== 0) && (P6IN & BIT1))//��6.1��
  {right();you(pwm4_H,pwm4_L);   cn=0;delay_nms(10);}
  else if((((P6IN & BIT1)  && (P6IN & BIT3))== 0) && (P6IN & BIT2))//��6.2�� 
  {front();qian(pwm4_go);cn=0;} 
  else 
  {front();qian(pwm4_go);}
}
void main(void)
{
  
  
  WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
  //initClockTo16MHz();
  OLED_Init();//oled��ʼ��
  OLED_Clear();//oled��Ļ����
  others_init();//�ƺͷ�������ʼ��
  interrupt_two_key();//����������ʼ��
  PWM0_Config();//2.0 pwm����
  PWM1_Config();//1.3 pwm����
  PWM2_Config();//1.4 pwm����
  PWM3_Config();//1.5
  PWM0_State(100);
  PWM1_State(100);
  PWM2_State(100);
  PWM3_State(100);
  USART0_Config();//���ڣ���������ʼ��
  catinit();//С����ʼ��
  hcsr04_init();
  clock_init();//��ʱ����ʼ��
  //OLED_ShowString(0,4,"Mode",16);
  __bis_SR_register(GIE);
  cycle=0;
  while(mode==0);
  while(1)
  {
    //���Ǵӻ���
    while(mode==1)
    {
      //OLED_ShowString(0,5,"1",16);
      if(flag=='a')
      {
        //������
        //OLED_ShowString(2,4,"go",16);
        //��С��һ�� ����� �ӻ�����Ҫ
//        front();
//        qian();
        delay_nms(600);
        while(1)
        {
          xunji_1();
          if(flag=='0')
          {
            stop();
            P8OUT &=~ BIT1;
            break;
          }
        }
        while(flag!='a')
        {
          stop();
          P8OUT &=~ BIT1;
        };
        //��⵽ֹͣλ����������
//        P8OUT |=BIT1;
//        delay_nms(100);
//        P8OUT &=~ BIT1;
        //OLED_ShowString(2,4,"stop",16);
        //OLED_ShowNum(4,6,cycle,1,16);
      }
    }
    //���Ǵӻ���
    while(mode==2)
    {
      //OLED_ShowString(0,5,"2",16);
      if(flag=='b')
      {
        //������
        //OLED_ShowString(2,4,"go",16);
        //��С��һ�� ����� �ӻ�����Ҫ
//        front();
//        qian();
//        delay_nms(300);
        while(1)
        {
          xunji_2();
          if(flag=='0')
          {
            stop();
            P8OUT &=~ BIT1;
            break;
          }
        }
        
        //��⵽ֹͣλ����������

        //OLED_ShowString(2,4,"stop",16);
        //OLED_ShowNum(4,6,cycle,1,16);
        while(flag!='b')
        {
          stop();
          P8OUT &=~ BIT1;
        }
      }
    }
    while(mode==4)
    {
      //OLED_ShowString(0,5,"1",16);
      if(flag=='d')
      {
        //������
        //OLED_ShowString(2,4,"go",16);
        //��С��һ�� ����� �ӻ�����Ҫ
//        front();
//        qian();
        delay_nms(600);
        while(1)
        {
          xunji_4();
          if(flag=='0')
          {
            stop();
            break;
          }
        }
        while(flag!='d')
        {
          stop();
          P8OUT &=~ BIT1;
        }
        //OLED_ShowString(2,4,"stop",16);
        //OLED_ShowNum(4,6,cycle,1,16);
      }
    }
    
  }
}



//#pragma vector = PORT1_VECTOR    //S1�жϷ�����
//__interrupt void P1_ISR()
//{
//  
//  if(mode==1)
//    flag='1';
//  else if(mode==2)
//    flag='2';
//  else if(mode==3)
//    flag='3';
//  else if(mode==4)
//    flag='4';
//  delay_nms(500);
//  
//  P1IFG &=~ BIT1;
//        
//}
//#pragma vector = PORT2_VECTOR    //S1�жϷ�����
//__interrupt void P2_ISR()
//{
//  delay_nms(50);
//  if((P2IN & BIT1)==0) 
//      mode+=1;
//
//    mode%=5;
//  P2IFG &=~ BIT1;
//}

#pragma vector=USCI_A0_VECTOR  // �����ж�������ַ
__interrupt void USCI_A0_ISR(void)
{
  static char Buff = 0;
  while (!(UCA0IFG&UCRXIFG));
  Buff = UCA0RXBUF;
  if(Buff=='s')
  {
    if(mode==1)
      delay_nms(280);
    if(mode==4)
      delay_nms(200);
    flag='0';
    stop();

    //while(1);
  }
  else
    flag=Buff;
  if(flag=='a')mode=1;
  else if(flag=='b')mode=2;
  else if(flag=='c')mode=3;
  else if(flag=='d')mode=4;
  OLED_ShowChar(0,2,flag,16);
  OLED_ShowNum(0,4,mode,1,16);
}


//#pragma vector=TIMER0_B0_VECTOR
#pragma vector = TIMER0_B0_VECTOR
__interrupt void func()
{
  _EINT();
  //P1OUT ^= BIT0;
  triggerMeasurement();
  
  // Wait for echo start
  __low_power_mode_3();
  
  lastCount = TA2CCR1;
  
  // Wait for echo end
  __low_power_mode_3();
  
  distance = TA2CCR1 - lastCount;
  distance *= 34000;
  distance >>= 14;  // division by 16384 (2 ^ 14)
  
  if (distance >= 4&&distance <= 15)
  {
    // Turn on LED
    pwm1_go=23;
    pwm2_L=50;
    pwm2_H=50;
    pwm2_go=34;
    pwm2_delay=60;
    pwm4_go=90;
    
//        delay_nms(100);
//        P8OUT &=~ BIT1;

  }
  else if (distance >= 25&&distance <= 32)
  {
    // Turn off LED
    pwm1_go=29;
    pwm2_L=50;
    pwm2_H=50;
    pwm2_go=40;
    pwm2_delay=60;
    pwm4_go=100;
    
//        delay_nms(100);
//        P8OUT &=~ BIT1;

  }
  else if(distance >= 16&&distance <= 24)
  {
    pwm1_go=26;
    pwm2_L=50;
    pwm2_H=50;
    pwm2_go=36;
    pwm2_delay=60;
    pwm4_go=80;
    
//        delay_nms(100);
//        P8OUT &=~ BIT1;
  }
  if((pwm1_go==25||(pwm2_go==35||pwm2_go==40))&&distance>=25)
  {
    pwm1_go=29;
    pwm2_L=50;
    pwm2_H=50;
    pwm2_go=40;
    pwm2_delay=60;
    pwm4_go=100;
    
  }
  if((pwm1_go==30||(pwm2_go==40||pwm2_go==44))&&distance<=15)
  {
    pwm1_go=23;
    pwm2_L=50;
    pwm2_H=50;
    pwm2_go=34;
    pwm2_delay=60;
    pwm4_go=80;
    
  }
  // Wait for the next measure interval tick
  //__low_power_mode_3();
}
#pragma vector = TIMER2_A0_VECTOR
__interrupt void TIMER2_A0_ISR (void) {
	// Measure interval tick
	__low_power_mode_off_on_exit();
	TA2CCR0 += MEASURE_INTERVAL;
}

#pragma vector = TIMER2_A1_VECTOR
__interrupt void TIMER2_A1_ISR (void) {
	// Echo pin state toggled
	__low_power_mode_off_on_exit();
	TA2IV = 0;
}