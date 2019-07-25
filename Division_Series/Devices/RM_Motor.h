#ifndef __RM_Motor_H
#define __RM_Motor_H

#include "Users.h"

void RM_MotorInit(void);
bool RM_MotorTest(void);
void RM_Motor_setCurrent(CAN_TypeDef* CANx, \
	                       int16_t currentFirst, \
	                       int16_t currentSecond, \
                         int16_t currentThird, \
                         int16_t currentFourth);
void Aerial_setCurrent(int16_t YawCurrent, \
	                     int16_t PitchCurrent, \
                       int16_t KreloadCurrent);

#endif /*__RM_Motor_H*/

