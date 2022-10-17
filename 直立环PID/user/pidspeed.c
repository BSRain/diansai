

/******************************************************************************/
//           左轮          右轮
//编码器     TIM3          TIM2
//PWM调速     正  反            正  反
//    TIM8_1  >0  =0    TIM8_3  =0  >0
//    TIM8_2  =0  >0    TIM8_4  >0  =0
/******************************************************************************/
void SpeedL_deal(void)
{
	long curr;
	long err;
	short tim3_cnt;
	float Kp=0.1,Ki=0.05;
	
	tim3_cnt=TIM3->CNT;
	TIM3->CNT=0;
	curr=tim3_cnt*50*60/28;  //计算实际转速=脉冲数*50*60/28 RPM
	
	err=speedL_set-curr;
	tim8L_ccr+=Kp*(err-speedL_e1)+Ki*err;
	speedL_e1=err;
	if(speedL_set==0)tim8L_ccr=0;
	if(tim8L_ccr<-5000)tim8L_ccr=-5000;
	if(tim8L_ccr> 5000)tim8L_ccr= 5000;
	if(tim8L_ccr==0)
		{
			TIM_SetCompare1(TIM8,0);
			TIM_SetCompare2(TIM8,0);
		}
	if(tim8L_ccr>0)  //正转
		{
			TIM_SetCompare1(TIM8,tim8L_ccr);
			TIM_SetCompare2(TIM8,0);
		}
	if(tim8L_ccr<0)  //反转
		{
			TIM_SetCompare1(TIM8,0);
			TIM_SetCompare2(TIM8,(-tim8L_ccr));
		}
}
/******************************************************************************/
void SpeedR_deal(void)//右轮
{
	long curr;
	long err;
	short tim2_cnt;
	float Kp=0.1,Ki=0.05;
	
	tim2_cnt=TIM2->CNT;
	TIM2->CNT=0;
	curr=tim2_cnt*50*60/28; //计算实际转速=脉冲数*50*60/28 RPM
	
	err=speedR_set-curr;
	tim8R_ccr+=Kp*(err-speedR_e1)+Ki*err;
	speedR_e1=err;
	if(speedR_set==0)tim8R_ccr=0;
	if(tim8R_ccr<-5000)tim8R_ccr=-5000;
	if(tim8R_ccr> 5000)tim8R_ccr= 5000;
	if(tim8R_ccr==0)
		{
			TIM_SetCompare3(TIM8,0);
			TIM_SetCompare4(TIM8,0);
		}
	if(tim8R_ccr>0)  //正转
		{
			TIM_SetCompare3(TIM8,0);
			TIM_SetCompare4(TIM8,tim8R_ccr);
		}
	if(tim8R_ccr<0)  //反转
		{
			TIM_SetCompare3(TIM8,(-tim8R_ccr));
			TIM_SetCompare4(TIM8,0);
		}
}
/******************************************************************************/




