#include "DR16_Receiver.h"

/******************************************************************************/
static bool isInit = false;
DR16_DBUS_DATA_t DR16;
rocker_t dr16_rocker_L,dr16_rocker_R;	
uint8_t DR16_rxBuff[DR16_DBUS_PACKSIZE+2]; 	//����buff

/*����̰���־*/
uint32_t KeyMouse_Click_Press_Flag;
/*���󳤰���־*/
uint32_t KeyMouse_Long_Press_Flag;
/*�����±�־*/
uint32_t KeyMouse_Press_Flag;
/*�����¼�ʱ*/
uint32_t KeyMousePressTime[KEYMOUSE_Index];

/******************************************************************************/

/**
  * @brief  DR16���ջ���ʼ��
  * @param  None
  * @retval None
  */
void DR16_receiverInit(void)
{
	if (isInit) return;
	USART1_QuickInit(100000);//��ռ���ȼ�3
	USART1_RXDMA_Config((uint32_t)DR16_rxBuff, DR16_DBUS_PACKSIZE+2);	//+2��֤��ȫ
	/*��ʼ�����*/
	isInit = true;
}
/**
  * @brief  DR16���ջ���ʼ����֤
  * @param  None
  * @retval isInit 0ʧ�� 1�ɹ�
  */
bool DR16_receiverTest(void)
{
	return isInit;
}

/**
* @brief	DR16�������ݴ���
* @param [in] pData ����buff����ָ��
* @retval None
*/
void DR16_processData(uint8_t *pData)
{
	/*���뿪ʼ*/
	if (pData == NULL)
		return;
		
	DR16.rc.ch0 = (pData[0] | (pData[1] << 8)) & 0x07FF;
	DR16.rc.ch0 -=1024;
	DR16.rc.ch1 = ((pData[1] >> 3) | (pData[2] << 5)) & 0x07FF;
	DR16.rc.ch1 -=1024;
	DR16.rc.ch2 = ((pData[2] >> 6) | (pData[3] << 2) | (pData[4] << 10)) & 0x07FF;
	DR16.rc.ch2 -=1024;
	DR16.rc.ch3 = ((pData[4] >> 1) | (pData[5] << 7)) & 0x07FF;
	DR16.rc.ch3 -=1024;
	
	DR16.rc.s_left = ((pData[5] >> 4) & 0x000C) >> 2;
	DR16.rc.s_right = ((pData[5] >> 4) & 0x0003) ;
	
	DR16.mouse.x = (pData[6]) | (pData[7] << 8);
	DR16.mouse.y = (pData[8]) | (pData[9] << 8);
	DR16.mouse.z = (pData[10]) | (pData[11] << 8);
	
	DR16.mouse.keyLeft = pData[12];
	DR16.mouse.keyRight = pData[13];
	
	DR16.keyBoard.key_code = pData[14]| (pData[15] << 8);
	
	DR16.thumbwheel = pData[16] | (pData[17] << 8);
	DR16.thumbwheel -=1024;
	/*�������*/
	
	DR16.infoUpdateFrame++;

	/* prevent remote control zero deviation */
	if(DR16.rc.ch0 <= 20 && DR16.rc.ch0 >= -20)
		DR16.rc.ch0 = 0;
	if(DR16.rc.ch1 <= 20 && DR16.rc.ch1 >= -20)
		DR16.rc.ch1 = 0;
	if(DR16.rc.ch2 <= 20 && DR16.rc.ch2 >= -20)
		DR16.rc.ch2 = 0;
	if(DR16.rc.ch3 <= 20 && DR16.rc.ch3 >= -20)
		DR16.rc.ch3 = 0;
	if(DR16.thumbwheel>DR16_ROCKER_MAXVALUE)
		DR16.thumbwheel=0;

	/*��ҡ��X(ch2)Y(ch3),��ҡ��X(ch0)Y(ch1)*/
	Rocker_getData(DR16.rc.ch2, DR16.rc.ch3, &dr16_rocker_L);
	Rocker_getData(DR16.rc.ch0, DR16.rc.ch1, &dr16_rocker_R);

}

/**
  * @brief  ������궯�����
  * @param  void
  * @retval void
  */
