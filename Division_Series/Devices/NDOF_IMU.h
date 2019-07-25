#ifndef _NDOF_IMU_H
#define _NDOF_IMU_H
/******************************************************************************/
#include "Users.h"
/******************************************************************************/
#define USE_NDOF055_CAN       1        // 1 使用NDOF055IMU_CAN      0  不使用 
#define NDOF055_DATA_READID 	0x413
/******************************************************************************/

typedef struct{
	struct {
		float x;
		float y;
		float z;
  }gyro;
  struct {
	  float roll;
	  float pitch;
	  float yaw;
  }eular;
	uint16_t  infoUpdateFrame;
  uint8_t   dataBuff[8];
	uint8_t   infoUpdateFlag;
	bool      offLineFlag;
} NDOF055Data_t;
/******************************************************************************/
extern NDOF055Data_t NDOF055_data;
/******************************************************************************/
void NDOF_IMU_Init(void);
bool NDOF_IMUTest(void);
void NDOF055_getInfo(CanRxMsg RxMessage);
/******************************************************************************/
#endif


