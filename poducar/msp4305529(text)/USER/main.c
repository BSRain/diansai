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
#include "GO/GY86.h"
#include   <msp430.h>
#include "Black_All.h"
#include "GO/Black_OLED.h"
#include <msp430f5529.h>

int i=10,a=1,sight=0;
int pwm0=30;pwm1=100;pwmgo=70;pwm0_0=40;pwm0_1=100;//����Ϳ��ٶȲ���
int line=0;
int rightsight=0;leftsight=0;//�����ת���־
int duojidafuduzuo=100,duojidafuduyou=100;//���ת�����ȳ̶�

void catinit(void)
{
    OUTPUT_GPIO_Config(4,3);
    OUTPUT_GPIO_Config(1,5);
    OUTPUT_GPIO_Config(2,4);
    OUTPUT_GPIO_Config(2,5);

    INPUT_GPIO_Config(6,0);
    INPUT_GPIO_Config(6,1);
    INPUT_GPIO_Config(6,3);
    INPUT_GPIO_Config(6,4);
}
void go(void)
{
    OUTPUT_GPIO_State(4,3,1);
    OUTPUT_GPIO_State(1,5,0);
    OUTPUT_GPIO_State(2,4,1);
    OUTPUT_GPIO_State(2,5,0);
}

void left(void)
{
    OUTPUT_GPIO_State(4,3,1);
    OUTPUT_GPIO_State(1,5,0);
    OUTPUT_GPIO_State(2,4,0);
    OUTPUT_GPIO_State(2,5,1);
}
void right(void)
{
    OUTPUT_GPIO_State(4,3,0);
    OUTPUT_GPIO_State(1,5,1);
    OUTPUT_GPIO_State(2,4,1);
    OUTPUT_GPIO_State(2,5,0);
}
void down(void)
{
    OUTPUT_GPIO_State(4,3,0);
    OUTPUT_GPIO_State(1,5,1);
    OUTPUT_GPIO_State(2,4,0);
    OUTPUT_GPIO_State(2,5,1);
}
void stop(void)
{
    OUTPUT_GPIO_State(4,3,0);
    OUTPUT_GPIO_State(1,5,0);
    OUTPUT_GPIO_State(2,4,0);
    OUTPUT_GPIO_State(2,5,0);
}
void you(void)
{
    int i=0;
    PWM0_State(pwm0);
    PWM1_State(pwm1);
}

void zuo(void)
{
    int i=0;
    PWM0_State(pwm1);
    PWM1_State(pwm0);
}
void qian(void)
{
    PWM0_State(pwmgo);
    PWM1_State(pwmgo);
}
void ting(void)
{
    PWM0_State(0);
    PWM1_State(0);
}
void xunji_0(viod)//ȫ����ѭ��
{
    if(((P6IN & BIT0) || (P6IN & BIT1)) && ((P6IN & BIT3) || (P6IN & BIT4)))//ȫ1 ȫ����
        {go();ting();a=0;}
    else if(((P6IN & BIT0) || (P6IN & BIT1) || (P6IN & BIT3) || (P6IN & BIT4)) == 0)//ȫ0 ȫ��
        {go();qian();}
    else if( (((P6IN & BIT0) || (P6IN & BIT1) || (P6IN & BIT4) )== 0) && (P6IN & BIT3))//1����һ��
        {go();you();}
    else if( ((P6IN & BIT0 || P6IN & BIT3 || P6IN & BIT4 )== 0) && (P6IN & BIT1))//1����һ��
        {go();zuo();}
    else if( (((P6IN & BIT0) || (P6IN & BIT1) )== 0) && (P6IN & BIT4))//1������һ��
        {right();PWM0_State(pwm0_0);PWM1_State(pwm0_1);delay(300);}
    else if( (((P6IN & BIT3) || (P6IN & BIT4) )== 0) && (P6IN & BIT0))//1������һ��
        {left();PWM0_State(pwm0_1);PWM1_State(pwm0_0);delay(300);}
    else ting();
}



/*
 * û�м�����ʱ������ƽ״̬Ϊ0����⵽���߷�֮
 * �м�����������ת���ұ�������ұ�ת
 * ���Ա��������������߼�⵽�ͰѶ���������ֱ���м��ұ߼�⵽����Ϊֹ��������ұ߼�⵽��֮
 *
 */
