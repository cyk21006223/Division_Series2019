#include "UserControl.h"

ControlStatus_t ControlStatus;

/**
  * @brief  ��ȡ��ǰģʽ
  * @param  None
  * @retval uint8_t  ��ǰ�Ŀ���ģʽ
  */

uint8_t Control_getMode(void)
{
   return ControlStatus.ControlMode;
}


/**
  * @brief   ң����RC������
  * @param   None
  * @retval	 None
  */
void Control_ByRC(void)
{
	/*ң�������ߴ���*/ 
	if( DR16.offLineFlag == 1 )
	{
		/*�ر�Ħ����*/
		fric_turnOff();
		/*�رղ������*/
		Shoot_turnoff();
		/*������̨*/
		Cloud_setTargetAngle(Cloud_Yaw, Cloud_Yaw_Motor.totalAngle);
	  Cloud_setTargetAngle(Cloud_Pitch, Cloud_Pitch_Motor.totalAngle);
	}
	else if( DR16.rc.s_left == DR16_SWITCH_UP )
	{
	  ControlStatus.ControlMode=ControlMode_OFF;
	}
	else 
		ControlStatus.ControlMode=ControlMode_ByRC;
	
  /*ģ��������ģʽ*/
	if( ControlStatus.ControlMode==ControlMode_OFF )
	{
		/*��̨������������ϵ�*/
	 	Cloud_setCurrent(0, 0, 0);
		/*�ر�Ħ����*/
		fric_turnOff();
		/*�رղ������*/
		Shoot_turnoff();
	}
	
	/*ң����RC����ģʽ*/
	if( ControlStatus.ControlMode==ControlMode_ByRC )
	{
		/*��˲��£�����ģʽ*/
		Shoot_single_shot(DR16.rc.s_right,DR16_SWITCH_DOWN);

		/*�Ҹ˲��ϣ�����ģʽ*/
		Shoot_Running_fire(DR16.rc.s_right,DR16_SWITCH_UP);
		
		if(DR16.rc.s_right == DR16_SWITCH_MID)
		{
			Shoot_turnoff();//��������ر�
		}
		
		/*��̨��IMU��RC����ģʽ*/
//   Cloud_processing_withoutIMU(dr16_rocker_L.x*Cloud.Sensity[Cloud_Yaw],\
		                            dr16_rocker_L.y*Cloud.Sensity[Cloud_Pitch]);
		
		/*��̨��IMU��RC����ģʽ*/
		 Cloud_processing_withIMU(dr16_rocker_L.x*Cloud.Sensity[Cloud_Yaw],\
		                            dr16_rocker_L.y*Cloud.Sensity[Cloud_Pitch]);
  }
}

/**
  * @brief   ң����PC������
  * @param   None
  * @retval	 None
  */
void Control_ByPC(void)
{
	;
}

