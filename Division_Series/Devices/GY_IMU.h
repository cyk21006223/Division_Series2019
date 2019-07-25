#ifndef __GY_IMU_H
#define __GY_IMU_H
/******************************************************************************/
#include "Users.h"
/******************************************************************************/
#define GY_IMU_PACKAGE_LENGTH 	18
#define GY_IMU_BUFFSIZE 				30
/******************************************************************************/

typedef struct {
		float w;
		float x;
		float y;
		float z;
}quaternion_t;



typedef struct {
		float x;
		float y;
		float z;
}vector_t;



typedef struct {
		int16_t x;
		int16_t y;
		int16_t z;
}vector16_t;



typedef struct {
	float roll;
	float pitch;
	float yaw;
}eular_t;



typedef struct{
	vector_t gyro;
	eular_t eular;

	float lastYaw;
	float totalYaw;
	float yawTurnCount;
	
	float lastPitch;
	float totalPitch;
	float pitchTurnCount;
	
	uint16_t infoUpdateFrame;
	
	uint8_t devStatus;
	uint8_t infoUpdateFlag;
	uint8_t offLineFlag;
	
}GY_IMU_t;


/******************************************************************************/

extern  GY_IMU_t  Cloud_IMU;
extern  uint8_t   Cloud_GY_IMU_RXBUFF[GY_IMU_BUFFSIZE];
extern  uint16_t  GY_Counter;
/******************************************************************************/

void 		GY_IMU_Init(void);
uint8_t GY_IMU_Checksum(uint8_t* buff);
void    GY_IMU_getInfo(uint8_t* buff, uint16_t dataLength, GY_IMU_t* dataOut);

/******************************************************************************/
#endif /* __GY_IMU_H */


