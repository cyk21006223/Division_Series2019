#ifndef __CLOUD_CONTROL_H
#define __CLOUD_CONTROL_H
/******************************************************************************/
#include "Users.h"
/******************************************************************************/
#define Pluse_per_Deg_6020			((float)360/(float)8192)
#define Pluse_per_Deg_2006		  ((float)1/(float)819.2)
#define AngleToDEG_6020(ang)		((float)ang*(float)Pluse_per_Deg_6020)
#define AngleToDEG_2006(ang)		((float)ang*(float)Pluse_per_Deg_2006)

#define Cloud_Yaw_Center        -123
#define Cloud_Pitch_Center      -5426
#define Cloud_Pitch_UP          4110

#define	Cloud_Yaw_Motor         M6020s
#define	Cloud_Pitch_Motor		  	M2006s[0]
/******************************************************************************/

typedef enum{
	Yaw = 0,
	Pitch = 1,
}CloudName_e;


typedef struct{
	float    YAWCENTER;             //初始化后当前YAW中心
	float    PITCHCENTER;           //初始化后当前PITCH中心
	float    Sensity[2];				    //云台移动灵敏度
	float    IMUYawAngleCenter;     //Yaw云台IMU中心
	float    IMUYawAngleMax;		    //云台IMU最大角度(右)
	float    IMUYawAngleMin;		    //云台IMU最小角度(左)
	float    IMUPitchAngleCenter;     //Pitch云台IMU中心
	float    IMUPitchAngleTopped;     //Pitch云台IMU最高点
	float    IMUPitchAngleMax;		  //云台IMU最大角度(下)
	float    IMUPitchAngleMin;		  //云台IMU最小角度(上)
	float    IMUtargetYaw;		      //云台目标yaw轴角度
	float    IMUtargetPitch;	      //云台目标pitch轴角度
	int16_t	 Control[2];						//云台控制入口
	uint8_t  ControlMode;           //云台控制模式
	bool 		 CloudInit;							//云台初始化标志
	bool     BeCloseToGND;          //靠近地面
}Cloud_t;

/******************************************************************************/

extern Cloud_t Cloud; 

/******************************************************************************/
void Cloud_withGYIMU_Init(void);
void Cloud_deciding_5MSGYIMU(void);
void Cloud_deciding_10MSGYIMU(void);
void Cloud_setCurrent(int16_t iq5, int16_t iq6, int16_t iq7);
void Cloud_setTargetAngle(CloudName_e motorName, float angle_current);
float Cloud_getYawAngleToCenter(void);
float Cloud_getPitchAngleToCenter(void);
float Cloud_getPitchAngleTopped(void);
void SetPitchLimit(float pitchMAX, float pitchMIN);
/******************************************************************************/

#endif /* __CLOUD_CONTROL_H */



