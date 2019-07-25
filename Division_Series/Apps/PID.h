#ifndef __PID_H
#define __PID_H

/******************************************************************************/
/*Lib*/
#include "stm32f4xx.h"
/*User*/
#include "UserMath.h"

/******************************************************************************/
/*����ʽPID*/
typedef struct{
	float Target; 					//�趨Ŀ��ֵ
	float Measured; 				//����ֵ
	float err; 						//����ƫ��ֵ
	float err_last; 				//��һ��ƫ��
	float err_beforeLast; 			//���ϴ�ƫ��
	float err_doubleLast;   //�����ϴ�ƫ��
	float Kp, Ki, Kd;				//Kp, Ki, Kd����ϵ��
	float pwm; 						//pwm��� 
	uint32_t MaxOutput;				//����޷�
  uint32_t IntegralLimit;			//�����޷� 
}incrementalpid_t;


/*λ��ʽPID*/
typedef struct{
	float Target; 					//�趨Ŀ��ֵ
	float Measured; 				//����ֵ
	float err; 						//����ƫ��ֵ
	float err_last; 				//��һ��ƫ��
	float err_beforeLast; 			//���ϴ�ƫ��
	float integral_err; 			//����ƫ��֮��
	float Kp, Ki, Kd;				//Kp, Ki, Kd����ϵ��
	float pwm; 						//pwm���
	uint32_t MaxOutput;				//����޷�
  uint32_t IntegralLimit;			//�����޷� 
}positionpid_t;


/*ģ��PID*/
typedef struct FuzzyPID_t
{
	float Kp;
	float Ki;
	float Kd;
	float KpOut;
	float KiOut;
	float KdOut;
	float Error;//ƫ��
	float LastError;//�ϴ�ƫ��
	float ErrorRate;//ƫ��仯��
	float PrevError;//���ϴ�ƫ��
	float Integral;//����
	float IOutMax;//��������޷�
	float Output;//pid���
	float OutputMax;//�������
	float Kp_Rule[5];
	float Ki_Rule[5];
	float Kd_Rule[5];
	void (*FCalc)(struct FuzzyPID_t *pid_t,float TargetValue,float RealValue);//����ָ��
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