void xunji(viod)//ȫ���ת��ѭ��
{
    if((P6IN & BIT1) && (P6IN & BIT3) )//�м�����1 �м���������ͣ
        {
        ting();
        a=0;
        TimerA1_1_SetPWM(20000*1.4/20);//���ǰ
        }
    else if((((P6IN & BIT0) || (P6IN & BIT1) || (P6IN & BIT3) || (P6IN & BIT4)) == 0)&&((!rightsight)&(!leftsight)))//ȫ0 ȫ��û�����ת���־ ��
        {
        qian();
        TimerA1_1_SetPWM(20000*1.4/20);//���ǰ
        }
    else if( (((P6IN & BIT0) || (P6IN & BIT1) || (P6IN & BIT4) )== 0) && (P6IN & BIT3) )//1����һ��
        {
        if(rightsight ==0)
        TimerA1_1_SetPWM(20000*1.7/20);//�����
        delay(50);
        leftsight=0;//���־����
        }
    else if( ((P6IN & BIT0 || P6IN & BIT3 || P6IN & BIT4 )== 0) && (P6IN & BIT1))//1����һ��
        {
        if(leftsight ==0)
        TimerA1_1_SetPWM(20000*1.1/20);//�����
        delay(50);
        rightsight=0;//�ұ�־����
        }
    else if( (((P6IN & BIT0) || (P6IN & BIT1) )== 0) && (P6IN & BIT4))//1������һ��
        {
        TimerA1_1_SetPWM(2100-10*duojidafuduyou);//����������(1000*(2.0-0.7*duojidafuduyou/100))
        rightsight=1;
        }
    else if( (((P6IN & BIT3) || (P6IN & BIT4) )== 0) && (P6IN & BIT0))//1������һ��
        {
        TimerA1_1_SetPWM(500+9*duojidafuduyou);//����������20000*0.5*duojidafuduyou/(20*100)=1000*��0.5+0.9*duojidafuduyou/100��//
        leftsight=1;
        }
    else
        ;
}
void interrupt_two_key(void)
{
P1DIR |= BIT0;
P4DIR |= BIT7;            //P1.0��P4.7������Ϊ���
P1OUT &= ~BIT0;
P4OUT &= ~BIT7;          //��ʼ��P1.0,P4.7����͵�ƽ����ʼ��LED��Ϊ��

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

_EINT();      //��ȫ���ж������ж�Ƕ��
}

void main(void)
{
    int k=0;
    int result,ax,ay,az,gx,gy,gz;
    OLED_Init();//oled��ʼ��
//    P6DIR |= BIT3+BIT4;
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
/*    P1DIR|=0x01;
    _EINT();//�����жϣ������൱��GIE
    TA1CCTL0=CCIE;
//�����Ƚ��жϣ��Ƚ�/�����ж�ʹ�ܿ���λ�����Ժ�����ʹ�ö�ʱ������жϻ��ǱȽϲ����жϣ������������жϿ���λ��
    TA1CTL=TASSEL0+MC_1+TACLR;//TASSEL0�ο�ʱ��ACLK��������ģʽ��Ԥ�����TAR
    TA1CCR0=32768;//�жϷ������ִ�е�ʱ������Լ1s
    */

    OLED_Clear();//oled��Ļ����
    interrupt_two_key();//����������ʼ��

    PWM0_Config();//1.2 pwm����
    PWM1_Config();//1.3 pwm����
    PWM0_State(100);
    PWM1_State(100);

    TimerA1_1_PWM_Init(100,20000);//���
    catinit();//С����ʼ��

//        TimerA1_1_SetPWM(20000*0.5/20);//�����
//        TimerA1_1_SetPWM(20000*1.43/20);//���ǰ
//        TimerA1_1_SetPWM(20000*2.5/20);//�����
    InitMPU6050();

    OLED_ShowString(0,4,"stop",16);

    OLED_ShowNum(10,0,GetData(ACCEL_XOUT_H),10,16);//��ʾ��ʼ�Ƕ�
    duojidafuduyou=(GetData(ACCEL_XOUT_H)*50/13000);//�����ǵ�������޶��ת��


    go();
    while(1)
    {
        go();
        qian();
        //if(a==1)
        //{
        //xunji();//ȫ���ת��ѭ��
        //xunji_0(viod)//ȫ����ѭ��
        //}
    }
}


