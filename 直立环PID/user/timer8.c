

#define TIM8_Period  5000   //5000--14.4KHz
/***************************************************************************/
void TIM8_PWM_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8|RCC_APB2Periph_GPIOC,ENABLE); //使能TIM8时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;   //TIM8的四个PWM输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	TIM_DeInit(TIM8);
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;
	TIM_TimeBaseInitStructure.TIM_Period=TIM8_Period-1;  //14.4KHz
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC1Init(TIM8,&TIM_OCInitStructure);
	TIM_OC2Init(TIM8,&TIM_OCInitStructure);
	TIM_OC3Init(TIM8,&TIM_OCInitStructure);
	TIM_OC4Init(TIM8,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM8,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM8,ENABLE);
  TIM_Cmd(TIM8,ENABLE);
	//TIM_CCPreloadControl(TIM8,DISABLE);  //ENABLE
	TIM_CtrlPWMOutputs(TIM8,ENABLE);
}
/***************************************************************************/
void TIM2_Encoder_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB
	              |RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	//使能TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); 
	//TIM2和TIM3部分重映射
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2|GPIO_PartialRemap_TIM3,ENABLE); 
	//禁止JTAG功能，把PB3，PB4作为普通IO口使用
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;               
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure); //GPIO端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//GPIO端口配置
	
	TIM_DeInit(TIM2);//TIM2寄存器复位
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0; 
	TIM_TimeBaseStructure.TIM_Period = 0xffff; 
	//初始值配置为0xffff，便于通过计数值正负判断出电机的正反转
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//
	//配置编码器接口工作模式TIM2为两相计数模式（速度和方向）正向上升沿计数模式
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, 
	                   TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);//输出比较结构体参数初始化
	TIM_ICInitStructure.TIM_ICFilter =6; //滤波
	TIM_ICInit(TIM2, &TIM_ICInitStructure);//初始化PWM输入捕获模式
	
	//TIM_ClearFlag(TIM2, TIM_FLAG_Update);  
	//TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_SetCounter(TIM2,0);  
	TIM_Cmd(TIM2, ENABLE); //使能定时器开始计数
}
/***************************************************************************/
void TIM3_Encoder_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //使能TIM3时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;     //TIM3_1/TIM3_2 Remap
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0; // No prescaling 
	TIM_TimeBaseStructure.TIM_Period = 0xffff; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	//配置编码器接口工作模式TIM3为两相计数模式（速度和方向）反向上升沿计数模式
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, 
	                           TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter =6;   //;TIM_ICFilter_Mask  6
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	//TIM_ClearFlag(TIM3, TIM_FLAG_Update);  // Clear all pending interrupts
	//TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  TIM_SetCounter(TIM3,0);  //速度模式  //TIM3->CNT = 10000;  //位置模式
	TIM_Cmd(TIM3, ENABLE); 
}
/***************************************************************************/



