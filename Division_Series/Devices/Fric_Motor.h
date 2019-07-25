#ifndef __FRIC_MOTOR_H
#define __FRIC_MOTOR_H
/******************************************************************************/
#include "Users.h"
/******************************************************************************/
#define FRICMOTOR_SPEEDMAX		2000-1
#define FRICMOTOR_SPEEDMIN		1000
/******************************************************************************/

typedef enum{
	FrictShutdown_Status = 0,//摩擦轮待机状态
	FrictNormalSpeed_Status = 1,//摩擦轮正常速度状态
	FrictHighSpeed_Status = 2,//摩擦轮高速状态
	FrictInvincibleSpeed_Status = 3,//摩擦轮无敌超速状态
}FrictWorkStatus_e;

typedef struct
{
	uint16_t   TargetSpeed;//目标速度
	uint16_t   Rampcount;//斜坡函数计数值
	int16_t    RampRate;//斜坡函数比率
	uint16_t   Maxcount;//计数最大值
}FrictWheelParam_t;

typedef struct{
	bool fricPro;                 //摩擦轮保护标志
	bool fricReady;								//摩擦轮准备标志
	bool fric_prestart;           //摩擦轮预启动标志
	bool onLineFlag;              //摩擦轮未离线标志
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


