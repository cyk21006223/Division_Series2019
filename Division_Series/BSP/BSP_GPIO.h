#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include "stm32f4xx.h"


void GPIO_QuickInit(uint32_t RCC_AHB1Periph, GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, GPIOOType_TypeDef GPIO_OType, GPIOPuPd_TypeDef GPIO_PuPd);


#endif /*__GPIO_H*/

