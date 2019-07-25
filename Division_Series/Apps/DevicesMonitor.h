#ifndef __DEVICESMONITOR_H
#define __DEVICESMONITOR_H
/******************************************************************************/
#include "Users.h"
/******************************************************************************/

#define GY_IMU_FRAMEMIN		    3			//陀螺仪最小帧率
#define Ultrasonic_FRAMEMIN   50	    //超声波最小帧率
#define JudgeSystem_FRAMEMIN  2	    //超声波最小帧率

/******************************************************************************/

void DevicesMonitor_update(void);
 
/******************************************************************************/

#endif /* __DEVICESMONITOR_H */



