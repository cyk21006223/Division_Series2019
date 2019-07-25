#include "PID.h"


/**
* @brief	限制最大输出
* @param [in] 输入值
*				 [in] 限制的最大值
* @retval None
*/
void abs_limit(float *a, float ABS_MAX)
{
    if(*a > ABS_MAX)
        *a = ABS_MAX;
    if(*a < -ABS_MAX)
        *a = -ABS_MAX;
}
/**
* @brief	增量式PID-kp\ki\kd参数重设
* @param [in] 增量式PID指针
*				 [in] kp
*				 [in] ki
*				 [in] kd
* @retval None
*/
void IncrementalPID_ResetPara ( incrementalpid_t *pid_t, \
																float kp, float ki, float kd) \
{
	pid_t->Kp = kp;
	pid_t->Ki = ki;
	pid_t->Kd = kd;	
}
/**
* @brief	增量式PID-参数初始化
* @param [in] 增量式PID指针
*				 [in] kp
*				 [in] ki
*				 [in] kd
*				 [in] MaxOutput最大输出
*				 [in] IntegralLimit积分限幅
* @retval None
*/
void IncrementalPID_SetPara ( incrementalpid_t *pid_t, \
															float kp, float ki, float kd, \
															uint32_t MaxOutput, \
															uint32_t IntegralLimit) \
{
	pid_t->Target = 0;
	pid_t->Measured = 0;
	pid_t->err = 0;
	pid_t->err_last = 0;
	pid_t->err_beforeLast = 0;
	pid_t->err_doubleLast = 0;
	pid_t->Kp = kp;
	pid_t->Ki = ki;
	pid_t->Kd = kd;
	pid_t->MaxOutput = MaxOutput;
	pid_t->IntegralLimit = IntegralLimit;
	pid_t->pwm = 0; 			
}
/**
* @brief	增量式PID-目标值与当前值
* @param [in]  增量式PID指针
*				 [in]  目标值
*				 [in]  当前值
* @retval [out] pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
*/
int Incremental_PID(incrementalpid_t *pid_t, float target, float measured)
{
	float p_out, i_out, d_out;
	pid_t->Target = target;
	pid_t->Measured = measured;
	pid_t->err = pid_t->Target - pid_t->Measured;
	
	p_out = pid_t->Kp*(pid_t->err - pid_t->err_last);
	i_out = pid_t->Ki*pid_t->err;	
	d_out = pid_t->Kd*(pid_t->err - 2*pid_t->err_last + pid_t->err_beforeLast);

	
	//积分限幅
	abs_limit(&i_out, pid_t->IntegralLimit);
	
	pid_t->pwm += (p_out + i_out + d_out);
	
	//输出限幅
	abs_limit(&pid_t->pwm, pid_t->MaxOutput);
	
	pid_t->err_doubleLast = pid_t->err_beforeLast;
	pid_t->err_beforeLast = pid_t->err_last;
	pid_t->err_last = pid_t->err;

	return (int)pid_t->pwm;
}
/**
* @brief	增量式PID-积分分离-目标值与当前值
* @param [in]  增量式PID指针
*				 [in]  目标值
*				 [in]  当前值
* @retval [out] pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
*/
int Incremental_Integral_Separation_PID(incrementalpid_t *pid_t, \
	                                      float Epsilon, \
																				float target, \
																				float measured)
{
	float p_out, i_out, d_out,index;
	pid_t->Target = target;
	pid_t->Measured = measured;
	pid_t->err = pid_t->Target - pid_t->Measured;
	
	//积分分离
	if(abs(pid_t->err)<Epsilon)
		index = 0.0f;
	else index = 1.0f;
	
	p_out = pid_t->Kp*(pid_t->err - pid_t->err_last);
	i_out = pid_t->Ki*pid_t->err*index;	
	d_out = pid_t->Kd*(pid_t->err - 2*pid_t->err_last + pid_t->err_beforeLast);

	
	//积分限幅
	abs_limit(&i_out, pid_t->IntegralLimit);
	
	pid_t->pwm += (p_out + i_out + d_out);
	
	//输出限幅
	abs_limit(&pid_t->pwm, pid_t->MaxOutput);
	
	pid_t->err_doubleLast = pid_t->err_beforeLast;
	pid_t->err_beforeLast = pid_t->err_last;
	pid_t->err_last = pid_t->err;

	return (int)pid_t->pwm;
}

