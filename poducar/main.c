/*
* maker:��С��
*
* time:2021 7.22
*
* */
/*
* ������������ѭ��ģʽ��һ����ȫ����ת��ѭ��ģʽ��һ����ȫ���ת��ģʽ����������ģʽ�����Թ������ת��ģʽЧ���ȽϺ�
*
*
* Ӳ����һ��msp4305529���ӣ���·����Թ�ѭ��ģ�飬ֱ������С�����СĴָ��ô�����֣���һ��tb6612�������ģ�飨����8V����������ѹģ�飨һ����12v��ѹ��5V����Ƭ��ʹ�ã���һ����ѹ��8V��TB6612ʹ�ã�����͵�Ƭ���ֵ�Դ��
* ��һ��12V�ĵ�ذ���һ��15kg���ֶ�������ڶ��ת�򣩣�һ��oled��ʾ����IICͨ�ţ�
* һ��mpu6050(������Ӧ��ͬ��б�Ƕȵ��µ�)�������£�
* ����ʵ����Է��֣������µ���б�Ƕȵ����ӣ�С����ת90�ȴ���ʱ����Ҫ��ǰ��ת�����ԽС�����Ա��������С����ʻǰ��Ҳ���ǰ�������������ʵ�顢�����������Ǽ��
* ���ӵ���б�Ƕȣ��Ӷ��޸����ת������ת�Ƕȡ�
*
*
*
*
* ������Ϊǰ�������������������������ұ��������Ҳ������û��ʹ�ñ�����������Ϊ��������Ϊ���ٵ���������ٶȱȽ��ȶ���ͨ���򵥵ĵ���pwm�Ϳ��ԱȽϾ�׼�ص����ٶ�
* �����ܹ���40�ȵ���
*
*
* ���ߣ�
* 4.1 4.2 mpu6050�����ߣ�6.0 6.1 6.3 6.4 ��ѭ��ģ�飻3.5 3.6 oled�����ߣ�2.0����ź��ߣ�1.2 1.3 tb6612pwm, 2.5 2.4 1.5 4.3 tb6612����ת������
*
*
* �����κ��ɻ������ϵ���ˣ�qq 1147176261 ��ӭһ��������
*
*/


#include "stdio.h"
#include "MPU6050.h"
#include <msp430f5529.h>
#include "Black_All.h"
#include "Black_OLED.h"
#include "PID.h"
int i=10,a=0,sight=0;
int pwm0=16;//16
int pwm1=30;
float pwmgo=12.24;//12.24
//int pwm0_0=20;
//int pwm0_1=40;//����Ϳ��ٶȲ���
int line=0;
int rightsight=0;
int leftsight=0;//�����ת���־
int duojidafuduzuo=100,duojidafuduyou=100;//���ת�����ȳ̶�
int cnt1=0;//��¼����������
int cnt2=0;
extern float v1,v2,Target;

int cal(int time)
{
  int a=100/time;
  double b=81.64/a;
  return (int)((100/b)/1.5);
}
void initClockTo16MHz()
{
  UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO
  __bis_SR_register(SCG0);                  // Disable the FLL control loop
  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
  UCSCTL1 = DCORSEL_5;                      // Select DCO range 16MHz operation
  UCSCTL2 = FLLD_0 + 487;                   // Set DCO Multiplier for 16MHz
  // (N + 1) * FLLRef = Fdco
  // (487 + 1) * 32768 = 16MHz
  // Set FLL Div = fDCOCLK
  __bic_SR_register(SCG0);                  // Enable the FLL control loop
  
  // Worst-case settling time for the DCO when the DCO range bits have been
  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
  // UG for optimization.
  // 32 x 32 x 16 MHz / 32,768 Hz = 500000 = MCLK cycles for DCO to settle
  __delay_cycles(500000);//
  // Loop until XT1,XT2 & DCO fault flag is cleared
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG); // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                          // Clear fault flags
  }while (SFRIFG1&OFIFG);                         // Test oscillator fault flag
}
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
void you(void)
{
  
  PWM0_State(pwm1);
  PWM1_State(pwm0);
  PWM2_State(pwm1);
  PWM3_State(pwm0);
}

