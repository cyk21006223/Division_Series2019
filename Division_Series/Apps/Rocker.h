#ifndef __ROCKER_H
#define __ROCKER_H

#include "Users.h"
///*Lib*/
//#include "stm32f4xx.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <arm_math.h>
///*Devices*/
//#include "DR16_Receiver.h"

typedef struct
{
    float x;
    float y;
    float radian;
    float degrees;
    float distance;
} rocker_t;

extern rocker_t dr16_rocker_L,dr16_rocker_R;

/**********将xy坐标变成摇杆数据***************/
void Rocker_getData(float posX, float posY, rocker_t *roc);


#endif
