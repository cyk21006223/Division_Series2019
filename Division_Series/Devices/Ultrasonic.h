#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H
/******************************************************************************/
#include "Users.h"
/******************************************************************************/

typedef struct{
#if USE_ULTRASONIC == 0
	float    length;		       //����
#endif
#if USE_ULTRASONIC == 1
	uint16_t  length;		       //����
#endif
	uint16_t infoUpdateFrame;	 //֡��
	uint8_t  offLineFlag;		   //�豸���߱�־
	uint8_t  infoUpdateFlag;    //���ݽ��ձ�־
}Ultrasonic_t;

/******************************************************************************/
	/*PA5\PA4,P1\P2*/
#define SR04_TRIG_RCC		 RCC_AHB1Periph_GPIOA
#define SR04_ECHO_RCC		 RCC_AHB1Periph_GPIOA

#define SR04_TRIG_PIN		 GPIO_Pin_4
#define SR04_ECHO_PIN		 GPIO_Pin_5

#define SR04_TRIG_PORT	 GPIOA
#define SR04_ECHO_PORT	 GPIOA

#define IOE_PACKAGE_LENGTH 4

/******************************************************************************/
extern Ultrasonic_t Ultrasonic;
extern uint8_t IOE_RXBUFF[4];
/******************************************************************************/
void Ultrasonic_getDistance(void);
void Ultrasonic_Init(void);
bool UltrasonicTest(void);
uint8_t IOE_Checksum(uint8_t* buff);
void IOE_getInfo(uint8_t* buff, uint16_t dataLength, Ultrasonic_t* dataOut);
/******************************************************************************/

#endif

