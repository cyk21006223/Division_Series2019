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
    uint8_t ControlMode;			//控制方式
    uint8_t isCombineKey;			//使用组合键
} ControlStatus_t;

extern ControlStatus_t ControlStatus;
/*******************************任务开始***************************************/

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
extern TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define XCOM_PRIO		4
//任务堆栈大小	
#define XCOM_SIZE 		128
//任务句柄
extern TaskHandle_t XCOM_Task_Handler;
//任务函数
void XCOM_SEND(void *pvParameters);

//任务优先级
#define Shoot_deciding_Task_PRIO		4
//任务堆栈大小	
#define Shoot_deciding_Task_SIZE 	128 
//任务句柄
extern TaskHandle_t Shoot_deciding_Handler;
//任务函数
void Shoot_deciding_Task(void *pvParameters);

//任务优先级
#define GetHeight_Task_PRIO		4
//任务堆栈大小	
#define GetHeight_Task_SIZE 		128  
//任务句柄
extern TaskHandle_t GetHeight_Task_Handler;
//任务函数
void GetHeight_Task(void *pvParameters);

//任务优先级
#define Cloud_deciding_PRIO		4
//任务堆栈大小	
#define Cloud_deciding_SIZE 	128  
//任务句柄
extern TaskHandle_t Cloud_deciding_Handler;
//任务函数
void Cloud_deciding_Task(void *pvParameters);

//任务优先级
#define Cloud_executing_PRIO		4
//任务堆栈大小	
#define Cloud_executing_SIZE 	128  
//任务句柄
extern TaskHandle_t Cloud_executing_Handler;
//任务函数
void Cloud_executing_Task(void *pvParameters);

//任务优先级
#define Control_BySwitch_PRIO		5
//任务堆栈大小	
#define Control_BySwitch_SIZE 	128  
//任务句柄
extern TaskHandle_t Control_BySwitch_Handler;
//任务函数
void Control_BySwitch_Task(void *pvParameters);

//任务优先级
#define Control_ByMouse_PRIO		5
//任务堆栈大小	
#define Control_ByMouse_SIZE 		128  
//任务句柄
extern TaskHandle_t Control_ByMouse_Handler;
//任务函数
void Control_ByMouse_Task(void *pvParameters);

//任务优先级
#define DevicesMonitor_PRIO		4
//任务堆栈大小	
#define DevicesMonitor_SIZE 	128  
//任务句柄
extern TaskHandle_t DevicesMonitor_Handler;
//任务函数
void DevicesMonitor_Task(void *pvParameters);

//任务优先级
#define JudgeSystem_Send_PRIO	 4
//任务堆栈大小	
#define JudgeSystem_Send_SIZE 	 128  
//任务句柄
extern TaskHandle_t JudgeSystem_Send_Handler;
//任务函数
void JudgeSystem_Send(void *pvParameters);

/*******************************任务结束***************************************/

#endif





