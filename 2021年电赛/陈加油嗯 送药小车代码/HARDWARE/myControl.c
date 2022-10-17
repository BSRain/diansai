#include "sys.h"
#include "myControl.h"
#include "myGrayscale.h"
#include "myMOTOR.h"
#include "myPID.h"

#define L2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)   //灰度传感器用宏定义代替
#define L1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)
#define M GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)
#define R1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)
#define R2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)

#define LoadOrNot GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)   //红外传感器，用于检测药品是否放下和拿出


int Encoder_timeSum1=0,Encoder_timeSum2=0;  //定时器2、和4的脉冲累加，用于计算路程（计算脉冲次数，脉冲次数达到后清零）
int Flag=1;    //用这个标志来分段走（比如走病房1：Flag=1时直走65cm，然后把Flag置为2，Flag=2则用转向环转90度.....依此类推）
int DelayOrNot=1,DelayOrNot1=1;
extern int Times;       //用这个变量来延时
extern char RxData;    //摄像头传过来的数字（字符型）
char LeftOrRight='1';  //初值赋‘0’    '1'向左转     '2'向右转
int TimesOnOrOff=1;

extern int Position_1,Position_2;
extern float Targer;

int Position_1PWM,Position_2PWM;

/*---------------------------------------------------------------------------------------------------------------------------------*/
/*

停车延时用
参数一：输入要延时的毫秒数

*/
void TIME_Delay_ms(int ms)
{
	if(TimesOnOrOff==1)  //延时1S
	{
		TimesOnOrOff=0;
		Stop();
		Times=0;
		while(Times<=(ms/5));
		TimesOnOrOff=1;
	}
}
/*---------------------------------------------------------------------------------------------------------------------------------*/


/********************************>> 1号病房 <<***************************************************************************************************/
void Home1(void)
{
	/***************************>> 1 <<********************************************/
	
	if(Flag==1&&LoadOrNot==0)//跑65cm
	{
		/******>>下面三行用于放药后延时500ms<<**********/
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
			Encoder_timeSum1=0;   //清零
			Encoder_timeSum2=0;
			
			TIME_Delay_ms(1000);
		}
	}

	/***************************>> 2 <<********************************************/
	
	if(Flag==2)//转90°
	{
		Turn_Left_90du();
		
		Flag=3;	
		
		Times=0;
		while(Times<=50)	Front();
	}
	
	/***************************>> 3 <<********************************************/
	if(Flag==3)//跑55cm
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-2350)
		{
			Front();
		}
		else
		{
			Flag=4;
			Encoder_timeSum1=0;   //清零
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
	if(Flag==5)   //跑55cm°
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-2300)
		{
			Front();
		}
		else
		{
			Flag=6;
			Encoder_timeSum1=0;   //清零
			Encoder_timeSum2=0;
			
			TIME_Delay_ms(500);
		}
	}
	
	
	/***************************>> 6 <<********************************************/
	if(Flag==6)//右转90°
	{	
		Turn_Right_90du();
		
		Flag=7;
		
		Times=0;
		while(Times<=50) Front();
	}
	
	/***************************>> 7 <<********************************************/
	if(Flag==7)//跑65cm°
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-3220)
		{
			Front();
		}
		else
		{
			Flag=8;
			Encoder_timeSum1=0;   //清零
			Encoder_timeSum2=0;
		} 
	}
	
	/***************************>> 8 <<********************************************/
	if(Flag==8)//停车
	{
		Stop();
		while(1);
	}

	/*****************************>> 灰度传感器弥补矫正方向 <<******************************************/
	if(L1==1||L2==1)
	{
		Right();
	}
		
	if(R1==1||R2==1)
	{
		Left();
	}
}



