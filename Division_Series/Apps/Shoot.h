#ifndef __SHOOT_H
#define __SHOOT_H
/******************************************************************************/
#include "Users.h"
/******************************************************************************/
#define KreloadMotor    M2006s[1]  //�������
/******************************************************************************/

typedef struct{
	bool ShootInit;								//�����ʼ����־
	bool shootFlag;			          //��������־
	bool SingleShootFlag;					//���������־
	bool TwoShootFlag;            //��������־
	bool RepeatedShootFlag;				//���������־
	bool shootReady;							//���׼����־
	bool SpeedUnqualified;        //�������ͱ�־
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