void zuo(void)
{
  
  PWM0_State(pwm0);
  PWM1_State(pwm1);
  PWM2_State(pwm0);
  PWM3_State(pwm1);
}
void qian(void)
{
  PWM0_State(pwmgo);
  PWM1_State(pwmgo);
  PWM2_State(pwmgo);
  PWM3_State(pwmgo);
}
void ting(void)
{
  PWM0_State(0);
  PWM1_State(0);
  PWM2_State(0);
  PWM3_State(0);
}
void quansujianjin(void)
{PWM0_State(100);
PWM1_State(100);
PWM2_State(100);
PWM3_State(100);
}
void xunji_0(void)//ȫ����ѭ��
{
  if(((P6IN & BIT0) || (P6IN & BIT3)) && ((P6IN & BIT4) && (P6IN & BIT1)))//ȫ1 ȫ����
  {go();ting();a=0;}               
  //else if( (((P6IN & BIT0) || (P6IN & BIT1) || (P6IN & BIT3) )== 0) && (P6IN & BIT4))//1����һ��
  //{left();zuo();}//l����
  else if( ((P6IN & BIT1 || P6IN & BIT3 || P6IN & BIT4 )== 0) && (P6IN & BIT0))//1����һ��
  {right();you();delay(100);}
  else if((((P6IN & BIT0) || (P6IN & BIT1) || (P6IN & BIT3) || (P6IN & BIT4)) == 0)||(P6IN & BIT1)||(P6IN & BIT3))//ȫ0 ȫ��
  {go();qian();} 
  //else if( (((P6IN & BIT0) || (P6IN & BIT1) )== 0) && (P6IN & BIT4))//1������һ��
  //{right();PWM0_State(pwm0_0);PWM1_State(pwm0_1);delay(300);}
  //else if( (((P6IN & BIT3) || (P6IN & BIT4) )== 0) && (P6IN & BIT0))//1������һ��
  //{right();you();PWM0_State(pwm0_1);PWM1_State(pwm0_0);PWM2_State(pwm0_1);PWM3_State(pwm0_0);delay(500);}
  else ting();
}