void KeyMouseActionMonitor(void)
{
	/*�������*/
	uint32_t Index;
	
	/*���浱ǰ���а����ļ����*/
	uint32_t KeyMouseData = (uint32_t)DR16.keyBoard.key_code
													|(uint32_t)DR16.mouse.keyLeft<<16
													|(uint32_t)DR16.mouse.keyRight<<17;
	
	/*�������а������ж�*/
	for(Index=0; Index<KEYMOUSE_Index; Index++)
	{
		if(KeyMouseData & ((uint32_t)1<<Index))
		{
			/*���¼�ʱ*/
			KeyMousePressTime[Index]++;
			
			/*�Ƿ����ж�*/
			if(KeyMousePressTime[Index] > KeyMouse_Click_Time)
			{
				/*���򰴼����±�־��1*/
				KeyMouse_Press_Flag |= ((uint32_t)1<<Index);
			}
			
			/*�Ƿ񳤰��ж�*/
			if(KeyMousePressTime[Index] > KeyMouse_Long_Time)
			{
				/*���򳤰���־��1*/
				KeyMouse_Long_Press_Flag |= ((uint32_t)1<<Index);
			}
		}
		/*����δ�����ж�*/
		else
		{
			/*�Ƿ�̰��ж�*/
			if((KeyMousePressTime[Index] > KeyMouse_Click_Time)
					&&(KeyMousePressTime[Index] < KeyMouse_Long_Time))
			{
				/*�̰���־��1*/
				KeyMouse_Click_Press_Flag |= ((uint32_t)1<<Index);
			}
			else
			{
				/*����̰���־��0*/
				KeyMouse_Click_Press_Flag &=~ ((uint32_t)1<<Index);
			}
			
			/*����δ������,���±�־��������־��0*/
			KeyMouse_Press_Flag &=~ ((uint32_t)1<<Index);
			KeyMouse_Long_Press_Flag	&=~ ((uint32_t)1<<Index);
			
			/*��ʱ����*/
			KeyMousePressTime[Index] = 0;
		}
	}
}

/**
	* @brief  ��ȡ�����̶���
  * @param	��ֵ  ����
  * @retval ���ؼ�����״̬  0 û�иö��� 1 �иö���
  */
uint8_t GetKeyMouseAction(KeyMouseData_e KeyMouse, KeyMouseActionType Action)
{
	switch(Action)
	{
		case Click_Press://����
			return (KeyMouse_Click_Press_Flag>>KeyMouse) & 1;
			break;
		case Press://����
			return (KeyMouse_Press_Flag>>KeyMouse) & 1;
			break;
		case Long_Press://����
			return (KeyMouse_Long_Press_Flag>>KeyMouse) & 1;
			break;
		default:
			return 0;
			break;
	}
}
/**
	* @brief  ������ֹͬʱ���������
  * @param	None
  * @retval None
  */
bool KeyPRO(void)
{
  if( (GetKeyMouseAction(KEY_C,Press)
		&& GetKeyMouseAction(KEY_V,Press))//KEY_C && KEY_V
	  
	  || (GetKeyMouseAction(KEY_C,Press)
		&& GetKeyMouseAction(KEY_B,Press))//KEY_C && KEY_B
	  
		|| (GetKeyMouseAction(KEY_B,Press)
		&& GetKeyMouseAction(KEY_V,Press))//KEY_B && KEY_V
	
		|| (GetKeyMouseAction(KEY_C,Press)
		&& GetKeyMouseAction(KEY_V,Press)
	  && GetKeyMouseAction(KEY_B,Press))//KEY_C && KEY_V && KEY_B
	
	  ||((dr16_rocker_L.y > 400)
		&&(dr16_rocker_L.x > 400))	//y���ϣ�x����
	
	  ||((dr16_rocker_L.y > 400)
		&&(dr16_rocker_L.x < -400)) //y���ϣ�x����
	
	  ||((dr16_rocker_L.x > 400)
		&&(dr16_rocker_L.y < -400)) //x���ң�y����
 
	  ||((dr16_rocker_L.y < -400)
		&&(dr16_rocker_L.x < -400))//y���£�x����
	)
	{
	  return 0;
	}
	else{
	  return 1;
	}
}