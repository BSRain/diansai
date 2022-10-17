#ifndef __myPID_H
#define __myPID_H	 

struct PID
{
 int kp;
 float ki;
 int kd;
};


int Position_control_left(long Encoder,long Target);
int Position_control_right(long Encoder,long Target);

float velocity_PI_value1(int velocity);
float velocity_PI_value2(int velocity);
	
#endif