/**
* @brief	位置式PID-参数初始化
* @param [in] 位置式PID指针
*				 [in] kp
*				 [in] ki
*				 [in] kd
*				 [in] MaxOutput最大输出
*				 [in] IntegralLimit积分限幅
* @retval None
*/
void PositionPID_setPara ( positionpid_t *pid_t, \
													float kp, float ki, float kd, \
													uint32_t MaxOutput, \
													uint32_t IntegralLimit) \
{
	pid_t->Target = 0;
	pid_t->Measured = 0;
	pid_t->MaxOutput = MaxOutput;
	pid_t->IntegralLimit = IntegralLimit;
	pid_t->err = 0;
	pid_t->err_last = 0;
	pid_t->err_beforeLast = 0;
	pid_t->integral_err = 0;
	pid_t->Kp = kp;
	pid_t->Ki = ki;
	pid_t->Kd = kd;
	pid_t->pwm = 0;
}
/**
* @brief	位置式PID-kp\ki\kd参数重设
* @param [in] 位置式PID指针
*				 [in] kp
*				 [in] ki
*				 [in] kd
* @retval None
*/
void PositionPID_ResetPara(positionpid_t *pid_t, float kp, float ki, float kd)
{
//	pid_t->err = 0;
//	pid_t->err_last = 0;
//	pid_t->integral_err = 0;
	pid_t->Kp = kp;
	pid_t->Ki = ki;
	pid_t->Kd = kd;		
//	pid_t->pwm = 0;	
}

/**
* @brief	位置式PID--目标值与当前值
* @param  [in]  位置式PID-指针
*				  [in]  目标值
*				  [in]  当前值
* @retval [out] pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
*/
float Position_PID(positionpid_t *pid_t, float target, float measured)
{
	float p_out, i_out, d_out;
	pid_t->Target = (float)target;
	pid_t->Measured = (float)measured;
	pid_t->err = pid_t->Target - pid_t->Measured;
	
	pid_t->integral_err += pid_t->err;
	
	p_out = pid_t->Kp*pid_t->err;
	i_out = pid_t->Ki*pid_t->integral_err;
	d_out = pid_t->Kd*(pid_t->err - pid_t->err_last);
	
	//积分限幅
	abs_limit(&i_out, pid_t->IntegralLimit);
	
	pid_t->pwm = (p_out + i_out + d_out);
	
	//输出限幅
	abs_limit(&pid_t->pwm, pid_t->MaxOutput);
	
	pid_t->err_last = pid_t->err;

	return pid_t->pwm;
}

/**
* @brief	递推位置式PID--目标值与当前值
* @param [in]  位置式PID-指针
*				 [in]  目标值
*				 [in]  当前值
* @retval [out] pwm=pwm(k-1)+(Kp+Ki+Kd)e(k)-(Kp+2*Kd)e(k-1)+Kd*e(k-2)
*/
float Position_Recursion_PID(positionpid_t *pid_t, float target, float measured)
{
	pid_t->Target = (float)target;
	pid_t->Measured = (float)measured;
	pid_t->err = pid_t->Target - pid_t->Measured;

	pid_t->pwm += (pid_t->Kp + pid_t->Ki + pid_t->Kd) * pid_t->err \
	            -(pid_t->Kp + 2*pid_t->Kd) * pid_t->err_last \
	            + pid_t->Kd * pid_t->err_beforeLast;
	
	pid_t->err_beforeLast = pid_t->err_last;
	pid_t->err_last = pid_t->err;
	
	//输出限幅
	abs_limit(&pid_t->pwm, pid_t->MaxOutput);
	
	return pid_t->pwm;
}

