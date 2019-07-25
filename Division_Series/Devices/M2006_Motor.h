#ifndef __M2006_MOTOR_H
#define __M2006_MOTOR_H

/******************************************************************************/
#include "Users.h"
/******************************************************************************/
#define M2006_FIRST_ID	  			0x206		/*0x205~0x208*/
#define M2006_SENDID		        0x1FF

#define PITCHMOTOR_RECEIVEID    0x206
#define KRELOADMOTOR_RECEIVEID  0x207

#define M2006_LOADANGLE		      49152		//拨一颗弹转的角度数360/6*8192*0.1(6孔)
#define M2006LockedRotorCurrent 4500-1  //堵转电流,减1则不堵转
/******************************************************************************/
typedef enum{
	PITCHMOTOR =0,
	KRELOADMOTOR =1,
}M2006Name_e;


typedef struct{
	incrementalpid_t inc_pid_speed;		//电机速度增量pid
	positionpid_t pid_speed;		      //电机速度pid
	positionpid_t pid_angle;		      //电机角度pid
	positionpid_t pid_IMUSpeed;		    //电机IMU速度pid
	positionpid_t pid_IMUAngle;		    //电机IMU角度pid
	positionpid_t pid_current;        //电机电流环
	FuzzyPID_t    fuzzypid_angle;		  //电机角度模糊pid
	float         targetSpeed;        //内环输出量
	int32_t       blockCounter;				//卡弹计数
	int32_t       realAngle;					//读回来的机械角度
	int32_t       realSpeed;					//读回来的速度
	int32_t       realTorque;					//读回来的实际转矩
	int32_t       lastAngle;					//上次的角度
	int32_t       lasttargetAngle;		//上次的目标角度
	int32_t       lasttotalAngle;			//上次的累积总共角度
	int32_t       totalAngle;					//累积总共角度
	int32_t       turnCount;					//转过的圈数
	int32_t       targetAngle;				//目标角度
	int32_t       targetCurrent;      //目标电流
	int32_t       outCurrent;					//输出电流
	int32_t       lastoutCurrent;			//上一次输出电流
	int32_t       infoUpdateFlag;			//信息读取更新标志
	int32_t       infoUpdateFrame;		//帧率
	int32_t       offLineFlag;				//设备离线标志
	uint8_t       RecoverCount;       //恢复计数
	uint8_t       blocking;				    //卡弹状态标志
	bool          KreloadError;       //拨盘故障
	bool          KreloadVeer;        //拨盘转向标志
  bool          BulletLaunched;     //已发射弹量
	bool          loadFinish;					//子弹加载完毕
}M2006s_t;


/******************************************************************************/

extern M2006s_t M2006s[2];

/******************************************************************************/

void M2006_setCurrent(int16_t iq7);
void M2006_getInfo(uint8_t* CanRxBuff, uint32_t MotorID);
void M2006_setTargetSpeed(M2006Name_e motorName, int16_t speed);
void M2006_setTargetAngle(M2006Name_e motorName, int32_t angle);

/******************************************************************************/

#endif /* __M2006_MOTOR_H */
