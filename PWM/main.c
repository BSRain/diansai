#include <msp430f5529.h> 
#define CPU_F ((double)1035000)
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define _delay_cycles __delay_cycles
int sum=0;                                  //ȫ�ֱ���
int main(void)
{
  volatile unsigned int i;                  //�ֲ�����
                //����timerA��ز���
  WDTCTL = WDTPW+WDTHOLD;                   // �رտ��Ź�
  P1DIR |= BIT2;                            // ��P1.2����Ϊ����˿ڣ���֤״̬
  P2DIR |= BIT0;                            //P2.0���PWM��
  TA0CCTL1 = OUTMOD_7;                         //�����ģʽѡ��Ϊģʽ7
  TA0CTL = TASSEL_1 + MC_1 + TACLR ;         //ѡ��ACLKʱ��Դ��timerA����ģʽѡ��upģʽ��������TA0CCR0������timerA������
  TA1CCTL1 = OUTMOD_7;
  TA1CTL = TASSEL_1 + MC_1 +TACLR ;
  /*�߼���ƽ���Ƶ������ת*/
    P1DIR |=  BIT3 + BIT4;
    P1OUT &=~  BIT3;
    P1OUT &= ~BIT4;
    
    P1REN |= BIT1;                            // ʹ�ܶ˿�1.1
    P1OUT |= BIT1;                            // ѡ������ģʽ
    P1IES |= BIT1;                            // 1.1�˿��жϴ�����ѡ���½��أ��ߵ��ʹ���
    P1IFG &= ~BIT1;                           // 1.1�˿��жϼĴ�������
    P1IE |= BIT1;                       // 1.1�˿��ж�ʹ��

    __bis_SR_register(GIE);                     //��GIE��1����ȫ���ж�ʹ��
  
  while(1)                                       // ����switch��ÿһ��caseֵ��Ӧһ��״̬
  {
    switch (sum)
    {
    case 0:
    P1OUT &= ~BIT2;                              //��ʼ״̬Ϊ�͹���״̬
        TA0CCR1 = 0;
        __bis_SR_register(LPM3_bits);            //��LPM3��1��ʹϵͳ����͹���
        break;                                   //����switch����
    case 1:                                      //ѡϵͳ��Ĭ��ʱ�ӣ�Ƶ��Ϊ1MHZ��
//        P1OUT &= ~BIT2;                 //״̬1Ҫ��ת����60�㣬900΢�룬���õ�Ϊ1000΢�룬ʱ��Ƶ��Ϊ32768Hz
//        TA0CCR0 = 20000;          //��֤����״̬1
//        TA0CCR1 = 10000;
//        TA1CCR0 = 100;              //���PWM������
//        TA1CCR1 = 34;               //ռ�ձ�
//        P1DIR |=BIT2;
//        P1SEL |=BIT2;
//        P2DIR |=BIT0;
//        P2SEL |=BIT0;
        for( int x = 0; x <=10; x++) 
        { //һȦ200������    ����pulΪһ������    16���������28.8�����30����1.2��
         P2OUT |= BIT0; 
          delay_us(2500);                //�޸�΢��ֵ���Ե���
         P2OUT &= ~BIT0; 
         delay_us(17500);               
         }
        break;
    case 2:
        P1OUT &= ~BIT2;                 //״̬2Ҫ��ת��0�㡣1520΢��
        TA0CCR0 = 20000;
        TA0CCR1 = 5000;
        TA1CCR0 = 100;
        TA1CCR1 = 50;
        P1DIR |=BIT2;
        P1SEL |=BIT2;
        P2DIR |=BIT0;
        P2SEL |=BIT0;
        break;
    case 3:                     //ʱ��Ϊ����ʱ��ACLK��Ƶ��Ϊ32768HZ
    //P1OUT &= ~BIT2;                 //״̬3Ҫ��ת��60��   2100΢��
        TA0CCR0 = 20000;
        TA0CCR1 = 800;
        TA1CCR0 = 100;
        TA1CCR1 = 66;
        P1DIR |=BIT2;
        P1SEL |=BIT2;  //ѡ��1.2�˿ڵĸ��ù��ܣ�ʹ֮�ܹ�ʵ��������
        P2DIR |=BIT0;
        P2SEL |=BIT0;
        break;

    }
    if(sum>3)                     //��sum��ֵ����3ʱ��Ҫ��sum��3�����½���ѭ��
          {
            sum=sum-3;
          }
    }
}

//����S1�жϰ���ʵ��
#pragma vector=PORT1_VECTOR                //�˿�p1���жϺ���
__interrupt void Port_1(void)
{
    unsigned int push_key =0;
    push_key == P1IFG & (P1IN);
    __delay_cycles(20000);                  //��ȷʱ���ӳ٣�����MCLKʱ������,��ֹ����
    if( push_key == (P1IN & BIT1))
    {
    __low_power_mode_off_on_exit();        //SR�Ĵ�����0���˳��͹���
    sum=sum+1;                               // ����p1.1��sum��1�����ڱ仯һ��
    }
      P1IFG &= ~BIT1;                          // p1.1�ж�����
}