/*

up�Ŵ�һ����������кܶ಻��ĵط��������ο����� �����Ǹ���ĵط����½�!!!

�в����Ŀ�����˽��up��up������Ϊ��ҽ��up�ı�վ��Ϊ���¼����š�  

���壡��������������������������������

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

int mode=0;  //**���������myKey.c��ᱻ�ı䣨�������£�**mode=1������˲��֣�mode=2�����ж˲��֣�mode=3����Զ�˲��֣���Ϊ��ʼ������Ŀ�ˣ���Ϊ����Ū�������
extern char RxData;  //��������ڶ�����myUsart�У���openmvʶ�����־ͻᷢ�͸����ء����������жϺ������������ֵ����Ϊopenmv�Ǳ߷��������������ַ��ͣ����Զ���Ϊchar��
float Targer=8,PID_speed1=0,PID_speed2=0;

float VKp=0,VKi=2,Vkd=0;  		//�ٶȻ�PI����           
float Actual1,Actual2;        //������������������


int main(void)
{		
	delay_init();
	LED_Init();												//�����ã�����LED��PC13��
	Beep_Init();      								//��ʼ�Ǽƻ���Ӧ�÷������ģ����Ǻ���û���ý�ȥ�����Բ��������
	Key_Init();       								//������ʼ��
	OLED_Init();	    								//OLED��ʼ��
	Motor_Init();    								 //�������������ų�ʼ��         
	Grayscale_Init();              	 //�Ҷȴ�������ʼ��
	USART1_Init(115200);             //����1��ʼ����ͨ������һ��Ҫ��openmv��һ�£�����޷�ͨ�ţ�
	TIM3_PWM_Init(7199,0);          //��ʱ��PWMģʽ��ʼ��(10KHZ)
	Encoder_Init_TIM2(65535,0);     //��ʱ��2������ģʽ��ʼ��
	Encoder_Init_TIM4(65535,0);     //��ʱ��4������ģʽ��ʼ��
	TIM1_Getsample_Int(499,719);		//5ms����ʱ�жϣ����ڼ�ʱ��������ͽ���PID�㷨(**�жϺ�������TINE.c�ļ���**)

	OLED_Clear();                   //����һ�£���ֹOLED������(��Ҳ����ΪʲôҪ���������������ܶ������OLED���������)
	
	while(1)
	{	
		KEY_number();                //ɨ�谴������ͨ������������ˣ�
		OLED_ShowNum(1,8,mode,1);    //��ʾһ�����ĸ�ģʽ
		
		if(mode==1)                //ǰ�˲���ִ�����
		{
			if(RxData=='1')	
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_15);//��ƣ�����ͷʶ��1��ƾ�������Ϊ��ʱ��openmv�Ǳ�û����ʾ�������Ծ������������һ���ơ������ݽ��ܳɹ��͵�����
				while(1)	Home1();
			}
			if(RxData=='2')	
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_14);//�̵�
				while(1)	Home2();
			}
		}
		
		if(mode==2)							//�ж˲���ִ�����
		{
			if(RxData>='3'&&RxData<='8')
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_14); //�̵�
				while(1)
				{
					Home_M();
					if(RxData=='1'||RxData=='2') GPIO_SetBits(GPIOC,GPIO_Pin_15);//���
				}
			}
		}
		
		if(mode==3)						//Զ�˲���ִ�����
		{
			if(RxData>='3'&&RxData<='8')
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_14); //�̵�
				while(1)
				{
					Home_F();
					if(RxData=='1'||RxData=='2') GPIO_SetBits(GPIOC,GPIO_Pin_15);//���
				}
			}
		}
		
		//�������ľ��ǵ�PID��ʾ����ʱ�ã��Ҿ����ⲻɾ�ˡ��ɺ��ԣ�
		OLED_ShowSignedNum(1,1,Actual1,5);
		OLED_ShowSignedNum(2,1,Actual2,5);
		OLED_ShowSignedNum(3,1,PID_speed1,4);
		OLED_ShowSignedNum(4,1,PID_speed2,4);
	}
}