/*
* û�м�����ʱ������ƽ״̬Ϊ0����⵽���߷�֮
* �м�����������ת���ұ�������ұ�ת
* ���Ա��������������߼�⵽�ͰѶ���������ֱ���м��ұ߼�⵽����Ϊֹ��������ұ߼�⵽��֮
*
*/
//void xunji(void)//ȫ���ת��ѭ��
//{
//  if((P6IN & BIT1) && (P6IN & BIT3) )//�м�����1 �м���������ͣ
//  {
//    ting();
//    a=0;
//    TimerA1_1_SetPWM(20000*1.4/20);//���ǰ
//  }
//  else if((((P6IN & BIT0) || (P6IN & BIT1) || (P6IN & BIT3) || (P6IN & BIT4)) == 0)&&((!rightsight)&(!leftsight)))//ȫ0 ȫ��û�����ת���־ ��
//  {
//    qian();
//    TimerA1_1_SetPWM(20000*1.4/20);//���ǰ
//  }
//  else if( (((P6IN & BIT0) || (P6IN & BIT1) || (P6IN & BIT4) )== 0) && (P6IN & BIT3) )//1����һ��
//  {
//    if(rightsight ==0)
//      TimerA1_1_SetPWM(20000*1.7/20);//�����
//    delay(50);
//    leftsight=0;//���־����
//  }
//  else if( ((P6IN & BIT0 || P6IN & BIT3 || P6IN & BIT4 )== 0) && (P6IN & BIT1))//1����һ��
//  {
//    if(leftsight ==0)
//      TimerA1_1_SetPWM(20000*1.1/20);//�����
//    delay(50);
//    rightsight=0;//�ұ�־����
//  }
//  else if( (((P6IN & BIT0) || (P6IN & BIT1) )== 0) && (P6IN & BIT4))//1������һ��
//  {
//    TimerA1_1_SetPWM(2100-10*duojidafuduyou);//����������(1000*(2.0-0.7*duojidafuduyou/100))
//    rightsight=1;
//  }
//  else if( (((P6IN & BIT3) || (P6IN & BIT4) )== 0) && (P6IN & BIT0))//1������һ��
//  {
//    TimerA1_1_SetPWM(500+9*duojidafuduyou);//����������20000*0.5*duojidafuduyou/(20*100)=1000*��0.5+0.9*duojidafuduyou/100��//
//    leftsight=1;
//  }
//  else
//    ;
//}
void interrupt_two_key(void)
{
  P1DIR |= BIT0;
  P4DIR |= BIT7;            //P1.0��P4.7������Ϊ���
  P1OUT &= ~BIT0;
  P4OUT |= BIT7;          //��ʼ��P1.0,P4.7����͵�ƽ����ʼ��LED��Ϊ��
  
  P1DIR &= ~BIT1;
  P2DIR &= ~BIT1;          //����P1.1,P2.1��Ϊ����
  
  P1REN |= BIT1;
  P2REN |= BIT1;
  
  P1OUT |= BIT1;
  P2OUT |= BIT1;          //����P1.1,P2.1��������
  
  //  P1IES |= BIT1;
  //  P2IES |= BIT1;          //P1.1,P2.1�½��ش����ж�
  //  
  //  P1IE |= BIT1;
  //  P2IE |= BIT1;           //P1.1,P2.1�ж�ʹ��
  //  
  //  P1IFG &= ~BIT1;
  //  P2IFG &= ~BIT1;        //����жϱ�־λ
  
  //_EINT();      //��ȫ���ж������ж�Ƕ��
}
void ceshu_init()
{
  P1IE |= BIT6;
  
  P1IES |= BIT6;
  P1IES &= ~BIT6;
  P1IFG &= ~BIT6;
  P1REN |=  BIT6;
  P1OUT |=  BIT6;
  
  P2IE |= BIT7;
  P2IES |= BIT7;
  P2IES &= ~BIT7;
  P2IFG &= ~BIT7;
  P2REN |=  BIT7;
  P2OUT |=  BIT7;
}

