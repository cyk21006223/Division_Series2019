#ifndef __FRIC_MOTOR_H
#define __FRIC_MOTOR_H
/******************************************************************************/
#include "Users.h"
/******************************************************************************/
#define FRICMOTOR_SPEEDMAX		2000-1
#define FRICMOTOR_SPEEDMIN		1000
/******************************************************************************/

typedef enum{
	FrictShutdown_Status = 0,//Ħ���ִ���״̬
	FrictNormalSpeed_Status = 1,//Ħ���������ٶ�״̬
	FrictHighSpeed_Status = 2,//Ħ���ָ���״̬
	FrictInvincibleSpeed_Status = 3,//Ħ�����޵г���״̬
}FrictWorkStatus_e;

typedef struct
{
	uint16_t   TargetSpeed;//Ŀ���ٶ�
	uint16_t   Rampcount;//б�º�������ֵ
	int16_t    RampRate;//б�º�������
	uint16_t   Maxcount;//�������ֵ
}FrictWheelParam_t;

typedef struct{
	bool fricPro;                 //Ħ���ֱ�����־
	bool fricReady;								//Ħ����׼����־
	bool fric_prestart;           //Ħ����Ԥ������־
	bool onLineFlag;              //Ħ����δ���߱�־
}FrictMotor_t;
/******************************************************************************/
extern FrictWorkStatus_e FrictWorkStatus;
extern FrictMotor_t      FrictMotor;
/******************************************************************************/
void Fric_Init(void);
void fric_PreStart(void);
void fric_turnOff(void);
void fricMotor_setSpeed(uint16_t speed);
void FrictWheelControl(FrictWorkStatus_e FrictStatus);
FrictWorkStatus_e GetFrictWorkStatus(void);
/******************************************************************************/


#endif /* __FRIC_MOTOR_H */


