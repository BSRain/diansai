#include "sys.h"
#include "myControl.h"
#include "myGrayscale.h"
#include "myMOTOR.h"
#include "myPID.h"

#define L2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)   //�Ҷȴ������ú궨�����
#define L1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)
#define M GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)
#define R1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)
#define R2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)

#define LoadOrNot GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)   //���⴫���������ڼ��ҩƷ�Ƿ���º��ó�


int Encoder_timeSum1=0,Encoder_timeSum2=0;  //��ʱ��2����4�������ۼӣ����ڼ���·�̣����������������������ﵽ�����㣩
int Flag=1;    //�������־���ֶ��ߣ������߲���1��Flag=1ʱֱ��65cm��Ȼ���Flag��Ϊ2��Flag=2����ת��ת90��.....�������ƣ�
int DelayOrNot=1,DelayOrNot1=1;
extern int Times;       //�������������ʱ
extern char RxData;    //����ͷ�����������֣��ַ��ͣ�
char LeftOrRight='1';  //��ֵ����0��    '1'����ת     '2'����ת
int TimesOnOrOff=1;

extern int Position_1,Position_2;
extern float Targer;

int Position_1PWM,Position_2PWM;

/*---------------------------------------------------------------------------------------------------------------------------------*/
/*

ͣ����ʱ��
����һ������Ҫ��ʱ�ĺ�����

*/
void TIME_Delay_ms(int ms)
{
	if(TimesOnOrOff==1)  //��ʱ1S
	{
		TimesOnOrOff=0;
		Stop();
		Times=0;
		while(Times<=(ms/5));
		TimesOnOrOff=1;
	}
}
/*---------------------------------------------------------------------------------------------------------------------------------*/


/********************************>> 1�Ų��� <<***************************************************************************************************/
void Home1(void)
{
	/***************************>> 1 <<********************************************/
	
	if(Flag==1&&LoadOrNot==0)//��65cm
	{
		/******>>�����������ڷ�ҩ����ʱ500ms<<**********/
		Times=0;
		while(DelayOrNot==1&&Times<=200);
		DelayOrNot=0;
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-3450)
		{
			Front();
		}
		else
		{
			Flag=2;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
			
			TIME_Delay_ms(1000);
		}
	}

	/***************************>> 2 <<********************************************/
	
	if(Flag==2)//ת90��
	{
		Turn_Left_90du();
		
		Flag=3;	
		
		Times=0;
		while(Times<=50)	Front();
	}
	
	/***************************>> 3 <<********************************************/
	if(Flag==3)//��55cm
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-2350)
		{
			Front();
		}
		else
		{
			Flag=4;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
			
			Stop();
		}
	}
	
	while(Flag==4&&LoadOrNot==0);
	
	/***************************>> 4 <<********************************************/
	if(Flag==4)
	{
		Turn_180du();
		Times=0;
		while(Times<=50)	Front();
		
		Flag=5;
	}
	
	
	/***************************>> 5 <<********************************************/
	if(Flag==5)   //��55cm��
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-2300)
		{
			Front();
		}
		else
		{
			Flag=6;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
			
			TIME_Delay_ms(500);
		}
	}
	
	
	/***************************>> 6 <<********************************************/
	if(Flag==6)//��ת90��
	{	
		Turn_Right_90du();
		
		Flag=7;
		
		Times=0;
		while(Times<=50) Front();
	}
	
	/***************************>> 7 <<********************************************/
	if(Flag==7)//��65cm��
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-3220)
		{
			Front();
		}
		else
		{
			Flag=8;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
		} 
	}
	
	/***************************>> 8 <<********************************************/
	if(Flag==8)//ͣ��
	{
		Stop();
		while(1);
	}

	/*****************************>> �Ҷȴ������ֲ��������� <<******************************************/
	if(L1==1||L2==1)
	{
		Right();
	}
		
	if(R1==1||R2==1)
	{
		Left();
	}
}