void main(void)
{
//  int k=0;
//  int result,ax,ay,az,gx,gy,gz;
  //OLED_Init();//oled��ʼ��
  //    P6DIR |= BIT3+BIT4;
  WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
  //initClockTo16MHz();
//  P1DIR|=0x01;
//  //_EINT();//�����жϣ������൱��GIE
//  TA1CCTL0=CCIE;
//  //�����Ƚ��жϣ��Ƚ�/�����ж�ʹ�ܿ���λ�����Ժ�����ʹ�ö�ʱ������жϻ��ǱȽϲ����жϣ������������жϿ���λ��
//  TA1CTL=TASSEL0+MC_1+TACLR;//TASSEL0�ο�ʱ��ACLK��������ģʽ��Ԥ�����TAR
//  TA1CCR0=32767;//�жϷ������ִ�е�ʱ������Լ1s
  
  
//  OLED_Clear();//oled��Ļ����
//  interrupt_two_key();//����������ʼ��
//  ceshu_init();//��������ʼ��
//  __bis_SR_register( GIE); 
  catinit();//С����ʼ��
  PWM0_Config();//1.2 pwm����
  PWM1_Config();//1.3 pwm����
  PWM2_Config();//1.4 pwm����
  PWM3_Config();//1.5
  PWM0_State(100);
  PWM1_State(100);
  PWM2_State(100);
  PWM3_State(100);
    
  //TimerA1_1_PWM_Init(100,20000);//���
  
  //        TimerA1_1_SetPWM(20000*0.5/20);//�����
  //        TimerA1_1_SetPWM(20000*1.43/20);//���ǰ
  //        TimerA1_1_SetPWM(20000*2.5/20);//�����

  int time=10;
  //OLED_ShowNum(10,0,Get6050Data(ACCEL_XOUT_H),10,16);//��ʾ��ʼ�Ƕ�
  //duojidafuduyou=(GetData(ACCEL_XOUT_H)*50/13000);//�����ǵ�������޶��ת��
  while(1)
  {
    int n=0;
    if((P1IN&BIT1)==0)
    {
      for(int j=0;j<=1000;j++)       //P1.1Ϊ����״̬����
      {
        if((P1IN&BIT1)==0)
          n++;
      }
      P4OUT^=BIT7;
      if(n>=900) 
      {
        if(sight==0)//�˵�ѡ��
        {
          sight=1;
          OLED_ShowString(0,4,"time:    ",16);
        }
        else if(sight==1)
        {
          sight=0;
          OLED_ShowString(0,4,"pleass:   ",16);
        }
      }
    }
    if(((P2IN&BIT1)==0)&&(sight==0))
    {
      n=0;
      for(int j=0;j<=1000;j++)   //�������������ܣ����1000�Σ���<900�Ĺ��˵������ڰ�������������ë�̸��ţ�
      {
        if((P2IN&BIT1)==0)
          n++;
      }
      P1OUT^=BIT0;
      if(n>=900)
      {
        OLED_ShowString(0,4,"go      ",16);//����
        a=1;
        rightsight=0;//�ұ�־����
        leftsight=0;//���־����
        //x=Get6050Data(ACCEL_XOUT_H);//����ʼ���ӽǶ�
        //OLED_ShowNum(10,0,x,10,16);
        //duojidafuduyou=(x*50/13000);//�����ǵ�������޶��ת��
        break;
      }
    }
    else if(((P2IN&BIT1)==0)&&(sight==1))
    {
      for(int j=0;j<=1000;j++)   //�������������ܣ����1000�Σ���<900�Ĺ��˵������ڰ�������������ë�̸��ţ�
      {
        if((P2IN&BIT1)==0)
          n++;
      }
      P1OUT|=BIT0;
      if(n>=900)
      {
        if(i<20) i++,time++;
        else i=10;
        OLED_ShowString(0,4,"time:",16);
        switch(i)//�������
        {
        case 10 : pwmgo=12.24;OLED_ShowNum(40,4,i,8,16);Target=10.0; break;
        case 11 : pwmgo=11.12;OLED_ShowNum(40,4,i,8,16);Target=9.09; break;
        case 12 : pwmgo=10.2;OLED_ShowNum(40,4,i,8,16);Target=8.33;break;
        case 13 : pwmgo=12.24/1.3;OLED_ShowNum(40,4,i,8,16);Target=7.69; break;
        case 14 : pwmgo=12.24/1.4;OLED_ShowNum(40,4,i,8,16);Target=7.14; break;
        case 15 : pwmgo=12.24/1.5;OLED_ShowNum(40,4,i,8,16);Target=6.66;break;
        case 16 : pwmgo=12.24/1.6;OLED_ShowNum(40,4,i,8,16);Target=6.25; break;
        case 17 : pwmgo=12.24/1.7;OLED_ShowNum(40,4,i,8,16);Target=5.88; break;
        case 18 : pwmgo=12.24/1.8;OLED_ShowNum(40,4,i,8,16);Target=5.56; break;
        case 19 : pwmgo=12.24/1.9;OLED_ShowNum(40,4,i,8,16);Target=5.26;break;
        case 20 : pwmgo=12.24/2.0;OLED_ShowNum(40,4,i,8,16);Target=5.0; break;
        }
      }
    }
    
  }
  go();
  delay_nus(1000);
  while(1)
  {
    go();
    //qian();
    if(a==1)
    {
      //xunji();//ȫ���ת��ѭ��
      xunji_0();//ȫ����ѭ��
    }
  }
}



#pragma vector = PORT1_VECTOR    //S1�жϷ�����
__interrupt void P1_ISR()
{
  
  cnt1++;
  
  P1IFG &=~ BIT6;
}
#pragma vector = PORT2_VECTOR    //S1�жϷ�����
__interrupt void P2_ISR()
{
  
  cnt2++;
  
  P2IFG &=~ BIT7;
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
  v1=cnt1*3.14/20*6.5;//cm/s
  v2=cnt2*3.14/20*6.5; 

  
  cnt1=0;
  
  cnt2=0;
}

