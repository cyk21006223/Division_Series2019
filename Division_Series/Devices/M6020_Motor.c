#include "M6020_Motor.h"
/******************************************************************************/
M6020_t M6020s;
/******************************************************************************/
/**
  * @brief  从CAN报文中获取M6020电机信息
  * @param  [in]  RxMessage 	CAN报文接收结构体
  * @retval None
  */
void M6020_getInfo(uint8_t* CanRxBuff, uint32_t MotorID)
{
	
	if(MotorID != YAWMOTOR_RECEIVEID)
	{
		return;
	}
	
	M6020s.realAngle = (uint16_t)(CanRxBuff[0]<<8 \
	                                        | CanRxBuff[1]);
	M6020s.realSpeed = (int16_t)(CanRxBuff[2]<<8 \
	                                       | CanRxBuff[3]);
	M6020s.realTorque = (int16_t)(CanRxBuff[4]<<8 \
	                                        | CanRxBuff[5]);
	M6020s.temperture = (uint16_t)CanRxBuff[6];
	
	/*过零处理开始*/
	if(M6020s.realAngle - M6020s.lastAngle < -6000)
	{
		M6020s.turnCount++;
	}
	if(M6020s.lastAngle - M6020s.realAngle < -6000)
	{
		M6020s.turnCount--;
	}
	M6020s.totalAngle = M6020s.realAngle + \
																(8192*M6020s.turnCount);
	
	M6020s.lastAngle =  M6020s.realAngle;
	/*过零处理结束*/
	
	/*帧率更新*/
	M6020s.infoUpdateFrame++;
	/*更新标志*/
	M6020s.infoUpdateFlag = 1;
}

/**
  * @brief  设置M6020电机电流值
  * @param  iqy 对应id号(0x20y)电机的电流值，范围-30000~0~30000
  * @retval None
  */
void M6020_setCurrent(int16_t iq5, int16_t iq6, int16_t iq7, int16_t iq8)
{

	uint8_t data[8];

	//数据格式详见说明书P13
	data[0] = iq5 >> 8;
	data[1] = iq5;
	data[2] = iq6 >> 8;
	data[3] = iq6;
	data[4] = iq7 >> 8;
	data[5] = iq7;
	data[6] = iq8 >> 8;
	data[7] = iq8;


	CAN_SendData(CAN1, CAN_ID_STD, M6020_SENDID, data);
	
}	
	
