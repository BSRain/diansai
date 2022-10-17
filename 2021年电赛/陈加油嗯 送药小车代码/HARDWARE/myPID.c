#include "myPID.h"


extern float VKp,VKi,Vkd;    //�ٶȻ�����
extern float Targer;
//float velocity_sum=0;    //�ٶȵ��ۼ�
int Pwm1,Pwm2;

struct PID pid_Position_turn={155,0,1200};
struct PID pid_Position_turn1={50,0,1200};


int Position_control_left(long Encoder,long Target)
{ 	
	 static int Bias,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;	//����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 if(Integral_bias>100000)Integral_bias=10000;
	 if(Integral_bias<-100000)Integral_bias=-10000;
	 Pwm1=pid_Position_turn.kp*Bias+pid_Position_turn.ki*Integral_bias+pid_Position_turn.kd*(Bias-Last_Bias);       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return (int)Pwm1;                                           //�������
}


int Position_control_right(long Encoder,long Target)
{ 	
	 static int Bias,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;	//����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 if(Integral_bias>100000)Integral_bias=10000;
	 if(Integral_bias<-100000)Integral_bias=-10000;
	 Pwm2=pid_Position_turn1.kp*Bias+pid_Position_turn1.ki*Integral_bias+pid_Position_turn1.kd*(Bias-Last_Bias);       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return (int)Pwm2;                                           //�������
}


//�ٶȻ���
float velocity_PI_value1(int velocity)
{ 
	static float error_actual;
  	static float last_err=0;
	static float PWM;
	static float velocity_sum=0;    //�ٶȵ��ۼ�
	velocity=-velocity;
	error_actual=Targer-velocity;
	velocity_sum +=error_actual;  	//�ٶȵ��ۼ�
	last_err=error_actual;
//	PWM+=VKp*error_actual + VKi*velocity_sum+Vkd*(error_actual-last_err);
	PWM+=VKp*(error_actual-last_err) + VKi*error_actual;
	if(PWM<-5000)    PWM=-5000;
	if(PWM>5000) PWM=5000;
	
	return PWM;
}

float velocity_PI_value2(int velocity)
{  
	static float error_actual;
  static float last_err=0;
	static float PWM;
	static float velocity_sum=0;    //�ٶȵ��ۼ�
	velocity=-velocity;
	error_actual=Targer-velocity;
	velocity_sum +=error_actual;  	//�ٶȵ��ۼ�
	last_err=error_actual;
	//PWM+=VKp*error_actual + VKi*velocity_sum+Vkd*(error_actual-last_err);
	PWM+=VKp*(error_actual-last_err) + VKi*error_actual;
  if(PWM<-5000)    PWM=-5000;
	if(PWM>5000) PWM=5000;
	
	return PWM;
}

