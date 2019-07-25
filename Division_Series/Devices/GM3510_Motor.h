#ifndef __GM3510_MOTOR_H
#define __GM3510_MOTOR_H

/******************************************************************************/
#include  "Users.h"
/******************************************************************************/
#define GM3510_ID	  0x206//0x205~0x207
#define GM3510_SENDID		0x1FF
/******************************************************************************/

typedef struct
{
	int32_t infoUpdateFlag;		//信息读取更新标志
	int32_t infoUpdateFrame;	//帧率
	int32_t offLineFlag;		//设备离线标志
	float realAngle;			//读回来的机械角度
	float realSpeed;			//读回来的速度
	float realTorque;			//读回来的实际转矩
	float lastAngle;			//上次的角度
	float lasttargetAngle;			//上次的目标角度
	float lasttotalAngle;			//上次的累积总共角度
	float totalAngle;			//累积总共角度
	float turnCount;			//转过的圈数
	float targetSpeed;			//目标速度
	float targetAngle;			//目标角度
	float outCurrent;				//输出电流
	float lastoutCurrent;				//上一次输出电流
	float speedCurrent;       //内环输出量
	positionpid_t pid_IMUSpeed;		//电机IMU速度pid
	positionpid_t pid_IMUAngle;		//电机IMU角度pid
	positionpid_t pid_angle;		//电机角度pid
}GM3510_t;

/******************************************************************************/
extern GM3510_t  GM3510s;
/******************************************************************************/

void GM3510_getInfo(uint8_t* CanRxBuff, uint32_t MotorID);
void GM3510_setCurrent(int16_t iq5, int16_t iq6, int16_t iq7);

/******************************************************************************/

#endif /* __GM3510_MOTOR_H */

