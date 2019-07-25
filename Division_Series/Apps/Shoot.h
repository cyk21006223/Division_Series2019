#ifndef __SHOOT_H
#define __SHOOT_H
/******************************************************************************/
#include "Users.h"
/******************************************************************************/
#define KreloadMotor    M2006s[1]  //拨弹电机
/******************************************************************************/

typedef struct{
	bool ShootInit;								//射击初始化标志
	bool shootFlag;			          //射击请求标志
	bool SingleShootFlag;					//单发请求标志
	bool TwoShootFlag;            //二连发标志
	bool RepeatedShootFlag;				//连发请求标志
	bool shootReady;							//射击准备标志
	bool SpeedUnqualified;        //射速拉低标志
}Shooter_t;

/******************************************************************************/
extern Shooter_t Shooter;
extern const uint16_t FricSpeed[4];
/******************************************************************************/
void Shoot_Init(void);
void Shoot_deciding(void);
void Kreload_Init(void);
void Shoot_single_shot(void);
void Shoot_repeated_fire(int in);
void Kreload_Jam_Handle(void);
void Kreload_turnoff(void);
double Shoot_Freq_Mode(double Shoot_NumPerSecond);
bool getKreloadVeer(void);
void RecoverTimeMonitor(void);
/******************************************************************************/
#endif /* __SHOOT_H */



