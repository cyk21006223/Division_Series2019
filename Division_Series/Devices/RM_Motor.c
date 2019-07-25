#include "RM_Motor.h"

static bool isInit = false;

/**
  * @brief  RM电机初始化
  * @param  None
  * @retval None
  */
void RM_MotorInit(void)
{
	if (isInit) return;

  /*CAN1初始化-电机CAN1通讯*/
	CAN1_QuickInit();			//抢占优先级3

	/*初始化完成*/
	isInit = true;
}

/**
  * @brief  RM电机初始化验证
  * @param  None
  * @retval isInit 0失败 1成功
  */
bool RM_MotorTest(void)
{
	return isInit;
}
/**
  * @brief  无人机-设置云台与拨弹电机电流值并发送给电调
  * @param[in] YawCurrent 对应yaw轴电机(id 0x205)的电流值,范围-30000~0~30000
  *	@param[in] PitchCurrent 对应pitch轴电机(id 0x206)的电流值,范围-29000~0~29000
  *	@param[in] KreloadCurrent 对应拨弹电机(id 0x207)电流值,范围-10000~0~10000
  * @retval None
  */
void Aerial_setCurrent(int16_t YawCurrent, \
	                     int16_t PitchCurrent, \
                       int16_t KreloadCurrent)
{
   RM_Motor_setCurrent(CAN1, YawCurrent, PitchCurrent, KreloadCurrent, NULL);
}

/**
  * @brief  RM电机-电流值并发送给电调
  * @param[in] CANx  CAN1 or CAN2 
  * @param[in] currentFirst 
  *	@param[in] currentSecond 
  *	@param[in] currentThird 
  *	@param[in] currentFourth 
  * @retval None
  */
void RM_Motor_setCurrent(CAN_TypeDef* CANx, \
	                       int16_t currentFirst, \
	                       int16_t currentSecond, \
                         int16_t currentThird, \
                         int16_t currentFourth)
{
	/*数据长度(最大为8字节)*/
	uint8_t data[8];
	
	/*数据格式详见说明书P32*/
	data[0] = currentFirst >> 8;
	data[1] = currentFirst;
	data[2] = currentSecond >> 8;
	data[3] = currentSecond;
	data[4] = currentThird >> 8;
	data[5] = currentThird;
	data[6] = currentFourth >> 8;
	data[7] = currentFourth;

	CAN_SendData(CANx, CAN_ID_STD, 0x1FF, data);
}	


