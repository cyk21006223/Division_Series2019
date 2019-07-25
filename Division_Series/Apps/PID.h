#ifndef __PID_H
#define __PID_H

/******************************************************************************/
/*Lib*/
#include "stm32f4xx.h"
/*User*/
#include "UserMath.h"

/******************************************************************************/
/*增量式PID*/
typedef struct{
	float Target; 					//设定目标值
	float Measured; 				//测量值
	float err; 						//本次偏差值
	float err_last; 				//上一次偏差
	float err_beforeLast; 			//上上次偏差
	float err_doubleLast;   //上上上次偏差
	float Kp, Ki, Kd;				//Kp, Ki, Kd控制系数
	float pwm; 						//pwm输出 
	uint32_t MaxOutput;				//输出限幅
  uint32_t IntegralLimit;			//积分限幅 
}incrementalpid_t;


/*位置式PID*/
typedef struct{
	float Target; 					//设定目标值
	float Measured; 				//测量值
	float err; 						//本次偏差值
	float err_last; 				//上一次偏差
	float err_beforeLast; 			//上上次偏差
	float integral_err; 			//所有偏差之和
	float Kp, Ki, Kd;				//Kp, Ki, Kd控制系数
	float pwm; 						//pwm输出
	uint32_t MaxOutput;				//输出限幅
  uint32_t IntegralLimit;			//积分限幅 
}positionpid_t;


/*模糊PID*/
typedef struct FuzzyPID_t
{
	float Kp;
	float Ki;
	float Kd;
	float KpOut;
	float KiOut;
	float KdOut;
	float Error;//偏差
	float LastError;//上次偏差
	float ErrorRate;//偏差变化率
	float PrevError;//上上次偏差
	float Integral;//积分
	float IOutMax;//积分输出限幅
	float Output;//pid输出
	float OutputMax;//输出限制
	float Kp_Rule[5];
	float Ki_Rule[5];
	float Kd_Rule[5];
	void (*FCalc)(struct FuzzyPID_t *pid_t,float TargetValue,float RealValue);//函数指针
	void (*FRest)(struct FuzzyPID_t *pid_t);
}FuzzyPID_t;

/******************************************************************************/
void abs_limit(float *a, float ABS_MAX);
void IncrementalPID_ResetPara ( incrementalpid_t *pid_t, \
																float kp, float ki, float kd);
void IncrementalPID_SetPara ( incrementalpid_t *pid_t, \
															float kp, float ki, float kd, \
															uint32_t MaxOutput, \
															uint32_t IntegralLimit);
int Incremental_PID(incrementalpid_t *pid_t, float target, float measured);
int Incremental_Integral_Separation_PID(incrementalpid_t *pid_t, \
	                                      float Epsilon, \
																				float target, \
																				float measured);
void PositionPID_setPara ( positionpid_t *pid_t, \
													float kp, float ki, float kd, \
													uint32_t MaxOutput, \
													uint32_t IntegralLimit);
void PositionPID_ResetPara(positionpid_t *pid_t, float kp, float ki, float kd);
float Position_PID(positionpid_t *pid_t, float target, float measured);
float Position_Integral_Separation_PID(positionpid_t *pid_t,float Epsilon, \
	                                     float target, float measured);
float Position_Anti_Windup_PID(positionpid_t *pid_t,float Epsilon, \
												float Umax, float Umin, float target, float measured);
float Position_Integral_Trapezium_PID(positionpid_t *pid_t,float Epsilon, \
												float Umax, float Umin, float target, float measured);
float Position_Variable_Integral_PID(positionpid_t *pid_t,
							float EpsilonMax, float EpsilonMin,float target, float measured);

void FuzzyPIDCalc(FuzzyPID_t *fuzzy,float TargetValue,float RealValue);
void FuzzyPid_setPara(FuzzyPID_t *fuzzy, \
											float kp, float ki, float kd, \
											uint32_t MaxOutput);		
/******************************************************************************/


#endif /*PID.h*/

