#ifndef __M6020_MOTOR_H
#define __M6020_MOTOR_H

/******************************************************************************/
#include "Users.h"
/******************************************************************************/
#define M6020_FIRST_ID	  			0x205               ////0x205~0x208
#define YAWMOTOR_RECEIVEID      0x205
#define M6020_SENDID		        0x1FF
/******************************************************************************/

typedef struct
{
	incrementalpid_t inc_pid_speed;		//电机速度增量pid
	positionpid_t pid_IMUSpeed;		//电机IMU速度pid
	positionpid_t pid_IMUAngle;		//电机IMU角度pid
	positionpid_t pid_angle;		//电机角度pid
	float   speedCurrent;       //内环输出量
	int32_t realAngle;			//读回来的机械角度
	int32_t realSpeed;			//读回来的速度
	int32_t realTorque;			//读回来的实际转矩
	int32_t lastAngle;			//上次的角度
	int32_t lasttargetAngle;			//上次的目标角度
	int32_t lasttotalAngle;			//上次的累积总共角度
	int32_t totalAngle;			//累积总共角度
	int32_t turnCount;			//转过的圈数
	int32_t temperture;			//读回来的电机温度
	int32_t targetCurrent;			//目标电流
	int32_t targetSpeed;			//目标速度
	int32_t targetAngle;			//目标角度
	int32_t outCurrent;				//输出电流
	int32_t lastoutCurrent;				//上一次输出电流
	int32_t infoUpdateFlag;		//信息读取更新标志
	int32_t infoUpdateFrame;	//帧率
	int32_t offLineFlag;		//设备离线标志
}M6020_t;

/******************************************************************************/
extern M6020_t M6020s;
/******************************************************************************/

void M6020_getInfo(uint8_t* CanRxBuff, uint32_t MotorID);
void M6020_setCurrent(int16_t iq5, int16_t iq6, int16_t iq7, int16_t iq8);

/******************************************************************************/

#endif /* __M6020_MOTOR_H */