/**
* @brief	积分分离的位置式PID--目标值与当前值
* @param [in] 位置式PID-指针
*				 [in] 可允许误差范围
*				 [in] 目标值
*				 [in] 当前值
* @retval [out] pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
*/
float Position_Integral_Separation_PID(positionpid_t *pid_t,float Epsilon, \
	                                     float target, float measured)
{
	float p_out, i_out, d_out, index;
	pid_t->Target = (float)target;
	pid_t->Measured = (float)measured;
	pid_t->err = pid_t->Target - pid_t->Measured;
	
	//积分分离
	if(abs(pid_t->err)<=Epsilon)
		index = 0.0f;
	else{
		index = 1.0f;
		pid_t->integral_err += pid_t->err;
	}
	
	p_out = pid_t->Kp*pid_t->err;
	i_out = pid_t->Ki*pid_t->integral_err;
	d_out = pid_t->Kd*(pid_t->err - pid_t->err_last);
	
	//积分限幅
	abs_limit(&i_out, pid_t->IntegralLimit);
	
	pid_t->pwm = (p_out + i_out*index + d_out);
	
	//输出限幅
	abs_limit(&pid_t->pwm, pid_t->MaxOutput);
	
	pid_t->err_last = pid_t->err;

	return pid_t->pwm;
}
/**
* @brief	抗积分饱和的位置式PID--目标值与当前值
* @param [in] 位置式PID-指针
*				 [in] 可允许误差范围
*				 [in] 控制量饱和区范围，大于Umax累加负偏差
*				 [in] 控制量饱和区范围，小于Umin累加正偏差
*				 [in] 目标值
*				 [in] 当前值
* @retval [out] pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
*/
float Position_Anti_Windup_PID(positionpid_t *pid_t,float Epsilon, \
												float Umax, float Umin, float target, float measured)
{
	int index;
	float p_out, i_out, d_out;
	pid_t->Target = (float)target;
	pid_t->Measured = (float)measured;
	pid_t->err = pid_t->Target - pid_t->Measured;
	
	if(pid_t->Measured>Umax) // 抗积分饱和的实现
	{
		if(abs(pid_t->err)>Epsilon) // 积分分离过程
		{
			index=0;
		}
		else
		{
			index=1;
			if(pid_t->err<0)
			{
				pid_t->integral_err+=pid_t->err;
			}
		}
	}
	else if(pid_t->Measured<Umin)
	{
		if(abs(pid_t->err)>Epsilon) // 积分分离过程
		{
			index=0;
		}
		else
		{
		index=1;
			if(pid_t->err>0)
			{
				pid_t->integral_err+=pid_t->err;
			}
		}
	}
	else
	{
		if(abs(pid_t->err)>Epsilon) // 积分分离过程
		{
			index=0;
		}
		else
		{
			index=1;
			pid_t->integral_err+=pid_t->err;
		}
	}

	pid_t->integral_err += pid_t->err;
	
	p_out = pid_t->Kp*pid_t->err;
	i_out = pid_t->Ki*pid_t->integral_err;
	d_out = pid_t->Kd*(pid_t->err - pid_t->err_last);
	
	//积分限幅
	abs_limit(&i_out, pid_t->IntegralLimit);
	
	pid_t->pwm = (p_out + i_out*index + d_out);
	
	//输出限幅
	abs_limit(&pid_t->pwm, pid_t->MaxOutput);
	
	pid_t->err_last = pid_t->err;

	return pid_t->pwm;
}
/**
* @brief	抗积分饱和+积分梯形的位置式PID--目标值与当前值
* @param [in] 位置式PID-指针
*				 [in] 可允许误差范围
*				 [in] 控制量饱和区范围，大于Umax累加负偏差
*				 [in] 控制量饱和区范围，小于Umin累加正偏差
*				 [in] 目标值
*				 [in] 当前值
* @retval [out] pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
*/
float Position_Integral_Trapezium_PID(positionpid_t *pid_t,float Epsilon, \
												float Umax, float Umin, float target, float measured)
{
	int index;
	float p_out, i_out, d_out;
	pid_t->Target = (float)target;
	pid_t->Measured = (float)measured;
	pid_t->err = pid_t->Target - pid_t->Measured;
	
	if(pid_t->Measured>Umax) // 抗积分饱和的实现
	{
		if(abs(pid_t->err)>Epsilon) // 积分分离过程
		{
			index=0;
		}
		else
		{
			index=1;
			if(pid_t->err<0)
			{
				pid_t->integral_err+=pid_t->err;
			}
		}
	}
	else if(pid_t->Measured<Umin)
	{
		if(abs(pid_t->err)>Epsilon) // 积分分离过程
		{
			index=0;
		}
		else
		{
		index=1;
			if(pid_t->err>0)
			{
				pid_t->integral_err+=pid_t->err;
			}
		}
	}
	else
	{
		if(abs(pid_t->err)>Epsilon) // 积分分离过程
		{
			index=0;
		}
		else
		{
			index=1;
			pid_t->integral_err+=pid_t->err;
		}
	}

	pid_t->integral_err += pid_t->err;
	
	p_out = pid_t->Kp*pid_t->err;
	i_out = pid_t->Ki*pid_t->integral_err*(float)0.5;
	d_out = pid_t->Kd*(pid_t->err - pid_t->err_last);
	
	//积分限幅
	abs_limit(&i_out, pid_t->IntegralLimit);
	
	pid_t->pwm = (p_out + i_out*index + d_out);
	
	//输出限幅
	abs_limit(&pid_t->pwm, pid_t->MaxOutput);
	
	pid_t->err_last = pid_t->err;

	return pid_t->pwm;
}
/**
* @brief	变积分的位置式PID--目标值与当前值
* @param [in] 位置式PID-指针
*				 [in] 可允许误差范围
*				 [in] 目标值
*				 [in] 当前值
* @retval [out] pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
*/
float Position_Variable_Integral_PID(positionpid_t *pid_t,
							float EpsilonMax, float EpsilonMin,float target, float measured)
{
	float p_out, i_out, d_out, index;
	pid_t->Target = (float)target;
	pid_t->Measured = (float)measured;
	pid_t->err = pid_t->Target - pid_t->Measured;
	
	//积分分离
	if(abs(pid_t->err)<EpsilonMin)
		index = 0.0f;
	else if(abs(pid_t->err)>EpsilonMax)
	{
		index = 1.0f;
	}
	else{
		index=(EpsilonMax-abs(pid_t->err))/(EpsilonMax-EpsilonMin);
	}
	
	pid_t->integral_err += pid_t->err;
	
	p_out = pid_t->Kp*pid_t->err;
	i_out = pid_t->Ki*pid_t->integral_err;
	d_out = pid_t->Kd*(pid_t->err - pid_t->err_last);
	
	//积分限幅
	abs_limit(&i_out, pid_t->IntegralLimit);
	
	pid_t->pwm = (p_out + i_out*index + d_out);
	
	//输出限幅
	abs_limit(&pid_t->pwm, pid_t->MaxOutput);
	
	pid_t->err_last = pid_t->err;

	return pid_t->pwm;
}

