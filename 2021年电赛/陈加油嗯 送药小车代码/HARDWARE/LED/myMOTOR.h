#ifndef __MYMOTOR_H
#define __MYMOTOR_H	 

void Motor_Init(void);
void Front(void);
void Back(void);
void Left(void);
void Right(void);

void Big_Left(void); //×ó×ª
void Big_Right(void);

void Stop(void);

void PID_Position_left(void);
void PID_Position_right(void);
void Turn_Left_90du(void);
void Turn_Right_90du(void);
void Turn_180du(void);

#endif
