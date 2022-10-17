
#include "stm32f10x.h" //stm32头文件
#include "config.h"    //定义头文件
#include "delay.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "timer.c"
#include "usart1.c"
#include "timer8.c"
#include "pidspeed.c"

float Med_Angle=-3.00;      // 机械中值，能使得小车真正平衡住的角度 
float Target_Speed=0;	  // 期望速度。---二次开发接口，用于控制小车前进后退及其速度。
int set=0;
float 
  Vertical_Kp=5000,
  Vertical_Kd=50;      // 直立环Kp、Kd
float 
  Velocity_Kp=0.30,
  Velocity_Ki=0.0015;   // 速度环Kp、Ki（正反馈）
float 
  Turn_Kp=-0.6;

int Vertical_out,Velocity_out,Turn_out; // 直立环&速度环&转向环的输出变量

void key_count(void);
int Vertical(float Med,float Angle,float gyro_Y);
/******************************************************************************/
//所有程序使用到的IO都在这里集中配置
void GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//PA8是LED灯
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//通用推挽输出	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);//对选中管脚初始化
	GPIO_SetBits(GPIOA,GPIO_Pin_8);   //上电点亮LED
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;        //PB6是按键
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;    //输入下拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);           //对选中管脚初始化
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);                //打开下拉电阻
}
/******************************************************************************/
//通过串口1上报计算后的姿态数据给电脑
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
//roll:横滚角.单位0.01度。 -9000 -> 9000 对应-90.00 -> 90.00度
//pitch:俯仰角.单位 0.01度。-18000 -> 18000 对应 -180.00 -> 180.00 度
//yaw:航向角.单位为0.01度。-18000 -> 18000  对应 -180.00 -> 180.00 度
void usart1_report_imu(short roll,short pitch,short yaw)
{
	u32 i;
	
	send_buf[0]=0xaa;
	send_buf[1]=0xaa;
	send_buf[2]=0x01;
	send_buf[3]=11;
	send_buf[4]=roll>>8;
	send_buf[5]=roll;
	send_buf[6]=pitch>>8;
	send_buf[7]=pitch;
	send_buf[8]=yaw>>8;
	send_buf[9]=yaw;
	send_buf[10]=0;
	send_buf[11]=0;
	send_buf[12]=0;
	send_buf[13]=0;
	send_buf[14]=0;
	
	send_buf[15]=0;
	for(i=0;i<15;i++)
	{send_buf[15]+=send_buf[i];}
	UART1_send(send_buf,16);
}
/******************************************************************************/
/******************************************************************************/
int main(void)
{
	GPIO_Config();
	USART1_Init();
	delay_init();
	TIM8_PWM_Config();  //配置电机控制引脚
	TIM2_Encoder_Config();
	TIM3_Encoder_Config();
	delay_ms(200);  //等待MPU6050稳定
	MPU_Init();			//初始化MPU6050
	delay_ms(100);
	while(mpu_dmp_init()!=0) {delay_ms(200);}
	TIM6_Init(999,71);  //1ms定时

	while(1)
	{	
	//speedL_set=50000;
	//speedR_set=50000;
	
		
		if(time_cntr1>=200)  //0.2s
			{
				time_cntr1=0;
				LED_Shan;
			}
		if(time_cntr2>=20)
			{
				time_cntr2=0;
				SpeedL_deal();
				SpeedR_deal();
			}
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{
				//temp=MPU_Get_Temperature();	//得到温度值
				//MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
				short gyrox,gyroy,gyroz;
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
				//usart1_report_imu((short)(-pitch),(short)(-roll),(short)(yaw));
				set=Vertical(Med_Angle,roll,gyroy);
				speedL_set=set;
				speedR_set=set;
			}
	}
}
/***************直立环*******************************/
int Vertical(float Med,float Angle,float gyro_Y) 
{
  int PWM_out;
  
  PWM_out = Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro_Y-0);
  
  return PWM_out;
} 
/******************************************************************/
void key_count(void)
{
	switch(key_step)
	{
		case 0: 
			if(Read_Touch!=0)      //键被按下
				{
					key_cntr=0;
					key_step=1;
				}
			break;
		case 1: 
			if(key_cntr>=10)       //按下后10ms延时
				{
					if(Read_Touch!=0)  //键仍被按下
						{
							key_value++;
							if(key_value>=5)key_value=0;
							key_step=2;
						}
					else          //键没有按下，认为是抖动
						key_step=0;
				}
			break;
		case 2:
			if(Read_Touch==0)  //等待按键松开
				{
					key_cntr=0;
					key_step=3;
				}
			break;
		case 3:          //松开后500ms延时
			if(key_cntr>=500)key_step=0;
			break;
	}
}
/******************************************************************************/



	
