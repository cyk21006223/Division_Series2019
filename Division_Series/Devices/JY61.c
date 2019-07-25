#include "JY61.h"

uint8_t     JY61_reBuff[JY61_PACKAGE_LENGTH];
JY61Data_t  JY61Data;
uint16_t    JY61_Counter;

/**
  * @brief  JY61-IMU初始化
  * @param  void
  * @retval None
  */
void JY61IMU_Init(void)
{
#if USE_BOARDTYPE == 1
	USART3_QuickInit(115200);
	USART3_RXDMA_Config((uint32_t)JY61_reBuff, JY61_PACKAGE_LENGTH+2);//云台
#endif

#if USE_BOARDTYPE == 0
	UART8_QuickInit(115200);
	UART8_RXDMA_Config((uint32_t)JY61_reBuff, JY61_PACKAGE_LENGTH+2);//云台
#endif
}

/**
  * @brief  JY61数据报文处理
  * @param  报文长度
  * @retval None
  */
void JY61_DataProcess(uint8_t* Re_buf, uint16_t dataLength, JY61Data_t* dataOut)
{
	for(uint16_t n = 0; n < dataLength; )
	{
		//匹配帧头
		if(Re_buf[n+11] == 0x55&&Re_buf[n+11+11] == 0x55)
		{
				int16_t DATA[8];
				DATA[0]=Re_buf [n+11+3]<<8| Re_buf [n+11+2];
			  DATA[1]=Re_buf [n+11+5]<<8| Re_buf [n+11+4];
				DATA[2]=Re_buf [n+11+7]<<8| Re_buf [n+11+6];
				DATA[3]=Re_buf [n+11+9]<<8| Re_buf [n+11+8];
				
				DATA[4]=Re_buf [n+22+3]<<8| Re_buf [n+22+2];
			  DATA[5]=Re_buf [n+22+5]<<8| Re_buf [n+22+4];
				DATA[6]=Re_buf [n+22+7]<<8| Re_buf [n+22+6];
				DATA[7]=Re_buf [n+22+9]<<8| Re_buf [n+22+8];
				

			 dataOut->gyro.Wx = (float)DATA[0]/(float)32768.0*(float)2000;
			 dataOut->gyro.Wy = (float)DATA[1]/(float)32768.0*(float)2000;
			 dataOut->gyro.Wz = (float)DATA[2]/(float)32768.0*(float)2000;
			 dataOut->Temperature = (float)DATA[3]/(float)340.0+(float)36.25;

			 dataOut->eular.roll = (float)DATA[0+4]/(float)32768.0*(float)180;
			 dataOut->eular.pitch = (float)DATA[1+4]/(float)32768.0*(float)180;
			 dataOut->eular.yaw = (float)DATA[2+4]/(float)32768.0*(float)180;
			 dataOut->Temperature = (float)DATA[3+4]/(float)340.0+(float)36.25;

				/*帧率检测*/
				dataOut->infoUpdateFrame++;
				dataOut->infoUpdateFlag = 1;
				
				return;
		}
		else{
			n++;
		}
	}
	
}