/********************************* 模糊pid ************************************/
#define NB   0
#define NM	 1
#define NS	 2
#define ZO	 3
#define PS	 4
#define PM	 5
#define PB	 6


float errorrate;
 
//论域
static const int16_t e_domin[7] = {-100, -70, -30, 0, 30, 50, 100};
static const int16_t ec_domin[7] = {-15, -10, -5, 0, 5, 10, 15};

//static const	float KpRule[7] = {0, -5, -15, -25, 0};
//static const	float KdRule[7] = {0, -30, -50, -70, 0};

//#define KpB  2.2
//#define KiB  25*5
//#define KdB  0.9

//static const	float KpRule[5] = {0.3*KpB, 0.36*KpB, 0.42*KpB, 0.48*KpB, 0.5*KpB};
//static const	float KiRule[5] = {2.0*KiB, 2.8*KiB, 3.0*KiB, 3.8*KiB, 4.0*KiB};
//static const	float KdRule[5] = {2.5*KdB, 2.9*KdB, 3.3*KdB, 3.5*KdB, 4.3*KdB};
static const float KpRule[5] = {0.3, 0.36, 0.42, 0.48, 0.5};
static const float KiRule[5] = {2.0, 2.8, 3.0, 3.8, 4.0};
static const float KdRule[5] = {2.5, 2.9, 3.3, 3.5, 4.3};

static const u8 fuzzyRuleKp[7][7]={
	3, 3, 3, 3, 3, 3, 3,
	2, 2, 2, 2, 1, 2, 2,
	0, 0, 1, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 1, 0, 1, 0, 0, 1,
	2, 2, 2, 2, 1, 2, 2,
	3, 3, 3, 3, 3, 3, 3
};

