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
	incrementalpid_t inc_pid_speed;		//����ٶ�����pid
	positionpid_t pid_IMUSpeed;		//���IMU�ٶ�pid
	positionpid_t pid_IMUAngle;		//���IMU�Ƕ�pid
	positionpid_t pid_angle;		//����Ƕ�pid
	float   speedCurrent;       //�ڻ������
	int32_t realAngle;			//�������Ļ�е�Ƕ�
	int32_t realSpeed;			//���������ٶ�
	int32_t realTorque;			//��������ʵ��ת��
	int32_t lastAngle;			//�ϴεĽǶ�
	int32_t lasttargetAngle;			//�ϴε�Ŀ��Ƕ�
	int32_t lasttotalAngle;			//�ϴε��ۻ��ܹ��Ƕ�
	int32_t totalAngle;			//�ۻ��ܹ��Ƕ�
	int32_t turnCount;			//ת����Ȧ��
	int32_t temperture;			//�������ĵ���¶�
	int32_t targetCurrent;			//Ŀ�����
	int32_t targetSpeed;			//Ŀ���ٶ�
	int32_t targetAngle;			//Ŀ��Ƕ�
	int32_t outCurrent;				//�������
	int32_t lastoutCurrent;				//��һ���������
	int32_t infoUpdateFlag;		//��Ϣ��ȡ���±�־
	int32_t infoUpdateFrame;	//֡��
	int32_t offLineFlag;		//�豸���߱�־
}M6020_t;

/******************************************************************************/
extern M6020_t M6020s;
/******************************************************************************/

void M6020_getInfo(uint8_t* CanRxBuff, uint32_t MotorID);
void M6020_setCurrent(int16_t iq5, int16_t iq6, int16_t iq7, int16_t iq8);

/******************************************************************************/

#endif /* __M6020_MOTOR_H */
