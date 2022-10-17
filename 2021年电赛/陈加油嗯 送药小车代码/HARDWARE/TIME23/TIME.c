#include "TIME.h"
#include "myPID.h"
#include "myMOTOR.h"

int flag=0;
int Times=0;
extern int Encoder_timeSum1,Encoder_timeSum2;  //定时器2、和4的脉冲累加，用于计算路程（计算脉冲次数，脉冲次数达到后清零）
extern float Actual1,Actual2;
extern float PID_speed1,PID_speed2;

int Position_1,Position_2;

#define L2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)
#define L1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)
#define M  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)
#define R1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)
#define R2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)


/*********************************************编码器脉冲读取，PID计算刷新时钟2***********************************/
void TIM1_Getsample_Int(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 为50ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0; //重复计数设置
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  TIM_ClearFlag(TIM1,TIM_FLAG_Update);  //清楚中断标志位
	
	TIM_ITConfig( TIM1,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//使能定时器1更新触发中断
 
	TIM_Cmd(TIM1, ENABLE);  //使能TIMx外设
 	
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1中断;//**--**普通定时器（=TIMx_IRQn）
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure); 
}
//***************TIME1的中断********************************/
void TIM1_UP_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET) //溢出中断
	{
		if(flag == 1)
		{
			flag=0;
			GPIO_SetBits(GPIOC,GPIO_Pin_13);//灯
		}
		else
		{
			flag=1;
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);//灯
		}
		Times++;    
		Actual1=Read_Encoder_TIM2();   //读编码器5ms内的脉冲数
		Actual2=Read_Encoder_TIM4();
		
		Encoder_timeSum1+=Actual1;     //把脉冲累加（测距、在开始测量的时候一定要清零）
		Encoder_timeSum2+=Actual2;
		
		Position_1+=-Actual1;          //把脉冲累加、作为转向环的输入参数
		Position_2+=-Actual2;
		
		PID_speed1=velocity_PI_value1(Actual1);  //速度环PI控制
		PID_speed2=velocity_PI_value2(Actual2);
	}	
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //清除中断标志位
}

//定时器3 PWM输出（CH1->PA6 CH2->PA7 CH3->PB0 CH4->PB1）**TIM_SetComparex(TIMx,xxx);//占空比函数
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIO外设；————假如要AFIO复用功能模块时钟则要加上（  | RCC_APB2Periph_AFIO）
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
 
   //初始化输出TIM3四个通道PWM脉冲波形的引脚(CH1->PA6 CH2->PA7 CH3->PB0 CH4->PB1)
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //TIM_CH1、CH2
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //TIM_CH3、CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channe1234 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	
	//TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1
  //TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	
	//IM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
  //TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIM 3
}


/*TIM2初始化为编码器接口*/
void Encoder_Init_TIM2(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义一个引脚初始化的结构体  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//定义一个定时器初始化的结构体
  TIM_ICInitTypeDef TIM_ICInitStructure; //定义一个定时器编码器模式初始化的结构体
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //使能TIM2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能GPIOA时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//PA0、PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//根据GPIO_InitStructure的参数初始化GPIOA

	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler = psc; // 预分频器 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct的参数初始化定时器TIM2
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3：CH1、CH2同时计数，四分频
	TIM_ICStructInit(&TIM_ICInitStructure); //把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 10;  //设置滤波器长度
	TIM_ICInit(TIM2, &TIM_ICInitStructure); //根TIM_ICInitStructure参数初始化定时器TIM1编码器模式

	TIM_Cmd(TIM2, ENABLE); //使能定时器2
}

//读取编码器计数
int Read_Encoder_TIM2(void)
{
	int Encoder_TIM2;
	Encoder_TIM2=TIM2->CNT; //读取计数
	if(Encoder_TIM2>0xefff)Encoder_TIM2=Encoder_TIM2-0xffff; //转化计数值为有方向的值，大于0正转，小于0反转。
	                                                      //TIM2->CNT范围为0-0xffff，初值为0。
	TIM2->CNT=0; //读取完后计数清零
	return Encoder_TIM2; //返回值
}



/*TIM4初始化为编码器接口*/
void Encoder_Init_TIM4(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义一个引脚初始化的结构体  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//定义一个定时器初始化的结构体
  TIM_ICInitTypeDef TIM_ICInitStructure; //定义一个定时器编码器模式初始化的结构体
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIM4时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能GPIOB时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//PB6、PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);	//根据GPIO_InitStructure的参数初始化GPIOB

	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler = psc; // 预分频器 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct的参数初始化定时器TIM4
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3：CH1、CH2同时计数，四分频
	TIM_ICStructInit(&TIM_ICInitStructure); //把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 10;  //设置滤波器长度
	TIM_ICInit(TIM4, &TIM_ICInitStructure); //根TIM_ICInitStructure参数初始化定时器TIM4编码器模式

	TIM_Cmd(TIM4, ENABLE); //使能定时器4
}

//读取编码器计数
int Read_Encoder_TIM4(void)
{
	int Encoder_TIM4;
	Encoder_TIM4=TIM4->CNT; //读取计数
	if(Encoder_TIM4>0xefff)Encoder_TIM4=Encoder_TIM4-0xffff; //转化计数值为有方向的值，大于0正转，小于0反转。
	                                                      //TIM4->CNT范围为0-0xffff，初值为0。
	TIM4->CNT=0; //读取完后计数清零
	return Encoder_TIM4; //返回值
}
