#ifndef __JY901_H
#define __JY901_H

#include "Users.h"

#define  JY901_PACKAGE_LENGTH       22

/******************************************************************************/
typedef struct{
  struct {
		float Wx;
		float Wy;
		float Wz;
  }gyro;
  struct {
	  float roll;
	  float pitch;
	  float yaw;
  }eular;
	
	float lastYaw;
	float totalYaw;
	float yawTurnCount;
	
	float lastPitch;
	float totalPitch;
	float pitchTurnCount;
	
  float     Temperature;//ÎÂ¶È
  uint16_t  infoUpdateFrame;
  uint8_t   infoUpdateFlag;
  bool      offLineFlag;
}JY901Data_t;
/******************************************************************************/
extern uint8_t     JY901_reBuff[JY901_PACKAGE_LENGTH];
extern JY901Data_t JY901Data;
extern uint16_t    JY901_Counter;
/******************************************************************************/
void JY901IMU_Init(void);
uint8_t JY901_Checksum(uint8_t* buff);
void JY901_DataProcess(uint8_t* Re_buf,uint16_t dataLength,JY901Data_t* dataOut);
/******************************************************************************/
#endif  /*__JY61_H*/

