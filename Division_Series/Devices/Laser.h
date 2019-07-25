#ifndef __LASER_H
#define __LASER_H

#include "Users.h"
//#include "stm32f4xx.h"

typedef enum{
	LaserOFF = 0,
	LaserON = 1,
}Laserc_e;

#define LASER_GPIO_CLK     RCC_AHB1Periph_GPIOG
#define LASER_PIN          GPIO_Pin_13
#define LASER_GPIO_PORT    GPIOG

#define LASER(x) 					 GPIO_WriteBit(LASER_GPIO_PORT,LASER_PIN,(BitAction)x)
#define LASER_TOGGLE() 		 GPIO_ToggleBits(LASER_GPIO_PORT, LASER_PIN)


void Laser_Init(void);

#endif


