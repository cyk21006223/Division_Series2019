#ifndef __USERCONTROL_H
#define	__USERCONTROL_H

/******************************************************************************/

#include "Users.h"

/******************************************************************************/
typedef enum{
	ControlMode_ByRC = 3,
	ControlMode_ByPC = 1,
	ControlMode_OFF = 2,
}ControlMode_e;

typedef struct
{
    uint8_t ControlMode;			//控制方式
    uint8_t isCombineKey;			//使用组合键
} ControlStatus_t;
/******************************************************************************/
extern ControlStatus_t ControlStatus;
/******************************************************************************/
uint8_t Control_getMode(void);
void Control_ByRC(void);
void Control_ByPC(void);
/******************************************************************************/
#endif