///********************************>> 2号病房 <<***************************************************************************************************/
void Home2(void)
{
	/***************************>> 1 <<********************************************/
	
	if(Flag==1&&LoadOrNot==0)//跑65cm
	{
		/******>>下面三行用于放药后延时500ms<<**********/
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
			Encoder_timeSum1=0;   //清零
			Encoder_timeSum2=0;
			
			TIME_Delay_ms(1000);
		}
	}

	/***************************>> 2 <<********************************************/
	
	if(Flag==2)//转90°
	{
		Turn_Right_90du();
		
		Flag=3;	
		
		Times=0;
		while(Times<=50)	Front();
	}
	
	/***************************>> 3 <<********************************************/
	if(Flag==3)//跑55cm
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-2350)
		{
			Front();
		}
		else
		{
			Flag=4;
			Encoder_timeSum1=0;   //清零
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
	if(Flag==5)   //跑55cm°
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-2300)
		{
			Front();
		}
		else
		{
			Flag=6;
			Encoder_timeSum1=0;   //清零
			Encoder_timeSum2=0;
			
			TIME_Delay_ms(500);
		}
	}
	
	
	/***************************>> 6 <<********************************************/
	if(Flag==6)//左转90°
	{	
		Turn_Left_90du();
		
		Flag=7;
		
		Times=0;
		while(Times<=50) Front();
	}
	
	/***************************>> 7 <<********************************************/
	if(Flag==7)//跑65cm°
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-3220)
		{
			Front();
		}
		else
		{
			Flag=8;
			Encoder_timeSum1=0;   //清零
			Encoder_timeSum2=0;
		} 
	}
	
	/***************************>> 8 <<********************************************/
	if(Flag==8)//停车
	{
		Stop();
		while(1);
	}

	/*****************************>> 灰度传感器弥补矫正方向 <<******************************************/
	if(L1==1||L2==1)
	{
		Right();
	}
		
	if(R1==1||R2==1)
	{
		Left();
	}
}


///********************************>> 中部病房 <<***************************************************************************************************/
/*
中部病房执行步骤：
车跑60cm关闭灰度传感器-->过十字路口-->打开灰度传感器矫正车的位置（如果车偏离红线了）-->直跑10cm

-->降低车速跑10cm（用于摄像头识别数字）-->变为原来车速跑40cm-->根据摄像头传回来的值左转或者右转
*/
void Home_M(void)
{
	/***************************>> 1 <<********************************************/
	if(Flag==1&&LoadOrNot==0)//跑60cm
	{
		/******>>下面三行用于放药后延时1000ms<<**********/
		Times=0;
		while(DelayOrNot==1&&Times<=200);
		DelayOrNot=0;
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-8750)  //4500到十字路口（65cm）
		{
			Front();
			//if((Encoder_timeSum1+Encoder_timeSum2)/2>=2000) kaiguan=1;
		}
		else
		{
			Flag=2;
			Encoder_timeSum1=0;   //清零
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
	if(Flag==2)   //左转90°或者右转90°(要左转还是右转得看摄像头传过来的值)
	{
		if(RxData=='1')     //////////////////////////判断摄像头第二次识别后传回来的数据（‘1’向左、‘2’向右）
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
			Encoder_timeSum1=0;   //清零
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
	if(Flag==3)//跑55cm
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-2300)
		{
			Front();
		}
		else
		{
			Flag=4;
			Encoder_timeSum1=0;   //清零
			Encoder_timeSum2=0;
			Stop();     					//停车，等待药拿走
		}
	}
	
	while(Flag==4&&LoadOrNot==0);   //等待药拿走
	
	/***************************>> 4 <<********************************************/
	if(Flag==4)
	{
		Turn_180du();
		Times=0;
		while(Times<=30)	Front();
		
		Flag=5;
	}
	
	/***************************>> 5 <<********************************************/
	
	if(Flag==5)//跑55cm°
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-2750)
		{
			Front();
		}
		else
		{
			Flag=6;
			Encoder_timeSum1=0;   //清零
			Encoder_timeSum2=0;
		}
	}
	
	/***************************>> 6 <<********************************************/
	if(Flag==6)//向左或者右转90°
	{	
		if(RxData=='1')     //////////////////////////判断摄像头第二次识别后传回来的数据（‘1’向左、‘2’向右）
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
			Encoder_timeSum1=0;   //清零
			Encoder_timeSum2=0;
			TimesOnOrOff=1;
			Times=0;
			while(Times<=200);
	}
	
	/***************************>> 7 <<********************************************/
	if(Flag==7)//跑80cm
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-8700)  //90cm到十字路口
		{
			Front();
		}
		else
		{
			Flag=8;
			Encoder_timeSum1=0;   //清零
			Encoder_timeSum2=0;
		}
	}
	
	/***************************>> 8 <<********************************************/
	if(Flag==8)//停车
	{
		Stop();
		while(1);
	}

	/*****************************>> 灰度传感器弥补矫正方向 <<******************************************/
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


