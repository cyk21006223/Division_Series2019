#ifndef __JY61_H
#define __JY61_H

#include "Users.h"

#define  JY61_PACKAGE_LENGTH       33

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
  float     Temperature;//ÎÂ¶È
  uint16_t  infoUpdateFrame;
  uint8_t   infoUpdateFlag;
  bool      offLineFlag;
}JY61Data_t;
/******************************************************************************/
extern uint8_t     JY61_reBuff[JY61_PACKAGE_LENGTH];
extern JY61Data_t  JY61Data;
extern uint16_t    JY61_Counter;
/******************************************************************************/
void JY61IMU_Init(void);
uint8_t JY61_Checksum(uint8_t* buff);
void JY61_DataProcess(uint8_t* Re_buf,uint16_t dataLength,JY61Data_t* dataOut);
/******************************************************************************/
#endif  /*__JY61_H*/

