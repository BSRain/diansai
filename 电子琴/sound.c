#include "msp430.h"
#include "sound.h"
void P_Onclick(int n,int j)
{
  P8DIR |= BIT1;
                        ///��ʱÿ��������ʱ��
                      for ( ;j>0;j--)                                  
                      {
                        volatile unsigned int i;                
                        P8OUT ^= BIT1;                          
                        i = n;                             
                        do
                        {
                            i--;                                // �Լ�����
                            _NOP();                             // �ղ���
                        }while (i != 0);                        // ��׼��do����while��䣬ע���©�������ź���ķֺ�
                      }
  P8OUT &= ~BIT1;
}
// do
void jby_1(int s)   
{
    P_Onclick(95,s);
}
//re
void jby_2(int s)  
{
    P_Onclick(85,s);
}

//mi
void jby_3(int s)   
{
    P_Onclick(75,s);
}

//fa
void jby_4(int s)  
{
    P_Onclick(71,s);
}

//sol
void jby_5(int s)   
{
    P_Onclick(63,s);
}

//la
void jby_6(int s)  
{
    P_Onclick(56,s);
}

//si
void jby_7(int s)    
{
    P_Onclick(50,s);
}
