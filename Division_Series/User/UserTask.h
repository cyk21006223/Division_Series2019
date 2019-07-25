#ifndef __USERTASK_H
#define __USERTASK_H

#include "Users.h"

typedef enum{
	ShootTask = 1,
	CloudTask = 2,
}TimerID_e;

typedef enum{
	ControlMode_ByPC_andRocker = 3,
	ControlMode_ByPC_notRocker = 1,
	ControlMode_AllScram = 2,
}ControlMode_e;

typedef struct
{
    uint8_t ControlMode;			//���Ʒ�ʽ
    uint8_t isCombineKey;			//ʹ����ϼ�
} ControlStatus_t;

extern ControlStatus_t ControlStatus;
/*******************************����ʼ***************************************/

//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
extern TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define XCOM_PRIO		4
//�����ջ��С	
#define XCOM_SIZE 		128
//������
extern TaskHandle_t XCOM_Task_Handler;
//������
void XCOM_SEND(void *pvParameters);

//�������ȼ�
#define Shoot_deciding_Task_PRIO		4
//�����ջ��С	
#define Shoot_deciding_Task_SIZE 	128 
//������
extern TaskHandle_t Shoot_deciding_Handler;
//������
void Shoot_deciding_Task(void *pvParameters);

//�������ȼ�
#define GetHeight_Task_PRIO		4
//�����ջ��С	
#define GetHeight_Task_SIZE 		128  
//������
extern TaskHandle_t GetHeight_Task_Handler;
//������
void GetHeight_Task(void *pvParameters);

//�������ȼ�
#define Cloud_deciding_PRIO		4
//�����ջ��С	
#define Cloud_deciding_SIZE 	128  
//������
extern TaskHandle_t Cloud_deciding_Handler;
//������
void Cloud_deciding_Task(void *pvParameters);

//�������ȼ�
#define Cloud_executing_PRIO		4
//�����ջ��С	
#define Cloud_executing_SIZE 	128  
//������
extern TaskHandle_t Cloud_executing_Handler;
//������
void Cloud_executing_Task(void *pvParameters);

//�������ȼ�
#define Control_BySwitch_PRIO		5
//�����ջ��С	
#define Control_BySwitch_SIZE 	128  
//������
extern TaskHandle_t Control_BySwitch_Handler;
//������
void Control_BySwitch_Task(void *pvParameters);

//�������ȼ�
#define Control_ByMouse_PRIO		5
//�����ջ��С	
#define Control_ByMouse_SIZE 		128  
//������
extern TaskHandle_t Control_ByMouse_Handler;
//������
void Control_ByMouse_Task(void *pvParameters);

//�������ȼ�
#define DevicesMonitor_PRIO		4
//�����ջ��С	
#define DevicesMonitor_SIZE 	128  
//������
extern TaskHandle_t DevicesMonitor_Handler;
//������
void DevicesMonitor_Task(void *pvParameters);

//�������ȼ�
#define JudgeSystem_Send_PRIO	 4
//�����ջ��С	
#define JudgeSystem_Send_SIZE 	 128  
//������
extern TaskHandle_t JudgeSystem_Send_Handler;
//������
void JudgeSystem_Send(void *pvParameters);

/*******************************�������***************************************/

#endif





