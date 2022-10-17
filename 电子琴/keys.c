#include <stdio.h>
#include <msp430.h>
#include "cryfucns.h"
#include "keys.h"
//���� f5529
/*
  ��p1.4~1.5andP2.4~p2.5 
  ��p6.0~p6.3 
*/

//int result=0;
//int row=0;
//int flag,flag1,flag2,flag3;
//unsigned int Push_Key=0;

char test1;
/*
  ����ɨ�跨
  ��ʼ��
*/
void keyboard_init()
{
    //�����йܽ�Ϊ����
    P3DIR &= ~BIT5; //P3.5�˿���Ϊ����
    P3REN |= BIT5;  //ʹ3.5������/��������
    P3OUT &=~ BIT5;  //��P3.5Ϊ��������ģʽ
    P3SEL &= ~BIT5; //P3.5ΪI/O�ӿ�

    P3DIR &= ~BIT6; //P3.6�˿���Ϊ����
    P3REN |= BIT6;  //ʹP3.6������/��������
    P3OUT &=~ BIT6;  //��P3.6Ϊ��������ģʽ
    P3SEL &= ~BIT6; //P3.6ΪI/O�ӿ�

    P7DIR &= ~BIT0; //P7.0�˿���Ϊ����
    P7REN |= BIT0;  //ʹP7.0������/��������
    P7OUT &=~ BIT0;  //��P7.0Ϊ��������ģʽ
    P7SEL &= ~BIT0; //P7.0ΪI/O�ӿ�

    P6DIR &= ~BIT4; //P6.4�˿���Ϊ����
    P6REN |= BIT4;  //ʹP6.4������/��������
    P6OUT &=~ BIT4;  //��P6.4Ϊ��������ģʽ
    P6SEL &=~ BIT4; //P6.4ΪI/O�ӿ�



    //�����йܽ�Ϊ���
    P6DIR |= BIT3+BIT2+BIT1+BIT0; //P6.3,2,1,0�˿���Ϊ���
    P6OUT &=~(BIT3+BIT2+BIT1+BIT0);

}

char scanKey(){
    //ɨ����
    char key_value=0;
    unsigned char state=0;
    do{
        key_value=0;
        if(state==0){
            P6OUT |= BIT0;
            P6OUT &=~ (BIT1+BIT2+BIT3);

        }else if(state==1){
            P6OUT |= BIT1;
            P6OUT &=~ (BIT0+BIT2+BIT3);

        }else if(state==2){
            P6OUT |= BIT2;
            P6OUT &=~ (BIT0+BIT1+BIT3);

        }else if(state==3){
            P6OUT |= BIT3;
            P6OUT &=~ (BIT0+BIT1+BIT2);

        }



        if(++state==4) state=0;


        if(P6OUT & BIT0)
            key_value = BIT4;

        if(P6OUT & BIT1)
            key_value = BIT5;

        if(P6OUT & BIT2)
            key_value = BIT6;

        if(P6OUT & BIT3)
            key_value = BIT7;


        if(P3IN & BIT5) key_value |= BIT0;
        else key_value &=~ BIT0;

        if(P3IN & BIT6) key_value |= BIT1;
        else key_value &=~ BIT1;

        if(P7IN & BIT0) key_value |= BIT2;
        else key_value &=~ BIT2;

        if(P6IN & BIT4) key_value |= BIT3;
        else key_value &=~ BIT3;



    }while((key_value&0x0f)==0);//ֱ�����б�����
    //__delay_cycles(1000);
    switch(key_value)
    {//���ذ��µ��ַ�
        case 129: return '1';   //s1
        case 65: return '4';    //s5
        case 33: return '7';    //s8
        case 17: return '*';    //s13
        case 130: return '2';   //s2
        case 66: return '5';    //s6
        case 34: return '8';    //s10
        case 18: return '0';    //s14
        case 132: return '3';   //s3
        case 68: return '6';    //s7
        case 36: return '9';    //s11
        case 20: return '#';    //s15
        case 136: return 'A';   //s4
        case 72: return 'B';    //s8
        case 40: return 'C';    //s12
        case 24: return 'D';    //s16
        default: return 0;
    }
}


/*#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
	if(P1IFG & BIT3)//�ж��Ƿ���P1.3�����ж�
	{
		P1IFG &= ~BIT3;//�����־λ
		while(1){P1OUT ^= BIT6;
                __delay_cycles(100000);
	}
        }
}*/

