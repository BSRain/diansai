
#include <stdio.h>
#include "PID.h"
float Target=10.0;//cm/s
float Encoder;
float kp=0.1,ki=0,kd;
float v1;
float v2;
int pid()
{
    static float Bias, Integral_bias, Last_Bias;
    double Pwm1;

    Encoder=v1;
    Bias =  Target-Encoder; //计算偏差 现在-目标
    /*if (Bias >= 0)
    {
        //控制电机方向
    }
    else
    {
        //控制电机方向
    }*/
    Integral_bias += Bias; //求出偏差的积分 误差的累计
    if (Integral_bias > 100)//积分限幅
        Integral_bias = 100;
    if (Integral_bias < -100)
        Integral_bias = -100;
    Pwm1 = kp* Bias + ki* Integral_bias + kd * (Bias - Last_Bias); //位置式PID控制器
    Last_Bias = Bias;      //保存上一次偏差
    //pwm限幅
    return (int)Pwm1;      //增量输出
}
int pid2()
{
    static float Bias, Integral_bias, Last_Bias;
    double Pwm2;
    Encoder=v2;
    Bias =  Target-Encoder; //计算偏差 现在-目标
    /*if (Bias >= 0)
    {
        //控制电机方向
    }
    else
    {
        //控制电机方向
    }*/
    Integral_bias += Bias; //求出偏差的积分 误差的累计
    if (Integral_bias > 10000)//积分限幅
        Integral_bias = 10000;
    if (Integral_bias < -10000)
        Integral_bias = -10000;
    Pwm2 = kp* Bias + ki* Integral_bias + kd * (Bias - Last_Bias); //位置式PID控制器
    Last_Bias = Bias;      //保存上一次偏差
    //pwm限幅
    return (int)Pwm2;      //增量输出
}