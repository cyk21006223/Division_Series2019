#include "UserControl.h"

ControlStatus_t ControlStatus;

/**
  * @brief  获取当前模式
  * @param  None
  * @retval uint8_t  当前的控制模式
  */

uint8_t Control_getMode(void)
{
   return ControlStatus.ControlMode;
}


/**
  * @brief   遥控器RC处理函数
  * @param   None
  * @retval	 None
  */
void Control_ByRC(void)
{
	/*遥控器离线处理*/ 
	if( DR16.offLineFlag == 1 )
	{
		/*关闭摩擦轮*/
		fric_turnOff();
		/*关闭拨弹电机*/
		Shoot_turnoff();
		/*锁定云台*/
		Cloud_setTargetAngle(Cloud_Yaw, Cloud_Yaw_Motor.totalAngle);
	  Cloud_setTargetAngle(Cloud_Pitch, Cloud_Pitch_Motor.totalAngle);
	}
	else if( DR16.rc.s_left == DR16_SWITCH_UP )
	{
	  ControlStatus.ControlMode=ControlMode_OFF;
	}
	else 
		ControlStatus.ControlMode=ControlMode_ByRC;
	
  /*模拟掉电控制模式*/
	if( ControlStatus.ControlMode==ControlMode_OFF )
	{
		/*云台、拨弹电机不上电*/
	 	Cloud_setCurrent(0, 0, 0);
		/*关闭摩擦轮*/
		fric_turnOff();
		/*关闭拨弹电机*/
		Shoot_turnoff();
	}
	
	/*遥控器RC控制模式*/
	if( ControlStatus.ControlMode==ControlMode_ByRC )
	{
		/*左杆拨下，单发模式*/
		Shoot_single_shot(DR16.rc.s_right,DR16_SWITCH_DOWN);

		/*右杆拨上，连发模式*/
		Shoot_Running_fire(DR16.rc.s_right,DR16_SWITCH_UP);
		
		if(DR16.rc.s_right == DR16_SWITCH_MID)
		{
			Shoot_turnoff();//拨弹电机关闭
		}
		
		/*云台无IMU，RC控制模式*/
//   Cloud_processing_withoutIMU(dr16_rocker_L.x*Cloud.Sensity[Cloud_Yaw],\
		                            dr16_rocker_L.y*Cloud.Sensity[Cloud_Pitch]);
		
		/*云台带IMU，RC控制模式*/
		 Cloud_processing_withIMU(dr16_rocker_L.x*Cloud.Sensity[Cloud_Yaw],\
		                            dr16_rocker_L.y*Cloud.Sensity[Cloud_Pitch]);
  }
}

/**
  * @brief   遥控器PC处理函数
  * @param   None
  * @retval	 None
  */
void Control_ByPC(void)
{
	;
}

