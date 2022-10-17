#include "myMOTOR.h"
#include "TIME.h"
#include "myPID.h"

extern float PID_speed1,PID_speed2;
extern int Position_1PWM,Position_2PWM;
extern int Position_1,Position_2;
extern int Times;
extern int Encoder_timeSum1,Encoder_timeSum2;  //��ʱ��2����4�������ۼӣ����ڼ���·�̣����������������������ﵽ�����㣩
/*
	��ʼ��4�����ţ����Ƶ��ת����
*/
void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

void Left(void)  //��ת
{
	TIM_SetCompare3(TIM3,6000);
	TIM_SetCompare4(TIM3,6000);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_6);			//����
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
}

void Right(void)		//��ת
{

	TIM_SetCompare3(TIM3,6000);
	TIM_SetCompare4(TIM3,6000);
		
	GPIO_SetBits(GPIOA,GPIO_Pin_4);			//����
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}

void Big_Left(void)  //��ת��ת�����ȴ�һ�㣩
{
	TIM_SetCompare3(TIM3,6500);
	TIM_SetCompare4(TIM3,6500);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_6);			//����
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
}

void Big_Right(void)		//��ת��ת�����ȴ�һ�㣩
{

	TIM_SetCompare3(TIM3,6500);
	TIM_SetCompare4(TIM3,6500);
		
	GPIO_SetBits(GPIOA,GPIO_Pin_4);			//����
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}


void Stop(void)		//ͣ��
{
	TIM_SetCompare3(TIM3,1500);   //ռ�ձȸ�1500�ҵĵ���Ƕ����˵�(�����0�Ļ��������Ϊ������һС�ξ���)
	TIM_SetCompare4(TIM3,1500);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
}


//ת��Ҫ�õ�
void PID_Position_left(void)
{
	if(Position_1PWM>0)
	{	
		//Front();
		TIM_SetCompare3(TIM3,Position_1PWM);
	
		GPIO_SetBits(GPIOA,GPIO_Pin_4);			//����
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	}
	if(Position_1PWM<0)
	{
		//Back();
		TIM_SetCompare3(TIM3,-Position_1PWM);
	
		GPIO_SetBits(GPIOA,GPIO_Pin_5);			//����
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	}
}

//ת��Ҫ�õ�
void PID_Position_right(void)
{
	if(Position_2PWM>0)
	{	
		//Front();
		TIM_SetCompare4(TIM3,Position_2PWM);
	
		GPIO_SetBits(GPIOA,GPIO_Pin_6);			//����
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	}
	if(Position_2PWM<0)
	{
		//Back();
		TIM_SetCompare4(TIM3,-Position_2PWM);
	
		GPIO_SetBits(GPIOA,GPIO_Pin_7);			//����
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	}
}

//��ת90�ȣ���ʵ����myControl�����е�ת��û���ú�����װ������Ƿ�װ�����ˣ�������ҿ����к�������Ĵ������ת���ˣ�������Ϊ���Ե�ʱ������ˣ���һ���־�û�ģ�
void Turn_Right_90du(void)
{
	Position_1=0;
	Position_2=0;
	Times=0;
	while(Times<=400)		//3s
	{
		Position_1PWM=Position_control_left(Position_1,800);
		Position_2PWM=Position_control_right(Position_2,-800);
		PID_Position_left();
		PID_Position_right();
	}
	
	Encoder_timeSum1=0;   //����
	Encoder_timeSum2=0;
}

void Turn_Left_90du(void)
{
	Position_1=0;
	Position_2=0;
	Times=0;
	while(Times<=300)		//3s
	{
		Position_1PWM=Position_control_left(Position_1,-750);
		Position_2PWM=Position_control_right(Position_2,750);
		PID_Position_left();
		PID_Position_right();
	}
	
	Encoder_timeSum1=0;   //����
	Encoder_timeSum2=0;
}


void Turn_180du(void)
{
	Position_1=0;
	Position_2=0;
	Times=0;
	while(Times<=400)		//3s
	{
		Position_1PWM=Position_control_left(Position_1,830*2);
		Position_2PWM=Position_control_right(Position_2,-830*2);
		PID_Position_left();
		PID_Position_right();
	}
	
	Encoder_timeSum1=0;   //����
	Encoder_timeSum2=0;
}





void Back(void)		//ǰ��
{
	if(PID_speed1<0) 
	{
		TIM_SetCompare3(TIM3,-PID_speed1);
		TIM_SetCompare4(TIM3,-PID_speed2);
		
		GPIO_SetBits(GPIOA,GPIO_Pin_5);			//����
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	
		GPIO_SetBits(GPIOA,GPIO_Pin_7);			//����
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	}
	else
	{
		TIM_SetCompare3(TIM3,PID_speed1);
		TIM_SetCompare4(TIM3,PID_speed2);
		
		GPIO_SetBits(GPIOA,GPIO_Pin_4);			//����
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	
		GPIO_SetBits(GPIOA,GPIO_Pin_6);			//����
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	}
}

void Front(void)
{
	if(PID_speed1<0) 
	{
		TIM_SetCompare3(TIM3,-PID_speed1);
		
		GPIO_SetBits(GPIOA,GPIO_Pin_4);			//����
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	}
	else
	{
		TIM_SetCompare3(TIM3,PID_speed1);
		
		GPIO_SetBits(GPIOA,GPIO_Pin_5);			//����
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	}

	if(PID_speed2<0) 
	{
		TIM_SetCompare4(TIM3,-PID_speed2);
	
		GPIO_SetBits(GPIOA,GPIO_Pin_6);			//����
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	}
	else
	{
		TIM_SetCompare4(TIM3,PID_speed2);
	
		GPIO_SetBits(GPIOA,GPIO_Pin_7);			//����
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	}
}



