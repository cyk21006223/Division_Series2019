#ifndef __N3_DJI_ASSISITANT_H
#define __N3_DJI_ASSISITANT_H

/******************************************************************************/
#include "Users.h"

/******************************************************************************/
#define N3_DJI_GPIO_CLK    RCC_AHB1Periph_GPIOD
#define N3_DJI_PIN         GPIO_Pin_13
#define N3_DJI_GPIO_PORT   GPIOD
#define N3_DJI_PinSource   GPIO_PinSource13

/******************************************************************************/

typedef struct{
	float    Value;				//测量值
	uint32_t captureValue;		//捕获的脉冲宽度，单位us
	uint16_t captureRising;		//捕获上升沿时CCR的值
	uint16_t captureFalling;	//捕获下降沿时CCR的值
	bool     captureFlag;		//上升沿/下降沿标志位
}N3_DJI_ASSISITANT_t;

/******************************************************************************/
extern N3_DJI_ASSISITANT_t N3_Semaphore;

/******************************************************************************/
void N3_DJI_ASSISITANT_Init(void);
float N3_Semaphore_getValue(void);
bool N3_DJI_InitTest(void);
float FlagMode(float Value);
/******************************************************************************/
#endif /* __N3_DJI_ASSISITANT_H */


