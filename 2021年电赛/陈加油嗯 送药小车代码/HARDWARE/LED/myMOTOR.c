#include "myMOTOR.h"
#include "TIME.h"
#include "myPID.h"

extern float PID_speed1,PID_speed2;
extern int Position_1PWM,Position_2PWM;
extern int Position_1,Position_2;
extern int Times;
extern int Encoder_timeSum1,Encoder_timeSum2;  //定时器2、和4的脉冲累加，用于计算路程（计算脉冲次数，脉冲次数达到后清零）
/*
	初始化4个引脚，控制电机转向用
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

void Left(void)  //左转
{
	TIM_SetCompare3(TIM3,6000);
	TIM_SetCompare4(TIM3,6000);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_6);			//右轮
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
}

void Right(void)		//右转
{

	TIM_SetCompare3(TIM3,6000);
	TIM_SetCompare4(TIM3,6000);
		
	GPIO_SetBits(GPIOA,GPIO_Pin_4);			//左轮
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}

void Big_Left(void)  //左转（转弯力度大一点）
{
	TIM_SetCompare3(TIM3,6500);
	TIM_SetCompare4(TIM3,6500);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_6);			//右轮
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
}

void Big_Right(void)		//右转（转弯力度大一点）
{

	TIM_SetCompare3(TIM3,6500);
	TIM_SetCompare4(TIM3,6500);
		
	GPIO_SetBits(GPIOA,GPIO_Pin_4);			//左轮
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}


void Stop(void)		//停车
{
	TIM_SetCompare3(TIM3,1500);   //占空比给1500我的电机是动不了的(如果给0的话电机会因为惯性走一小段距离)
	TIM_SetCompare4(TIM3,1500);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
}


//转向环要用到
void PID_Position_left(void)
{
	if(Position_1PWM>0)
	{	
		//Front();
		TIM_SetCompare3(TIM3,Position_1PWM);
	
		GPIO_SetBits(GPIOA,GPIO_Pin_4);			//左轮
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	}
	if(Position_1PWM<0)
	{
		//Back();
		TIM_SetCompare3(TIM3,-Position_1PWM);
	
		GPIO_SetBits(GPIOA,GPIO_Pin_5);			//左轮
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	}
}

//转向环要用到
void PID_Position_right(void)
{
	if(Position_2PWM>0)
	{	
		//Front();
		TIM_SetCompare4(TIM3,Position_2PWM);
	
		GPIO_SetBits(GPIOA,GPIO_Pin_6);			//左轮
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	}
	if(Position_2PWM<0)
	{
		//Back();
		TIM_SetCompare4(TIM3,-Position_2PWM);
	
		GPIO_SetBits(GPIOA,GPIO_Pin_7);			//左轮
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	}
}

//右转90度（其实我在myControl里面有的转向没有用函数封装，这个是封装起来了，，，大家看到有和这里差不多的代码就是转向了，，，因为调试的时候调好了，有一部分就没改）
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
	
	Encoder_timeSum1=0;   //清零
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
	
	Encoder_timeSum1=0;   //清零
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
	
	Encoder_timeSum1=0;   //清零
	Encoder_timeSum2=0;
}





void Back(void)		//前进
{
	if(PID_speed1<0) 
	{
		TIM_SetCompare3(TIM3,-PID_speed1);
		TIM_SetCompare4(TIM3,-PID_speed2);
		
		GPIO_SetBits(GPIOA,GPIO_Pin_5);			//左轮
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	
		GPIO_SetBits(GPIOA,GPIO_Pin_7);			//右轮
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	}
	else
	{
		TIM_SetCompare3(TIM3,PID_speed1);
		TIM_SetCompare4(TIM3,PID_speed2);
		
		GPIO_SetBits(GPIOA,GPIO_Pin_4);			//左轮
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	
		GPIO_SetBits(GPIOA,GPIO_Pin_6);			//右轮
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	}
}

void Front(void)
{
	if(PID_speed1<0) 
	{
		TIM_SetCompare3(TIM3,-PID_speed1);
		
		GPIO_SetBits(GPIOA,GPIO_Pin_4);			//左轮
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	}
	else
	{
		TIM_SetCompare3(TIM3,PID_speed1);
		
		GPIO_SetBits(GPIOA,GPIO_Pin_5);			//左轮
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	}

	if(PID_speed2<0) 
	{
		TIM_SetCompare4(TIM3,-PID_speed2);
	
		GPIO_SetBits(GPIOA,GPIO_Pin_6);			//右轮
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	}
	else
	{
		TIM_SetCompare4(TIM3,PID_speed2);
	
		GPIO_SetBits(GPIOA,GPIO_Pin_7);			//右轮
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	}
}