/********************************>> 远端病房 <<***************************************************************************************************/
/*
远端病房执行步骤：
车跑60cm关闭灰度传感器-->过十字路口-->打开灰度传感器矫正车的位置（如果车偏离红线了）-->直跑10cm

-->降低车速跑10cm（用于摄像头识别数字）-->变为原来车速跑40cm-->根据摄像头传回来的值左转或者右转
*/
void Home_F(void) 
{
	/***************************>> 1 <<********************************************/
	if(Flag==1&&LoadOrNot==0)//跑60cm
	{
		/******>>下面三行用于放药后延时1000ms<<**********/
		Times=0;
		while(DelayOrNot==1&&Times<=200);
		DelayOrNot=0;
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-13800)  //4500到十字路口（65cm）
		{
			Front();
			//if((Encoder_timeSum1+Encoder_timeSum2)/2>=2000) kaiguan=1;
		}
		else
		{
			Flag=2;
			Encoder_timeSum1=0;   //清零
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
	if(Flag==2)   //左转90°或者右转90°(要左转还是右转得看摄像头传过来的值)
	{
		LeftOrRight=RxData;//用于回来时转弯用
		if(RxData=='1')     //////////////////////////判断摄像头第二次识别后传回来的数据（‘1’向左、‘2’向右）
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
			Encoder_timeSum1=0;   //清零
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
	if(Flag==3)//跑55cm
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-5210)
		{
			Front();
		}
		else
		{
			Flag=4;
			Encoder_timeSum1=0;   //清零
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
	if(Flag==4)   //左转90°或者右转90°(要左转还是右转得看摄像头传过来的值)
	{
		DelayOrNot1=RxData;
		if(RxData=='1')     //////////////////////////判断摄像头第二次识别后传回来的数据（‘1’向左、‘2’向右）
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
			Encoder_timeSum1=0;   //清零
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
	if(Flag==5)//跑55cm
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-2400)
		{
			Front();
		}
		else
		{
			Flag=6;
			Encoder_timeSum1=0;   //清零
			Encoder_timeSum2=0;
			Stop();     					//停车，等待药拿走
		}
	}
	
	while(Flag==6&&LoadOrNot==0);   //等待药拿走*********************************************************************************************************************************
	
	/***************************>> 6 <<********************************************/
	if(Flag==6)
	{
		Turn_180du();
		Times=0;
		while(Times<=30)	Front();
		
		Flag=7;
	}
	
	/***************************>> 7 <<********************************************/
	
	if(Flag==7)//跑55cm°
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-2620)
		{
			Front();
		}
		else
		{
			Flag=8;
			Encoder_timeSum1=0;   //清零
			Encoder_timeSum2=0;
		}
	}
	
	/***************************>> 8 <<********************************************/
	if(Flag==8)//向左或者右转90°
	{	
		if(DelayOrNot1=='1')     //////////////////////////判断摄像头第二次识别后传回来的数据（‘1’向左、‘2’向右）
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
			Encoder_timeSum1=0;   //清零
			Encoder_timeSum2=0;
			TimesOnOrOff=1;
	}
	
	
	
	/***************************>> 9 <<********************************************/
	if(Flag==9)//跑80cm
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-5500)  //90cm到十字路口
		{
			Front();
		}
		else
		{
			Flag=10;
			Encoder_timeSum1=0;   //清零
			Encoder_timeSum2=0;
		}
	}
	
		/***************************>> 10 <<*********************************************************************************/
	if(Flag==10)//向左或者右转90°
	{	
		if(LeftOrRight=='1')     //////////////////////////判断摄像头第二次识别后传回来的数据（‘1’向左、‘2’向右）
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
			Encoder_timeSum1=0;   //清零
			Encoder_timeSum2=0;
			TimesOnOrOff=1;
	}
	
	
	
	/***************************>> 11 <<********************************************/
	if(Flag==11)//跑80cm
	{
		if((Encoder_timeSum1+Encoder_timeSum2)/2>=-14200)  //90cm到十字路口
		{
			Front();
		}
		else
		{
			Flag=12;
			Encoder_timeSum1=0;   //清零
			Encoder_timeSum2=0;
		}
	}
	
	/***************************>> 12 <<********************************************/
	if(Flag==12)//停车
	{
		Stop();
		while(1);
	}

	/*****************************>> 灰度传感器弥补矫正方向 <<******************************************/
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

