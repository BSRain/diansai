#include <stdio.h>
int main()
{
    static int Bias, Integral_bias, Last_Bias;
    Bias = Encoder - Target; //计算偏差 现在-目标
    if (Bias >= 0)
    {
        //控制电机方向
    }
    else
    {
        //控制电机方向
    }
    Integral_bias += Bias; //求出偏差的积分 误差的累计
    if (Integral_bias > 100000)//积分限幅
        Integral_bias = 10000;
    if (Integral_bias < -100000)
        Integral_bias = -10000;
    Pwm1 = pid_Position_turn.kp * Bias + pid_Position_turn.ki * Integral_bias + pid_Position_turn.kd * (Bias - Last_Bias); //位置式PID控制器
    Last_Bias = Bias;      //保存上一次偏差
    return (int)Pwm1;      //增量输出
}