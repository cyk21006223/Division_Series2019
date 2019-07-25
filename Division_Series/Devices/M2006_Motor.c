#include "M2006_Motor.h"
/******************************************************************************/
M2006s_t M2006s[2] ;
/******************************************************************************/
/**
  * @brief  从CAN报文中获取M2006电机信息
  * @param  [in]  RxMessage 	CAN报文接收结构体
  * @retval None
  */
void M2006_getInfo(uint8_t *CanRxBuff, uint32_t MotorID)
{
	uint32_t StdId;
	
	if(MotorID==PITCHMOTOR_RECEIVEID)
	{
		StdId = PITCHMOTOR;
	}
	if(MotorID==KRELOADMOTOR_RECEIVEID)
	{
		StdId = KRELOADMOTOR;
	}
	
	M2006s[StdId].realAngle = (uint16_t)(CanRxBuff[0]<<8 \
																				| CanRxBuff[1]);
	M2006s[StdId].realSpeed = (int16_t)(CanRxBuff[2]<<8 \
																				| CanRxBuff[3]);
	M2006s[StdId].realTorque = (int16_t)(CanRxBuff[4]<<8 \
																				| CanRxBuff[5]);
	
	/*过零处理开始*/
	if(M2006s[StdId].realAngle - M2006s[StdId].lastAngle < -6000)
	{
		M2006s[StdId].turnCount++;
	}
	if(M2006s[StdId].lastAngle - M2006s[StdId].realAngle < -6000)
	{
		M2006s[StdId].turnCount--;
	}
	M2006s[StdId].totalAngle = M2006s[StdId].realAngle + \
															( 8192 * M2006s[StdId].turnCount);
	M2006s[StdId].lastAngle =  M2006s[StdId].realAngle;
	/*过零处理结束*/
	
	/*转向*/
	if(M2006s[StdId].totalAngle - M2006s[StdId].lasttotalAngle < 0)
	{
		M2006s[StdId].KreloadVeer=0;
	}
	else if(M2006s[StdId].totalAngle - M2006s[StdId].lasttotalAngle > 0)
	{
		M2006s[StdId].KreloadVeer=1;
	}
	M2006s[StdId].lasttotalAngle = M2006s[StdId].totalAngle;
	
	/*帧率更新*/
	M2006s[StdId].infoUpdateFrame++;
	
	/*更新标志*/
	M2006s[StdId].infoUpdateFlag = 1;
}

/**
  * @brief  设置M2006电机电流值（id号为7）
  * @param  iqy 对应id号(0x20y)电机的电流值，范围-10000~0~10000
  * @retval None
  */
void M2006_setCurrent(int16_t iq7)
{

	uint8_t data[8];
	
	/*数据格式详见说明书P32*/
//	data[0] = iq5 >> 8;
//	data[1] = iq5;
//	data[2] = iq6 >> 8;
//	data[3] = iq6;
//	data[4] = iq7 >> 8;
//	data[5] = iq7;
//	data[6] = iq8 >> 8;
//	data[7] = iq8;

	
	CAN_SendData(CAN1, CAN_ID_STD, M2006_SENDID, data);
	
}	


/**
  * @brief  设定M2006电机的目标角度
  * @param  motorName 	电机名字 @ref M2006Name_e
  *			    angle		    电流值，范围 0~8191
  * @retval None
  */
void M2006_setTargetAngle(M2006Name_e motorName, int32_t angle)
{
	M2006s[motorName].targetAngle = angle;
}

/**
  * @brief  设定M2006电机的目标速度
  * @param  motorName 	电机名字 @ref M2006Name_e
  *			    speed		    电流值，范围 -10000~0~10000
  * @retval None
  */
void M2006_setTargetSpeed(M2006Name_e motorName, int16_t speed)
{
	M2006s[motorName].targetSpeed = speed;
}