//P1���жϷ������
#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR()
{
    int n,j;

    if(P1IFG&BIT1)
    {
        for(j=0;j<=1000;j++)       //P1.1Ϊ����״̬����
        {
            if((P1IN&BIT1)==0)
            n++;
        }
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
    P1IFG &= ~BIT1;   //����жϱ�־
}

//P2���жϷ������
#pragma vector = PORT2_VECTOR
__interrupt void P2_ISR()
{
    int j,n,x;

    TA0CCTL0 = 0 ;   //�˳���P1.1�����Ķ�ʱ���ж�
    if(((P2IN&BIT1)==0)&&(sight==0))
    {
        for(j=0;j<=1000;j++)   //�������������ܣ����1000�Σ���<900�Ĺ��˵������ڰ�������������ë�̸��ţ�
        {
            if((P2IN&BIT1)==0)
                n++;
        }
        if(n>=900)
        {
            OLED_ShowString(0,4,"go      ",16);//����
            a=1;
            rightsight=0;//�ұ�־����
            leftsight=0;//���־����
            x=GetData(ACCEL_XOUT_H);//����ʼ���ӽǶ�
            OLED_ShowNum(10,0,x,10,16);
            duojidafuduyou=(x*50/13000);//�����ǵ�������޶��ת��
        }
    }
        else if(((P2IN&BIT1)==0)&&(sight==1))
        {
            for(j=0;j<=1000;j++)   //�������������ܣ����1000�Σ���<900�Ĺ��˵������ڰ�������������ë�̸��ţ�
            {
                if((P2IN&BIT1)==0)
                    n++;
            }
            if(n>=900)
            {
                if(i<20) i++;
                else i=10;
            OLED_ShowString(0,4,"time:",16);
            switch(i)//�������
            {
            case 10 : pwm0=30;pwm1=100;pwmgo=70;pwm0_0=40;pwm0_1=100;OLED_ShowNum(40,4,i,8,16); break;
            case 11 : pwm0=30;pwm1=100;pwmgo=66;pwm0_0=40;pwm0_1=100;OLED_ShowNum(40,4,i,8,16); break;
            case 12 : pwm0=30;pwm1=100;pwmgo=62;pwm0_0=40;pwm0_1=100;OLED_ShowNum(40,4,i,8,16); break;
            case 13 : pwm0=30;pwm1=100;pwmgo=58;pwm0_0=40;pwm0_1=100;OLED_ShowNum(40,4,i,8,16); break;
            case 14 : pwm0=30;pwm1=100;pwmgo=54;pwm0_0=40;pwm0_1=100;OLED_ShowNum(40,4,i,8,16); break;
            case 15 : pwm0=30;pwm1=100;pwmgo=50;pwm0_0=40;pwm0_1=100;OLED_ShowNum(40,4,i,8,16); break;
            case 16 : pwm0=30;pwm1=100;pwmgo=46;pwm0_0=40;pwm0_1=100;OLED_ShowNum(40,4,i,8,16); break;
            case 17 : pwm0=30;pwm1=100;pwmgo=42;pwm0_0=40;pwm0_1=100;OLED_ShowNum(40,4,i,8,16); break;
            case 18 : pwm0=30;pwm1=100;pwmgo=38;pwm0_0=40;pwm0_1=100;OLED_ShowNum(40,4,i,8,16); break;
            case 19 : pwm0=30;pwm1=100;pwmgo=34;pwm0_0=40;pwm0_1=100;OLED_ShowNum(40,4,i,8,16); break;
            case 20 : pwm0=30;pwm1=100;pwmgo=30;pwm0_0=40;pwm0_1=100;OLED_ShowNum(40,4,i,8,16);break;
            }
            }
        }


    P2IFG &= ~BIT1;//����жϱ�־λ
}

/*
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
    P1OUT^=0x01;
}
*/