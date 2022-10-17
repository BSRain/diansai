/*

up才大一，这个代码有很多不足的地方，仅供参考！！ 如有那个错的地方请谅解!!!

有不懂的可以来私信up，up很乐意为大家解答，up的本站名为“陈加油嗯”  

冲冲冲！！！！！！！！！！！！！！！！！

*/


#include "sys.h"
#include "delay.h"
#include "string.h"		
#include "led.h"
#include "TIME.h"
#include "OLED.h"
#include "myMOTOR.h"
#include "myPID.h"
#include "myGrayscale.h"
#include "myControl.h"
#include "myUsart.h"
#include "myKEY.h"

int mode=0;  //**这个变量在myKey.c里会被改变（按键按下）**mode=1进入近端部分，mode=2进入中端部分，mode=3进入远端部分（因为开始理解错题目了，以为所以弄了这个）
extern char RxData;  //这个变量在定义在myUsart中，当openmv识别到数字就会发送给主控、触发串口中断后会给这个变量赋值（因为openmv那边发过来的数据是字符型，所以定义为char）
float Targer=8,PID_speed1=0,PID_speed2=0;

float VKp=0,VKi=2,Vkd=0;  		//速度环PI参数           
float Actual1,Actual2;        //编码器读回来的脉冲


int main(void)
{		
	delay_init();
	LED_Init();												//调试用，板载LED（PC13）
	Beep_Init();      								//开始是计划加应该蜂鸣器的，但是后面没有用进去（可以不管这个）
	Key_Init();       								//按键初始化
	OLED_Init();	    								//OLED初始化
	Motor_Init();    								 //电机控制相关引脚初始化         
	Grayscale_Init();              	 //灰度传感器初始化
	USART1_Init(115200);             //串口1初始化（通信速率一定要和openmv的一致，否侧无法通信）
	TIM3_PWM_Init(7199,0);          //定时器PWM模式初始化(10KHZ)
	Encoder_Init_TIM2(65535,0);     //定时器2编码器模式初始化
	Encoder_Init_TIM4(65535,0);     //定时器4编码器模式初始化
	TIM1_Getsample_Int(499,719);		//5ms任务定时中断，用于计时、记脉冲和进行PID算法(**中断函数放在TINE.c文件里**)

	OLED_Clear();                   //清屏一下，防止OLED不正常(我也不懂为什么要这样，反正看到很多大佬用OLED都加这个的)
	
	while(1)
	{	
		KEY_number();                //扫描按键（调通这个函数就行了）
		OLED_ShowNum(1,8,mode,1);    //显示一下是哪个模式
		
		if(mode==1)                //前端病房执行这段
		{
			if(RxData=='1')	
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_15);//红灯（摄像头识别到1红灯就亮、因为那时候openmv那边没有显示屏，所以就在主控这里加一个灯、当数据接受成功就点亮）
				while(1)	Home1();
			}
			if(RxData=='2')	
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_14);//绿灯
				while(1)	Home2();
			}
		}
		
		if(mode==2)							//中端病房执行这段
		{
			if(RxData>='3'&&RxData<='8')
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_14); //绿灯
				while(1)
				{
					Home_M();
					if(RxData=='1'||RxData=='2') GPIO_SetBits(GPIOC,GPIO_Pin_15);//红灯
				}
			}
		}
		
		if(mode==3)						//远端病房执行这段
		{
			if(RxData>='3'&&RxData<='8')
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_14); //绿灯
				while(1)
				{
					Home_F();
					if(RxData=='1'||RxData=='2') GPIO_SetBits(GPIOC,GPIO_Pin_15);//红灯
				}
			}
		}
		
		//下面这四句是调PID显示数据时用（我就留这不删了、可忽略）
		OLED_ShowSignedNum(1,1,Actual1,5);
		OLED_ShowSignedNum(2,1,Actual2,5);
		OLED_ShowSignedNum(3,1,PID_speed1,4);
		OLED_ShowSignedNum(4,1,PID_speed2,4);
	}
}

