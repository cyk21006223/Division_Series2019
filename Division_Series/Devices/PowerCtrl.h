#ifndef __POWERCTRL_H
#define __POWERCTRL_H
/******************************************************************************/
#include "Users.h"
//#include "stm32f4xx.h"
//#include "BSP_GPIO.h"
/******************************************************************************/
typedef enum{
	Close = 0,
	Open = 1,
}PowerCtrl_e;

typedef enum{
	PowerCtrl_1,
	PowerCtrl_2,
	PowerCtrl_3,
	PowerCtrl_4,
}PowerCtrlMark_e;


#define PowerCtrl_NUM            4

#define POWERCTRL4_1_GPIO_CLK    RCC_AHB1Periph_GPIOH
#define POWERCTRL4_1_GPIO_PORT   GPIOH
#define POWERCTRL4_1_PIN         GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5
#define POWERCTRL1_PIN           GPIO_Pin_2
#define POWERCTRL2_PIN           GPIO_Pin_3
#define POWERCTRL3_PIN           GPIO_Pin_4
#define POWERCTRL4_PIN           GPIO_Pin_5

/******************************************************************************/
void PowerCtrl_Init(void);
bool PowerCtrlTest(void);
void PowerCtrl_AllDISABLE(void);
void PowerCtrl_AllENABLE(void);
void PowerCtrlSet(PowerCtrlMark_e PowerCtrl_N, PowerCtrl_e value);
/******************************************************************************/

#endif /* __POWERCTRL_H */

