#include "GM3510_Motor.h"
/******************************************************************************/
GM3510_t  GM3510s;
/******************************************************************************/
/**
  * @brief  从CAN报文中获取GM3510电机信息
  * @param  [in]  RxMessage 	CAN报文接收结构体
  * @retval None
  */
void GM3510_getInfo(uint8_t* CanRxBuff, uint32_t MotorID)
{
	GM3510s.realAngle = (uint16_t)(CanRxBuff[0]<<8 \
	                                        | CanRxBuff[1]);
	GM3510s.realTorque = (int16_t)(CanRxBuff[2]<<8 \
	                                       | CanRxBuff[3]);
	/*过零处理开始*/
		/*总角度*/
	if(GM3510s.realAngle - GM3510s.lastAngle < -6000)
	{
		GM3510s.turnCount++;
	}
	if(GM3510s.lastAngle - GM3510s.realAngle < -6000)
	{
		GM3510s.turnCount--;
	}
	GM3510s.totalAngle = GM3510s.realAngle + \
															(8192*GM3510s.turnCount);
	
	GM3510s.lastAngle =  GM3510s.realAngle;
	/*过零处理结束*/
	
	/*帧率更新*/
	GM3510s.infoUpdateFrame++;
	/*更新标志*/
	GM3510s.infoUpdateFlag = 1;
}

/**
  * @brief  设置GM3510电机电流值，范围-29000~0~29000
  * @param  [in]  iq5 对应id号0x205电机的电流值
  * 		[in]  iq6 对应id号0x206电机的电流值
  * 		[in]  iq7 对应id号0x207电机的电流值
  * @retval None
  */
void GM3510_setCurrent(int16_t iq5, int16_t iq6, int16_t iq7)
{

	uint8_t data[8];

	data[0] = iq5 >> 8;
	data[1] = iq5;
	data[2] = iq6 >> 8;
	data[3] = iq6;
	data[4] = iq7 >> 8;
	data[5] = iq7;
	data[6] = 0 >> 8;
	data[7] = 0;


	CAN_SendData(CAN1, CAN_ID_STD, GM3510_SENDID, data);
	
}	


