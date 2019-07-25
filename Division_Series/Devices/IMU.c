#include "IMU.h"

static bool isInit = false;

/**
  * @brief  IMU初始化
  * @param  None
  * @retval None
  */
void IMU_Init(void)
{
	if (isInit) return;

	/*NDOF陀螺仪初始化-陀螺仪CAN2通讯*/
	#if USE_BOARDTYPE == 0 && USE_NDOF055_CAN ==1
	NDOF_IMU_Init();
	#endif
	
	/*GY、JY陀螺仪初始化-串口通讯*/
	#if USE_IMUBYUSART == 1
	GY_IMU_Init();				//抢占优先级3
	#elif USE_IMUBYUSART == 0
	JY61IMU_Init();
	#elif USE_IMUBYUSART == 2
	JY901IMU_Init();
	#endif

	/*初始化完成*/
	isInit = true;
}


/**
  * @brief  IMU初始化验证
  * @param  None
  * @retval isInit 0失败 1成功
  */
bool IMUTest(void)
{
	return isInit;
}