#ifndef __USERS_H
#define __USERS_H

/******************************************************************************/
//��������������
#pragma anon_unions
/******************************************************************************/
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <arm_math.h>
#include "UserMath.h"
/******************************************************************************/
#include "binary.h"
#include "BSP_CAN.h"
#include "BSP_DMA.h"
#include "BSP_GPIO.h"
#include "BSP_NVIC.h"
#include "BSP_TIM.h"
#include "BSP_USART.h"
/******************************************************************************/
#include "ANO.h"
#include "PID.h"
#include "Filter.h"
#include "Rocker.h"
#include "LED.h"
#include "Laser.h"
#include "delay.h"
#include "watchdog.h"
#include "ComAssistant.h"
#include "RM_Motor.h"
#include "IMU.h"
#include "GY_IMU.h"
#include "NDOF_IMU.h"
#include "JY61.h"
#include "JY901.h"
#include "PowerCtrl.h"
#include "JudgeSystem.h"
#include "DR16_Receiver.h"
//#include "GM3510_Motor.h"
//#include "Ultrasonic.h"
//#include "N3_DJI_ASSISITANT.h"
#include "M6020_Motor.h"
#include "M2006_Motor.h"
#include "Fric_Motor.h"
//#include "Init_Config.h"`
#include "Cloud_control.h"
#include "Shoot.h"
#include "DevicesMonitor.h"
#include "UserTask.h"
/****************************** GAME ROBOT ************************************/
#define GAME_ROBOT       0            // 1 ����           0  �췽

/*******************************STM BOARD**************************************/
#define USE_BOARDTYPE    0						// 1 �ɿ�����				0  A�Ͱ�

/*********************************MODE*****************************************/

#define USE_SYSTEMRESET  0            // 1 �Զ�����       0  �ֶ�����
#define USE_DEBUGASSERT  0            // 1 ���ö���       0  �����ö���

/*******************************CONTROL****************************************/
#define CLOUD_YAW_LIMIT_SWITCH	  1		// 1 ��̨yaw��λ  						0 ����λ
#define CLOUD_PITCH_LIMIT_SWITCH 	1		// 1 ��̨pitch��λ 					  0 ����λ
#define Kreload_VEER              0   // 1 ������ʱ��               0 ˳ʱ��
#define USE_BLUETOOTH             0   // 1 ʹ������                 0 ��ʹ������
#define USE_IMUBYUSART            1   // 1 ʹ��GY955   0 ʹ��JY61   2 ʹ��JY901
#define USE_ULTRASONIC            1   // 1 ʹ��IOE-SR05             0 ��HC-SR04
/********************************BSP*******************************************/

//#define FRICT_BSP_CONFIG 			8		// 0 �ر�				TIMx    Ħ����

//#define IMUGY955_BSP_CONFIG		8		// 0 �ر�				USARTx 			gy955������
//#define DR16_BSP_CONFIG 			1		// 0 �ر�				USARTx 			DJң����
//#define JUDGE_BSP_CONFIG  		3		// 0 �ر�				USARTx 			����ϵͳ
//#define VISION_BSP_CONFIG  		0		// 0 �ر�				USARTx 			�Ӿ��ӿ�

/******************************************************************************/
#endif /* USERS.H */

