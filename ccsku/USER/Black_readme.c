//-----------使用实例-----------//
//------------延时-----------//
/*
delay(int ms);//软件延时ms(待改进)
*/
//------------GPIO-----------//
/*
OUTPUT_GPIO_Config(1，0);//初始化p1.0为输出
INPUT_GPIO_Config(1,1);//初始化p1.1为输入
OUTPUT_GPIO_State(1,0,1);//p1.0输出高电平
OUTPUT_GPIO_State(1,0,0);//p1.0输出低电平
if(P1IN & BIT1) //判断P1.1输入电平高低状态
*/
//------------KEY-----------//
/*
KEY0_GPIO_Config();//初始化板载按键0 p1.1
KEY1_GPIO_Config();//初始化板载按键1 p2.1
KEY0_STATE();//如果按下返回0 松开返回1
KEY1_STATE();//如果按下返回0 松开返回1
*/
//------------LED-----------//
/*
LED0_GPIO_Config(void);//初始化板载LED0 p1.0 需要跳线on
LED1_GPIO_Config(void);//初始化板载LED1 p4.7
LED0_STATE(0);//操作led0 灭
LED1_STATE(1);//操作led1 亮
LED1_STATE(2);//操作led1 状态翻转
*/
//---------INTERRUPT--------//
/*
IN_Config(1,1);//板载按键p2.1做外部中断 下降沿触发
中断处理函数实例 请见Black_INTERRUPT.c
*/
//------------PWM-----------//
/*
PWM0_Config(void);//p1.2 时钟作PWM初始化
PWM1_Config(void);//p1.3 时钟作PWM初始化
PWM2_Config(void);//p1.4 时钟作PWM初始化
PWM3_Config(void);//p1.5 时钟作PWM初始化
PWM0_State(25);//p1.2 PWM百分之25占空比
PWM1_State(50);//p1.3 PWM百分之50占空比
PWM2_State(75);//p1.4 PWM百分之75占空比
PWM3_State(100);//p1.5 PWM百分之100占空比
*/
//------------USART-----------//
/*
USART0_Config();//串口0初始化
USART1_Config();//串口1初始化 此串口在usb上
send0_buf("this is com test"); //串口发送字符串
send1_buf("this is com test"); //串口发送字符串
*/
//------------OLED-----------//
/*
仅展示基本点亮函数，更多函数请见Black_OLED.c
OLED_Init(); //OLED初始化 SCL p6.3 SDA p6.4
OLED_Clear();//OLED清屏
OLED_ShowString(29,0,“I E LAB”,8); //按照x，y坐标来显示字符串，最后一位是字体大小
OLED_ShowVI(50-1,3,u32 num,8);//显示某个变量值
*/

