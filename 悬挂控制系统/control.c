// control.c
#include <msp430f5529.h> 
#define CPU_F ((double)1035000)
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))//����
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))//΢��
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
  /*�߼���ƽ���Ƶ������ת*/
  P2DIR |=  BIT6;// + BIT4;
  P2OUT |=  BIT6;//��������
  //P1OUT &= ~BIT4;//�ӵ�
}
void init_two()
{
  P2DIR |= BIT2;
  /*�߼���ƽ���Ƶ������ת*/
  P2DIR |=  BIT3; //+ BIT4;
  P2OUT |=  BIT3;//��������
  //P1OUT &= ~BIT4;//�ӵ�
}
void cycle1_min_f(int time)//3.3 10mm
{
  P2OUT |=  BIT6;//��
  for( int x = 0; x <=34; x++) 
  { //һȦ200������    ����pulΪһ������    1���������1.8��
    P2OUT |= BIT0;
    Delay_nus(time/2);                //�޸�΢��ֵ���Ե���
    P2OUT &= ~BIT0;
    Delay_nus(time/2);
  }
}
void cycle1_min_s(int time)//3.3 10mm
{
  P2OUT &= ~ BIT6;//��
  for( int x = 0; x <=34; x++) 
  { //һȦ200������    ����pulΪһ������    1���������1.8��
    P2OUT |= BIT0;
    Delay_nus(time/2);                //�޸�΢��ֵ���Ե���
    P2OUT &= ~BIT0;
    Delay_nus(time/2);
  }
}
void cycle2_min_f(int time)//nΪangle
{
  P2OUT |=  BIT3;//��
  for( int x = 0; x <=34; x++) 
  { //һȦ200������    ����pulΪһ������    1���������1.8��
    P2OUT |= BIT2; 
    Delay_nus(time/2);               //�޸�΢��ֵ���Ե���
    P2OUT &= ~BIT2; 
    Delay_nus(time/2);
  }
}
void cycle2_min_s(int time)//nΪangle
{
  P2OUT &=~  BIT3;//��
  for( int x = 0; x <=34; x++) 
  { //һȦ200������    ����pulΪһ������    1���������1.8��
    P2OUT |= BIT2; 
    Delay_nus(time/2);               //�޸�΢��ֵ���Ե���
    P2OUT &= ~BIT2; 
    Delay_nus(time/2);
  }
}
void cycle1_choose_f(int n,int time)//nΪangle
{
  P2OUT |=  BIT6;//��
  for( int x = 0; x <=n; x++) 
  { //һȦ200������    ����pulΪһ������    1���������1.8��
    P2OUT |= BIT0; 
    Delay_nus(time/2);               //�޸�΢��ֵ���Ե���
    P2OUT &= ~BIT0; 
    Delay_nus(time/2);
  }
}
void cycle1_choose_b(int n,int time)//nΪangle
{
  P2OUT &= ~ BIT6;
  for( int x = 0; x <=n; x++) 
  { //һȦ200������    ����pulΪһ������    1���������1.8��
    P2OUT |= BIT0; 
    Delay_nus(time/2);                //�޸�΢��ֵ���Ե���
    P2OUT &= ~BIT0; 
    Delay_nus(time/2);                
  }
}
void cycle2_choose_f(int n,int time)//nΪangle
{
  P2OUT |=  BIT3;
  for( int x = 0; x <=n; x++) 
  { //һȦ200������    ����pulΪһ������    1���������1.8��
    P2OUT |= BIT2; 
    Delay_nus(time/2);               //�޸�΢��ֵ���Ե���
    P2OUT &= ~BIT2; 
    Delay_nus(time/2);
  }
}
void cycle2_choose_b(int n,int time)//nΪangle
{
  P2OUT &= ~ BIT3;
  for( int x = 0; x <=n; x++) 
  { //һȦ200������    ����pulΪһ������    1���������1.8��
    P2OUT |= BIT2; 
    Delay_nus(time/2);                //�޸�΢��ֵ���Ե���
    P2OUT &= ~BIT2; 
    Delay_nus(time/2);                
  }
}
void left(int n,int time)
{
  P2OUT &= ~ BIT6;
  P2OUT &= ~ BIT3;
  for( int x = 0; x <=n; x++) 
  { //һȦ200������    ����pulΪһ������    1���������1.8��
    P2OUT |= BIT0;
    P2OUT |= BIT2;
    Delay_nus(time/2);                //�޸�΢��ֵ���Ե���
    P2OUT &= ~BIT0;
    P2OUT &= ~BIT2;
    Delay_nus(time/2);                
  }
}
void right(int n,int time)
{
  P2OUT |=  BIT6;//�ұ���
  P2OUT |=  BIT3;//��߷�
  for( int x = 0; x <=n; x++) 
  { //һȦ200������    ����pulΪһ������    1���������1.8��
    P2OUT |= BIT0; 
    P2OUT |= BIT2; 
    Delay_nus(time/2);               //�޸�΢��ֵ���Ե���
    
    P2OUT &= ~BIT0; 
    P2OUT &= ~BIT2;
    Delay_nus(time/2);
  }
  
}
void up(int n,int time)
{
  P2OUT |=  BIT6;//��
  P2OUT &= ~ BIT3;//��
  for( int x = 0; x <=n; x++) 
  { //һȦ200������    ����pulΪһ������    1���������1.8��
    P2OUT |= BIT0; 
    P2OUT |= BIT2;
    Delay_nus(time/2);               //�޸�΢��ֵ���Ե���
    P2OUT &= ~BIT0; 
    P2OUT &= ~BIT2;
    Delay_nus(time/2);
  }
  
}
void down(int n,int time)
{
  P2OUT &=~  BIT6;//��
  P2OUT |=  BIT3;//��
  for( int x = 0; x <=n; x++) 
  { //һȦ200������    ����pulΪһ������    1���������1.8��
    P2OUT |= BIT0; 
    P2OUT |= BIT2;
    Delay_nus(time/2);               //�޸�΢��ֵ���Ե���
    P2OUT &= ~BIT0; 
    P2OUT &= ~BIT2;
    Delay_nus(time/2);
  }
}