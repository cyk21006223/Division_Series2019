#ifndef __BSP_NVIC_H
#define __BSP_NVIC_H

#include "stm32f4xx.h"

void NVIC_QuickInit(uint16_t IRQChannel, \
	                  uint16_t MainPriority, \
                    uint16_t SubPriority);

void NVIC_RTOSInit(void);

#endif /*__NVIC_H*/

