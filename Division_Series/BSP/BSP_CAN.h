#ifndef __BSP_CAN_H
#define __BSP_CAN_H

#include "stm32f4xx.h"
#include "BSP_NVIC.h"


#define CAN1_RX_GPIO_CLK          	RCC_AHB1Periph_GPIOD
#define CAN1_RX_GPIO_PORT           GPIOD
#define CAN1_RX_Pin           		GPIO_Pin_0
#define CAN1_RX_PINSOURCE			GPIO_PinSource0

#define CAN1_TX_GPIO_CLK          	RCC_AHB1Periph_GPIOD
#define CAN1_TX_GPIO_PORT           GPIOD
#define CAN1_TX_Pin          	 	GPIO_Pin_1
#define CAN1_TX_PINSOURCE			GPIO_PinSource1


#define CAN1_SJW		CAN_SJW_1tq
#define CAN1_BS1		CAN_BS1_3tq
#define CAN1_BS2		CAN_BS2_5tq
#define CAN1_Prescaler	5


#define CAN2_RX_GPIO_CLK          	RCC_AHB1Periph_GPIOB
#define CAN2_RX_GPIO_PORT           GPIOB
#define CAN2_RX_Pin           		GPIO_Pin_12
#define CAN2_RX_PINSOURCE			GPIO_PinSource12

#define CAN2_TX_GPIO_CLK          	RCC_AHB1Periph_GPIOB
#define CAN2_TX_GPIO_PORT           GPIOB
#define CAN2_TX_Pin          	 	GPIO_Pin_13
#define CAN2_TX_PINSOURCE			GPIO_PinSource13


#define CAN2_SJW		CAN_SJW_1tq
#define CAN2_BS1		CAN_BS1_3tq
#define CAN2_BS2		CAN_BS2_5tq
#define CAN2_Prescaler	5


void CAN1_QuickInit(void);
void CAN2_QuickInit(void);
void CAN_SendData(CAN_TypeDef* CANx, uint8_t id_type, uint32_t id, uint8_t data[8]);
uint8_t CAN_sendStatus(CAN_TypeDef* CANx);

#endif /* __CAN_H */


