#ifndef __USERS_H
#define __USERS_H

/******************************************************************************/
//允许匿名共用体
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
#define GAME_ROBOT       0            // 1 蓝方           0  红方

/*******************************STM BOARD**************************************/
#define USE_BOARDTYPE    0						// 1 旧开发板				0  A型板

/*********************************MODE*****************************************/

#define USE_SYSTEMRESET  0            // 1 自动重启       0  手动重启
#define USE_DEBUGASSERT  0            // 1 启用断言       0  不启用断言

/*******************************CONTROL****************************************/
#define CLOUD_YAW_LIMIT_SWITCH	  1		// 1 云台yaw限位  						0 不限位
#define CLOUD_PITCH_LIMIT_SWITCH 	1		// 1 云台pitch限位 					  0 不限位
#define Kreload_VEER              0   // 1 拨盘逆时针               0 顺时针
#define USE_BLUETOOTH             0   // 1 使用蓝牙                 0 不使用蓝牙
#define USE_IMUBYUSART            1   // 1 使用GY955   0 使用JY61   2 使用JY901
#define USE_ULTRASONIC            1   // 1 使用IOE-SR05             0 用HC-SR04
/********************************BSP*******************************************/

//#define FRICT_BSP_CONFIG 			8		// 0 关闭				TIMx    摩擦轮

//#define IMUGY955_BSP_CONFIG		8		// 0 关闭				USARTx 			gy955陀螺仪
//#define DR16_BSP_CONFIG 			1		// 0 关闭				USARTx 			DJ遥控器
//#define JUDGE_BSP_CONFIG  		3		// 0 关闭				USARTx 			裁判系统
//#define VISION_BSP_CONFIG  		0		// 0 关闭				USARTx 			视觉接口

/******************************************************************************/
#endif /* USERS.H */