///********************************>> 2�Ų��� <<***************************************************************************************************/
void Home2(void)
{
	/***************************>> 1 <<********************************************/
	
	if(Flag==1&&LoadOrNot==0)//��65cm
	{
		/******>>�����������ڷ�ҩ����ʱ500ms<<**********/
		Times=0;
		while(DelayOrNot==1&&Times<=200);
		DelayOrNot=0;
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-3450)
		{
			Front();
		}
		else
		{
			Flag=2;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
			
			TIME_Delay_ms(1000);
		}
	}

	/***************************>> 2 <<********************************************/
	
	if(Flag==2)//ת90��
	{
		Turn_Right_90du();
		
		Flag=3;	
		
		Times=0;
		while(Times<=50)	Front();
	}
	
	/***************************>> 3 <<********************************************/
	if(Flag==3)//��55cm
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-2350)
		{
			Front();
		}
		else
		{
			Flag=4;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
			
			Stop();
		}
	}
	
	while(Flag==4&&LoadOrNot==0);
	
	/***************************>> 4 <<********************************************/
	if(Flag==4)
	{
		Turn_180du();
		Times=0;
		while(Times<=50)	Front();
		
		Flag=5;
	}
	
	
	/***************************>> 5 <<********************************************/
	if(Flag==5)   //��55cm��
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-2300)
		{
			Front();
		}
		else
		{
			Flag=6;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
			
			TIME_Delay_ms(500);
		}
	}
	
	
	/***************************>> 6 <<********************************************/
	if(Flag==6)//��ת90��
	{	
		Turn_Left_90du();
		
		Flag=7;
		
		Times=0;
		while(Times<=50) Front();
	}
	
	/***************************>> 7 <<********************************************/
	if(Flag==7)//��65cm��
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-3220)
		{
			Front();
		}
		else
		{
			Flag=8;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
		} 
	}
	
	/***************************>> 8 <<********************************************/
	if(Flag==8)//ͣ��
	{
		Stop();
		while(1);
	}

	/*****************************>> �Ҷȴ������ֲ��������� <<******************************************/
	if(L1==1||L2==1)
	{
		Right();
	}
		
	if(R1==1||R2==1)
	{
		Left();
	}
}


///********************************>> �в����� <<***************************************************************************************************/
/*
�в�����ִ�в��裺
����60cm�رջҶȴ�����-->��ʮ��·��-->�򿪻Ҷȴ�������������λ�ã������ƫ������ˣ�-->ֱ��10cm

-->���ͳ�����10cm����������ͷʶ�����֣�-->��Ϊԭ��������40cm-->��������ͷ��������ֵ��ת������ת
*/
void Home_M(void)
{
	/***************************>> 1 <<********************************************/
	if(Flag==1&&LoadOrNot==0)//��60cm
	{
		/******>>�����������ڷ�ҩ����ʱ1000ms<<**********/
		Times=0;
		while(DelayOrNot==1&&Times<=200);
		DelayOrNot=0;
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-8750)  //4500��ʮ��·�ڣ�65cm��
		{
			Front();
			//if((Encoder_timeSum1+Encoder_timeSum2)/2>=2000) kaiguan=1;
		}
		else
		{
			Flag=2;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
		}
	}
	
	/***************************>> 2 <<********************************************/
	if(TimesOnOrOff==1&&Flag==2)
	{
		TimesOnOrOff=0;
		Stop();
		Times=0;
		while(Times<=200);
	}
	if(Flag==2)   //��ת90�������ת90��(Ҫ��ת������ת�ÿ�����ͷ��������ֵ)
	{
		if(RxData=='1')     //////////////////////////�ж�����ͷ�ڶ���ʶ��󴫻��������ݣ���1�����󡢡�2�����ң�
			{
				Position_1=0;
				Position_2=0;
				Times=0;
				while(Times<=250)		//3s
				{
					Position_1PWM=Position_control_left(Position_1,-800);
					Position_2PWM=Position_control_right(Position_2,800);
					PID_Position_left();
					PID_Position_right();
				}
			}
			else
			{
				Position_1=0;
				Position_2=0;
				Times=0;
				while(Times<=250)		//3s
				{
					Position_1PWM=Position_control_left(Position_1,900);
					Position_2PWM=Position_control_right(Position_2,-900);
					PID_Position_left();
					PID_Position_right();
				}
			}
		
			Flag=3;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
			TimesOnOrOff=1;
	}
	
	/***************************>> 3 <<********************************************/
	if(TimesOnOrOff==1&&Flag==3)
	{
		TimesOnOrOff=0;
		Stop();
		Times=0;
		while(Times<=200);
		Times=0;
		while(Times<=15) Front();
	}
	if(Flag==3)//��55cm
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-2300)
		{
			Front();
		}
		else
		{
			Flag=4;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
			Stop();     					//ͣ�����ȴ�ҩ����
		}
	}
	
	while(Flag==4&&LoadOrNot==0);   //�ȴ�ҩ����
	
	/***************************>> 4 <<********************************************/
	if(Flag==4)
	{
		Turn_180du();
		Times=0;
		while(Times<=30)	Front();
		
		Flag=5;
	}
	
	/***************************>> 5 <<********************************************/
	
	if(Flag==5)//��55cm��
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-2750)
		{
			Front();
		}
		else
		{
			Flag=6;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
		}
	}
	
	/***************************>> 6 <<********************************************/
	if(Flag==6)//���������ת90��
	{	
		if(RxData=='1')     //////////////////////////�ж�����ͷ�ڶ���ʶ��󴫻��������ݣ���1�����󡢡�2�����ң�
			{
				Position_1=0;
				Position_2=0;
				Times=0;
				while(Times<=250)		//3s
				{
					Position_1PWM=Position_control_left(Position_1,800);
					Position_2PWM=Position_control_right(Position_2,-800);
					PID_Position_left();
					PID_Position_right();
				}
			}
			else
			{
				Position_1=0;
				Position_2=0;
				Times=0;
				while(Times<=250)		//3s
				{
					Position_1PWM=Position_control_left(Position_1,-800);
					Position_2PWM=Position_control_right(Position_2,800);
					PID_Position_left();
					PID_Position_right();
				}
			}
		
			Flag=7;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
			TimesOnOrOff=1;
			Times=0;
			while(Times<=200);
	}
	
	/***************************>> 7 <<********************************************/
	if(Flag==7)//��80cm
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-8700)  //90cm��ʮ��·��
		{
			Front();
		}
		else
		{
			Flag=8;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
		}
	}
	
	/***************************>> 8 <<********************************************/
	if(Flag==8)//ͣ��
	{
		Stop();
		while(1);
	}

	/*****************************>> �Ҷȴ������ֲ��������� <<******************************************/
		if(L1==1)
		{
			Right();
		}
		
	if(R1==1)
		{
			Left();
		}
		
		if(L2==1)
		{
			Big_Right();
		}
		
	if(R2==1)
		{
			Big_Left();
		}
}


