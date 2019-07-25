#include "NDOF_IMU.h"

static bool isInit = false;
NDOF055Data_t NDOF055_data;

/**
  * @brief  NDOF055信息处理
  * @param  *RxMessage 	CAN报文接收结构体指针
  * @retval None
  */
static void NDOF055_Analysis(void)
{
	int16_t DATA[4];
	
	DATA[0]=NDOF055_data.dataBuff[0]|NDOF055_data.dataBuff[1]<<8;
	DATA[1]=NDOF055_data.dataBuff[2]|NDOF055_data.dataBuff[3]<<8;
	DATA[2]=NDOF055_data.dataBuff[4]|NDOF055_data.dataBuff[5]<<8;
	DATA[3]=NDOF055_data.dataBuff[6]|NDOF055_data.dataBuff[7]<<8;

	NDOF055_data.eular.yaw =(float)DATA[0]/100;
	NDOF055_data.eular.pitch =(float)DATA[2]/100;
	NDOF055_data.gyro.z =(float)DATA[1]/16;
	NDOF055_data.gyro.x =(float)DATA[3]/16;
}

/**
  * @brief  从CAN报文中获取信息
  * @param  *RxMessage 	CAN报文接收结构体指针
  * @retval None
  */
void NDOF055_getInfo(CanRxMsg RxMessage)
{
	//报文id确认
	if(RxMessage.StdId != NDOF055_DATA_READID)
		return;
	
	memcpy(NDOF055_data.dataBuff, RxMessage.Data, sizeof(uint8_t[8]));
	NDOF055_Analysis();
	
	NDOF055_data.infoUpdateFrame++;
	NDOF055_data.infoUpdateFlag = 1;
}

/**
  * @brief  NDOF055_IMU串口初始化
  * @param  None
  * @retval None
  */
void NDOF_IMU_Init(void)
{
	if (isInit) return;
	
  /*CAN2通讯*/
	CAN2_QuickInit();
		
	/*初始化完成*/
	isInit = true;
}

/**
  * @brief  NDOF055_IMU初始化验证
  * @param  None
  * @retval isInit 0失败 1成功
  */
bool NDOF_IMUTest(void)
{
	return isInit;
}
