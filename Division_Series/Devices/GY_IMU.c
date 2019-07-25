#include "GY_IMU.h"
/******************************************************************************/

GY_IMU_t   Cloud_IMU;
uint8_t    Cloud_GY_IMU_RXBUFF[GY_IMU_BUFFSIZE];
uint16_t   GY_Counter;
/******************************************************************************/

/**
  * @brief  GY_IMU串口初始化
  * @param  None
  * @retval None
  */
void GY_IMU_Init(void)
{
#if USE_BOARDTYPE == 1
	USART3_QuickInit(115200);
	USART3_RXDMA_Config((uint32_t)Cloud_GY_IMU_RXBUFF, GY_IMU_BUFFSIZE);//云台
#endif

#if USE_BOARDTYPE == 0
	UART8_QuickInit(115200);
	UART8_RXDMA_Config((uint32_t)Cloud_GY_IMU_RXBUFF, GY_IMU_BUFFSIZE);//云台
#endif
}

/**
  * @brief   GY_IMU数据包校验
  * @param   [in]  buff	数据包
  * @retval  0 校验失败，1校验通过
  */
uint8_t GY_IMU_Checksum(uint8_t* buff)
{
	uint8_t sum = 0;
	
	for(uint8_t i=0; i<GY_IMU_PACKAGE_LENGTH-1; i++){
		sum += buff[i]; 
	}
	
	if(sum == buff[GY_IMU_PACKAGE_LENGTH-1]){
		//校验通过
		return 1;
	}
	else{
		return 0;
	}
}

/**
  * @brief   GY_IMU数据接收
  * @param   [in]  buff	数据包
	*					 [in]  dataLength
	*					 [in]  IMU's master
  * @retval  NULL
  */
void GY_IMU_getInfo(uint8_t* buff, uint16_t dataLength, GY_IMU_t* dataOut)
{
	for(uint16_t n = 0; n < dataLength; )
	{
		//匹配帧头
		if( (buff[n] == 0x5A) && (buff[n+1] == 0x5A) )
		{
			if(GY_IMU_Checksum(&buff[n]))
			{
				int16_t DATA[6];
				DATA[0]=(buff[n+4]<<8) | buff[n+5];
				DATA[1]=(buff[n+6]<<8) | buff[n+7];
				DATA[2]=(buff[n+8]<<8) | buff[n+9];
				DATA[3]=(buff[n+10]<<8) | buff[n+11];
				DATA[4]=(buff[n+12]<<8) | buff[n+13];
				DATA[5]=(buff[n+14]<<8) | buff[n+15];
				
				dataOut->gyro.x = (float)DATA[0]/16;
				dataOut->gyro.y = (float)DATA[1]/16;
				dataOut->gyro.z = (float)DATA[2]/16;
				
				dataOut->eular.yaw = (float)((uint16_t)DATA[3])/100;
				dataOut->eular.roll = (float)DATA[4]/100;
				dataOut->eular.pitch = (float)DATA[5]/100;
				
				dataOut->devStatus = buff[n+16];
				
				/*Yaw轴过零处理*/
				/*1、算圈数*/
				/*  '1度'->'359度'  */
				if(dataOut->eular.yaw - dataOut->lastYaw < -100){
					dataOut->yawTurnCount++;
				}
				/*  '359度'->'1度'  */
				if(dataOut->lastYaw - dataOut->eular.yaw < -100){
					dataOut->yawTurnCount--;
				}
				/*2、算totalYaw*/
				dataOut->totalYaw = dataOut->eular.yaw + \
									(360.0f * dataOut->yawTurnCount);
				/*3、保存上一次的totalYaw*/
				dataOut->lastYaw =  dataOut->eular.yaw;	
				
								
				/*Pitch轴过零处理*/
        dataOut->totalPitch = dataOut->eular.roll + \
									(360.0f * dataOut->pitchTurnCount);
				
				dataOut->lastPitch =  dataOut->eular.roll;

				/*帧率检测*/
				dataOut->infoUpdateFrame++;
				dataOut->infoUpdateFlag = 1;
				
				return;
			}
			else{
				n++;
			}
			
		}
		else{
			n++;
		}
	}
	
}


