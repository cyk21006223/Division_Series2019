#ifndef __DEVICESMONITOR_H
#define __DEVICESMONITOR_H
/******************************************************************************/
#include "Users.h"
/******************************************************************************/

#define GY_IMU_FRAMEMIN		    3			//��������С֡��
#define Ultrasonic_FRAMEMIN   50	    //��������С֡��
#define JudgeSystem_FRAMEMIN  2	    //��������С֡��

/******************************************************************************/

void DevicesMonitor_update(void);
 
/******************************************************************************/

#endif /* __DEVICESMONITOR_H */



