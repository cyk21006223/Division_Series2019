#include "JY901.h"

uint8_t      JY901_reBuff[JY901_PACKAGE_LENGTH];
JY901Data_t  JY901Data;
uint16_t     JY901_Counter;

/**
  * @brief  JY901-IMU初始化
  * @param  void
  * @retval None
  */
void JY901IMU_Init(void)
{
#if USE_BOARDTYPE == 1
	USART3_QuickInit(115200);
	USART3_RXDMA_Config((uint32_t)JY901_reBuff, JY901_PACKAGE_LENGTH+2);//云台
#endif

#if USE_BOARDTYPE == 0
	UART8_QuickInit(115200);
	UART8_RXDMA_Config((uint32_t)JY901_reBuff, JY901_PACKAGE_LENGTH+2);//云台
#endif
}

/**
  * @brief  JY901数据报文处理
  * @param  报文长度
  * @retval None
  */
void JY901_DataProcess(uint8_t* Re_buf, uint16_t dataLength, JY901Data_t* dataOut)
{
	for(uint16_t n = 0; n < dataLength; )
	{
		//匹配帧头
		if(Re_buf[n] == 0x55&&Re_buf[n+11] == 0x55)
		{
				int16_t DATA[8];
				DATA[0]=Re_buf [n+3]<<8| Re_buf [n+2];
			  DATA[1]=Re_buf [n+5]<<8| Re_buf [n+4];
				DATA[2]=Re_buf [n+7]<<8| Re_buf [n+6];
				DATA[3]=Re_buf [n+9]<<8| Re_buf [n+8];
				
				DATA[4]=Re_buf [n+11+3]<<8| Re_buf [n+11+2];
			  DATA[5]=Re_buf [n+11+5]<<8| Re_buf [n+11+4];
				DATA[6]=Re_buf [n+11+7]<<8| Re_buf [n+11+6];
				DATA[7]=Re_buf [n+11+9]<<8| Re_buf [n+11+8];

			 dataOut->gyro.Wx = (float)DATA[0]/(float)32768.0*(float)2000.0;
			 dataOut->gyro.Wy = (float)DATA[1]/(float)32768.0*(float)2000.0;
			 dataOut->gyro.Wz = (float)DATA[2]/(float)32768.0*(float)2000.0;
			 dataOut->Temperature = (float)DATA[3]/(float)271.868527;

			 dataOut->eular.roll = (float)DATA[4]/(float)32768.0*(float)180.0;
			 dataOut->eular.pitch = (float)DATA[5]/(float)32768.0*(float)180.0;
			 dataOut->eular.yaw = (float)DATA[6]/(float)32768.0*(float)180.0;
			 dataOut->Temperature = (float)DATA[7]/(float)271.868527;

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