/********************************>> Զ�˲��� <<***************************************************************************************************/
/*
Զ�˲���ִ�в��裺
����60cm�رջҶȴ�����-->��ʮ��·��-->�򿪻Ҷȴ�������������λ�ã������ƫ������ˣ�-->ֱ��10cm

-->���ͳ�����10cm����������ͷʶ�����֣�-->��Ϊԭ��������40cm-->��������ͷ��������ֵ��ת������ת
*/
void Home_F(void) 
{
	/***************************>> 1 <<********************************************/
	if(Flag==1&&LoadOrNot==0)//��60cm
	{
		/******>>�����������ڷ�ҩ����ʱ1000ms<<**********/
		Times=0;
		while(DelayOrNot==1&&Times<=200);
		DelayOrNot=0;
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-13800)  //4500��ʮ��·�ڣ�65cm��
		{
			Front();
			//if((Encoder_timeSum1+Encoder_timeSum2)/2>=2000) kaiguan=1;
		}
		else
		{
			Flag=2;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
		}
	}
	
	/***************************>> 2 <<********************************************/
	if(TimesOnOrOff==1&&Flag==2)
	{
		TimesOnOrOff=0;
		Stop();
		Times=0;
		while(Times<=200);
	}
	if(Flag==2)   //��ת90�������ת90��(Ҫ��ת������ת�ÿ�����ͷ��������ֵ)
	{
		LeftOrRight=RxData;//���ڻ���ʱת����
		if(RxData=='1')     //////////////////////////�ж�����ͷ�ڶ���ʶ��󴫻��������ݣ���1�����󡢡�2�����ң�
			{
				Position_1=0;
				Position_2=0;
				Times=0;
				while(Times<=250)		//3s
				{
					Position_1PWM=Position_control_left(Position_1,-770);
					Position_2PWM=Position_control_right(Position_2,770);
					PID_Position_left();
					PID_Position_right();
				}
			}
			else
			{
				Position_1=0;
				Position_2=0;
				Times=0;
				while(Times<=250)		//3s
				{
					Position_1PWM=Position_control_left(Position_1,800);
					Position_2PWM=Position_control_right(Position_2,-800);
					PID_Position_left();
					PID_Position_right();
				}
			}
		
			Flag=3;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
			TimesOnOrOff=1;
	}
	
	/***************************>> 3 <<********************************************/
	if(TimesOnOrOff==1&&Flag==3)
	{
		TimesOnOrOff=0;
		Stop();
		Times=0;
		while(Times<=200);
		Times=0;
		while(Times<=10) Front();
	}
	if(Flag==3)//��55cm
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-5210)
		{
			Front();
		}
		else
		{
			Flag=4;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
			Stop();     					//
		}
	}
	
	/***************************>> 4 <<********************************************/
	if(TimesOnOrOff==1&&Flag==4)
	{
		TimesOnOrOff=0;
		Stop();
		Times=0;
		while(Times<=200);
	}
	if(Flag==4)   //��ת90�������ת90��(Ҫ��ת������ת�ÿ�����ͷ��������ֵ)
	{
		DelayOrNot1=RxData;
		if(RxData=='1')     //////////////////////////�ж�����ͷ�ڶ���ʶ��󴫻��������ݣ���1�����󡢡�2�����ң�
			{
				Position_1=0;
				Position_2=0;
				Times=0;
				while(Times<=250)		//3s
				{
					Position_1PWM=Position_control_left(Position_1,-780);
					Position_2PWM=Position_control_right(Position_2,780);
					PID_Position_left();
					PID_Position_right();
				}
			}
			else
			{
				Position_1=0;
				Position_2=0;
				Times=0;
				while(Times<=250)		//3s
				{
					Position_1PWM=Position_control_left(Position_1,800);
					Position_2PWM=Position_control_right(Position_2,-800);
					PID_Position_left();
					PID_Position_right();
				}
			}
		
			Flag=5;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
			TimesOnOrOff=1;
	}
	
	/***************************>> 5 <<********************************************/
	if(TimesOnOrOff==1&&Flag==5)
	{
		TimesOnOrOff=0;
		Stop();
		Times=0;
		while(Times<=200);
		Times=0;
		while(Times<=10) Front();
	}
	if(Flag==5)//��55cm
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-2400)
		{
			Front();
		}
		else
		{
			Flag=6;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
			Stop();     					//ͣ�����ȴ�ҩ����
		}
	}
	
	while(Flag==6&&LoadOrNot==0);   //�ȴ�ҩ����*********************************************************************************************************************************
	
	/***************************>> 6 <<********************************************/
	if(Flag==6)
	{
		Turn_180du();
		Times=0;
		while(Times<=30)	Front();
		
		Flag=7;
	}
	
	/***************************>> 7 <<********************************************/
	
	if(Flag==7)//��55cm��
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-2620)
		{
			Front();
		}
		else
		{
			Flag=8;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
		}
	}
	
	/***************************>> 8 <<********************************************/
	if(Flag==8)//���������ת90��
	{	
		if(DelayOrNot1=='1')     //////////////////////////�ж�����ͷ�ڶ���ʶ��󴫻��������ݣ���1�����󡢡�2�����ң�
			{
				Position_1=0;
				Position_2=0;
				Times=0;
				while(Times<=250)		//3s
				{
					Position_1PWM=Position_control_left(Position_1,800);
					Position_2PWM=Position_control_right(Position_2,-800);
					PID_Position_left();
					PID_Position_right();
				}
			}
			else
			{
				Position_1=0;
				Position_2=0;
				Times=0;
				while(Times<=250)		//3s
				{
					Position_1PWM=Position_control_left(Position_1,-850);
					Position_2PWM=Position_control_right(Position_2,850);
					PID_Position_left();
					PID_Position_right();
				}
			}
		
			Flag=9;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
			TimesOnOrOff=1;
	}
	
	
	
	/***************************>> 9 <<********************************************/
	if(Flag==9)//��80cm
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-5500)  //90cm��ʮ��·��
		{
			Front();
		}
		else
		{
			Flag=10;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
		}
	}
	
		/***************************>> 10 <<*********************************************************************************/
	if(Flag==10)//���������ת90��
	{	
		if(LeftOrRight=='1')     //////////////////////////�ж�����ͷ�ڶ���ʶ��󴫻��������ݣ���1�����󡢡�2�����ң�
			{
				Position_1=0;
				Position_2=0;
				Times=0;
				while(Times<=250)		//3s
				{
					Position_1PWM=Position_control_left(Position_1,770);
					Position_2PWM=Position_control_right(Position_2,-770);
					PID_Position_left();
					PID_Position_right();
				}
			}
			else
			{
				Position_1=0;
				Position_2=0;
				Times=0;
				while(Times<=250)		//3s
				{
					Position_1PWM=Position_control_left(Position_1,-770);
					Position_2PWM=Position_control_right(Position_2,770);
					PID_Position_left();
					PID_Position_right();
				}
			}
		
			Flag=11;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
			TimesOnOrOff=1;
	}
	
	
	
	/***************************>> 11 <<********************************************/
	if(Flag==11)//��80cm
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-14200)  //90cm��ʮ��·��
		{
			Front();
		}
		else
		{
			Flag=12;
			Encoder_timeSum1=0;   //����
			Encoder_timeSum2=0;
		}
	}
	
	/***************************>> 12 <<********************************************/
	if(Flag==12)//ͣ��
	{
		Stop();
		while(1);
	}

	/*****************************>> �Ҷȴ������ֲ��������� <<******************************************/
		if(L1==1)
		{
			Right();
		}
		
	if(R1==1)
		{
			Left();
		}
		
		if(L2==1)
		{
			Big_Right();
		}
		
	if(R2==1)
		{
			Big_Left();
		}
}