static const u8 fuzzyRuleKi[7][7]={
	NB,	NB,	NB,	NM,	NM,	ZO,	ZO,
	NB,	NB,	NM,	NM,	NS,	ZO,	ZO,
	NM,	NM,	NS,	NS,	ZO,	PS,	PS,
	NM,	NS,	NS,	ZO,	PS,	PS,	PM,
	NS,	NS,	ZO,	PS,	PS,	PM,	PM,
	ZO,	ZO,	PS,	PM,	PM,	PB,	PB,
	ZO,	ZO,	PS,	PM,	PB,	PB,	PB
};
 
static const u8 fuzzyRuleKd[7][7]={
	{3,3,3,2,2,2,2},
	{2,2,2,1,1,1,1},
	{3,3,3,3,3,3,3},
	{2,2,1,1,1,2,2},
	{3,3,2,2,3,3,3},
	{2,2,1,0,1,1,1},
	{3,3,3,3,2,3,2}
};
 
void FuzzyPIDCalc(FuzzyPID_t *fuzzy,float TargetValue,float RealValue)
{
	uint8_t E_Domin;//偏差论域标识
	uint8_t Ec_Domin;//偏差变化率论域标识
	float E_LeftDegree,E_RightDegree;//隶属度
	float Ec_LeftDegree,Ec_RightDegree;//隶属度
	
	fuzzy->Error = TargetValue - RealValue;
	
	fuzzy->ErrorRate = fuzzy->Error - fuzzy->LastError;
	
	//偏差e模糊化
	if (fuzzy->Error < e_domin[NB])
	{
		E_Domin = NB;
		E_LeftDegree = 0;
		E_RightDegree = 1;
	}
	else if (fuzzy->Error < e_domin[NM])
	{
		E_Domin = NM;
		E_RightDegree = (fuzzy->Error - e_domin[NM])/(e_domin[NB] - e_domin[NM]);
		E_LeftDegree = 1 - E_RightDegree;
	}
	else if (fuzzy->Error < e_domin[NS])
	{
		E_Domin = NS;
		E_RightDegree = (fuzzy->Error - e_domin[NS])/(e_domin[NM] - e_domin[NS]);
		E_LeftDegree = 1 - E_RightDegree;
	}
	else if (fuzzy->Error < e_domin[ZO])
	{
		E_Domin = ZO;
		E_RightDegree = (fuzzy->Error - e_domin[ZO])/(e_domin[NS] - e_domin[ZO]);
		E_LeftDegree = 1 - E_RightDegree;
	}
	else if (fuzzy->Error < e_domin[PS])
	{
		E_Domin = PS;
		E_RightDegree = (fuzzy->Error - e_domin[PS])/(e_domin[ZO] - e_domin[PS]);
		E_LeftDegree = 1 - E_RightDegree;
	}
	else if (fuzzy->Error < e_domin[PM])
	{
		E_Domin = PM;
		E_RightDegree = (fuzzy->Error - e_domin[PM])/(e_domin[PS] - e_domin[PM]);
		E_LeftDegree = 1 - E_RightDegree;
	}
	else if (fuzzy->Error < e_domin[PB])
	{
		E_Domin = PB;
		E_RightDegree = (fuzzy->Error - e_domin[PB])/(e_domin[PM] - e_domin[PB]);
		E_LeftDegree = 1 - E_RightDegree;
	}
	else
	{
		E_Domin = PB;
		E_LeftDegree = 1;
		E_RightDegree = 0;		
	}
	
	//偏差变化率ec模糊化
	if (fuzzy->Error < ec_domin[NB])
	{
		Ec_Domin = NB;
		Ec_LeftDegree = 0;
		Ec_RightDegree = 1;	
	}
	else if (fuzzy->Error < ec_domin[NM])
	{
		Ec_Domin = NM;
		Ec_RightDegree = (fuzzy->Error - ec_domin[NM])/(ec_domin[NB] - ec_domin[NM]);
		Ec_LeftDegree = 1 - Ec_RightDegree;
	}
	else if (fuzzy->Error < ec_domin[NS])
	{
		Ec_Domin = NS;
		Ec_RightDegree = (fuzzy->Error - ec_domin[NS])/(ec_domin[NM] - ec_domin[NS]);
		Ec_LeftDegree = 1 - Ec_RightDegree;
	}
	else if (fuzzy->Error < ec_domin[ZO])
	{
		Ec_Domin = ZO;
		Ec_RightDegree = (fuzzy->Error - ec_domin[ZO])/(ec_domin[NS] - ec_domin[ZO]);
		Ec_LeftDegree = 1 - Ec_RightDegree;
	}
	else if (fuzzy->Error < ec_domin[PS])
	{
		Ec_Domin = PS;
		Ec_RightDegree = (fuzzy->Error - ec_domin[PS])/(ec_domin[ZO] - ec_domin[PS]);
		Ec_LeftDegree = 1 - Ec_RightDegree;
	}
	else if (fuzzy->Error < ec_domin[PM])
	{
		Ec_Domin = PM;
		Ec_RightDegree = (fuzzy->Error - ec_domin[PM])/(ec_domin[PS] - ec_domin[PM]);
		Ec_LeftDegree = 1 - Ec_RightDegree;
	}
	else if (fuzzy->Error < ec_domin[PB])
	{
		Ec_Domin = PB;
		Ec_RightDegree = (fuzzy->Error - ec_domin[PB])/(ec_domin[PM] - ec_domin[PB]);
		Ec_LeftDegree = 1 - Ec_RightDegree;
	}
	else
	{
		Ec_Domin = PB;
		Ec_LeftDegree = 1;
		Ec_RightDegree = 0;		
	}
	
//	/*Kp*/
//	if(E_Domin != NB)
//	{
//		fuzzy->Kp = E_LeftDegree * Ec_LeftDegree * KpRule[fuzzyRuleKp[E_Domin - 1][Ec_Domin - 1]];
//		fuzzy->Kp += E_LeftDegree * Ec_RightDegree * KpRule[fuzzyRuleKp[E_Domin - 1][Ec_Domin]];
//		fuzzy->Kp += E_RightDegree * Ec_LeftDegree * KpRule[fuzzyRuleKp[E_Domin][Ec_Domin - 1]];
//		fuzzy->Kp += E_RightDegree * Ec_RightDegree * KpRule[fuzzyRuleKp[E_Domin][Ec_Domin]];
//	}
//	else
//	{
//		fuzzy->Kp = KpRule[fuzzyRuleKp[E_Domin][Ec_Domin]];
//	}
//	/*Ki*/
//	if(Ec_Domin != NB)
//	{
//		fuzzy->Ki = E_LeftDegree * Ec_LeftDegree * KiRule[fuzzyRuleKi[E_Domin - 1][Ec_Domin - 1]];
//		fuzzy->Ki += E_LeftDegree * Ec_RightDegree * KiRule[fuzzyRuleKi[E_Domin - 1][Ec_Domin]];
//		fuzzy->Ki += E_RightDegree * Ec_LeftDegree * KiRule[fuzzyRuleKi[E_Domin][Ec_Domin - 1]];
//		fuzzy->Ki += E_RightDegree * Ec_RightDegree * KiRule[fuzzyRuleKi[E_Domin][Ec_Domin]];
//	}
//	else
//	{
//		fuzzy->Ki = KiRule[fuzzyRuleKi[E_Domin][Ec_Domin]];
//	}
//	/*Kd*/
//	if(Ec_Domin != NB)
//	{
//		fuzzy->Kd = E_LeftDegree * Ec_LeftDegree * KdRule[fuzzyRuleKd[E_Domin - 1][Ec_Domin - 1]];
//		fuzzy->Kd += E_LeftDegree * Ec_RightDegree * KdRule[fuzzyRuleKd[E_Domin - 1][Ec_Domin]];
//		fuzzy->Kd += E_RightDegree * Ec_LeftDegree * KdRule[fuzzyRuleKd[E_Domin][Ec_Domin - 1]];
//		fuzzy->Kd += E_RightDegree * Ec_RightDegree * KdRule[fuzzyRuleKd[E_Domin][Ec_Domin]];
//	}
//	else
//	{
//		fuzzy->Kd = KdRule[fuzzyRuleKd[E_Domin][Ec_Domin]];
//	}

		/*Kp*/
	if(E_Domin != NB)
	{
		fuzzy->Kp = E_LeftDegree * Ec_LeftDegree * fuzzy->Kp_Rule[fuzzyRuleKp[E_Domin - 1][Ec_Domin - 1]];
		fuzzy->Kp += E_LeftDegree * Ec_RightDegree * fuzzy->Kp_Rule[fuzzyRuleKp[E_Domin - 1][Ec_Domin]];
		fuzzy->Kp += E_RightDegree * Ec_LeftDegree * fuzzy->Kp_Rule[fuzzyRuleKp[E_Domin][Ec_Domin - 1]];
		fuzzy->Kp += E_RightDegree * Ec_RightDegree * fuzzy->Kp_Rule[fuzzyRuleKp[E_Domin][Ec_Domin]];
	}
	else
	{
		fuzzy->Kp = fuzzy->Kp_Rule[fuzzyRuleKp[E_Domin][Ec_Domin]];
	}
	/*Ki*/
	if(Ec_Domin != NB)
	{
		fuzzy->Ki = E_LeftDegree * Ec_LeftDegree * fuzzy->Ki_Rule[fuzzyRuleKi[E_Domin - 1][Ec_Domin - 1]];
		fuzzy->Ki += E_LeftDegree * Ec_RightDegree * fuzzy->Ki_Rule[fuzzyRuleKi[E_Domin - 1][Ec_Domin]];
		fuzzy->Ki += E_RightDegree * Ec_LeftDegree * fuzzy->Ki_Rule[fuzzyRuleKi[E_Domin][Ec_Domin - 1]];
		fuzzy->Ki += E_RightDegree * Ec_RightDegree * fuzzy->Ki_Rule[fuzzyRuleKi[E_Domin][Ec_Domin]];
	}
	else
	{
		fuzzy->Ki = fuzzy->Ki_Rule[fuzzyRuleKi[E_Domin][Ec_Domin]];
	}
	/*Kd*/
	if(Ec_Domin != NB)
	{
		fuzzy->Kd = E_LeftDegree * Ec_LeftDegree * fuzzy->Kd_Rule[fuzzyRuleKd[E_Domin - 1][Ec_Domin - 1]];
		fuzzy->Kd += E_LeftDegree * Ec_RightDegree * fuzzy->Kd_Rule[fuzzyRuleKd[E_Domin - 1][Ec_Domin]];
		fuzzy->Kd += E_RightDegree * Ec_LeftDegree * fuzzy->Kd_Rule[fuzzyRuleKd[E_Domin][Ec_Domin - 1]];
		fuzzy->Kd += E_RightDegree * Ec_RightDegree * fuzzy->Kd_Rule[fuzzyRuleKd[E_Domin][Ec_Domin]];
	}
	else
	{
		fuzzy->Kd = fuzzy->Kd_Rule[fuzzyRuleKd[E_Domin][Ec_Domin]];
	}
	//pid运算
	fuzzy->KpOut = fuzzy->Kp*fuzzy->Error;
	fuzzy->KiOut = fuzzy->Ki*fuzzy->Integral;
	fuzzy->KdOut = fuzzy->Kd*fuzzy->ErrorRate;
	
	fuzzy->Output = fuzzy->KpOut + fuzzy->KiOut + fuzzy->KdOut;
	
		//限幅
	abs_limit(&fuzzy->Output, fuzzy->OutputMax);
	fuzzy->PrevError = fuzzy->LastError;
	fuzzy->LastError = fuzzy->Error;
	
}
/**
* @brief	模糊PID-参数初始化
* @param [in] 模糊PID指针
*				 [in] kp_param  Kp规则系数
*				 [in] ki_param  Ki规则系数
*				 [in] kd_param  Kd规则系数
*				 [in] MaxOutput最大输出
* @retval None
*/
void FuzzyPid_setPara(FuzzyPID_t *fuzzy, \
											float kp_param, float ki_param, float kd_param, \
											uint32_t MaxOutput)
{
	int i;
	for(i=0;i<5;i++)
	{
	  fuzzy->Kp_Rule[i] = kp_param * KpRule[i];
		fuzzy->Ki_Rule[i] = ki_param * KiRule[i];
		fuzzy->Kd_Rule[i] = kd_param * KdRule[i];
	}

	fuzzy->OutputMax = MaxOutput;
}



/********************************* 模糊pid ************************************/

