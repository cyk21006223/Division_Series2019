#include "DR16_Receiver.h"

/******************************************************************************/
static bool isInit = false;
DR16_DBUS_DATA_t DR16;
rocker_t dr16_rocker_L,dr16_rocker_R;	
uint8_t DR16_rxBuff[DR16_DBUS_PACKSIZE+2]; 	//接收buff

/*键鼠短按标志*/
uint32_t KeyMouse_Click_Press_Flag;
/*键鼠长按标志*/
uint32_t KeyMouse_Long_Press_Flag;
/*键鼠按下标志*/
uint32_t KeyMouse_Press_Flag;
/*键鼠按下计时*/
uint32_t KeyMousePressTime[KEYMOUSE_Index];

/******************************************************************************/

/**
  * @brief  DR16接收机初始化
  * @param  None
  * @retval None
  */
void DR16_receiverInit(void)
{
	if (isInit) return;
	USART1_QuickInit(100000);//抢占优先级3
	USART1_RXDMA_Config((uint32_t)DR16_rxBuff, DR16_DBUS_PACKSIZE+2);	//+2保证安全
	/*初始化完成*/
	isInit = true;
}
/**
  * @brief  DR16接收机初始化验证
  * @param  None
  * @retval isInit 0失败 1成功
  */
bool DR16_receiverTest(void)
{
	return isInit;
}

/**
* @brief	DR16接收数据处理
* @param [in] pData 接收buff数据指针
* @retval None
*/
void DR16_processData(uint8_t *pData)
{
	/*解码开始*/
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
	/*解码结束*/
	
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

	/*左摇杆X(ch2)Y(ch3),右摇杆X(ch0)Y(ch1)*/
	Rocker_getData(DR16.rc.ch2, DR16.rc.ch3, &dr16_rocker_L);
	Rocker_getData(DR16.rc.ch0, DR16.rc.ch1, &dr16_rocker_R);

}

/**
  * @brief  键盘鼠标动作检测
  * @param  void
  * @retval void
  */
void KeyMouseActionMonitor(void)
{
	/*索引编号*/
	uint32_t Index;
	
	/*保存当前所有按键的检测结果*/
	uint32_t KeyMouseData = (uint32_t)DR16.keyBoard.key_code
													|(uint32_t)DR16.mouse.keyLeft<<16
													|(uint32_t)DR16.mouse.keyRight<<17;
	
	/*检索所有按键并判断*/
	for(Index=0; Index<KEYMOUSE_Index; Index++)
	{
		if(KeyMouseData & ((uint32_t)1<<Index))
		{
			/*按下计时*/
			KeyMousePressTime[Index]++;
			
			/*是否按下判断*/
			if(KeyMousePressTime[Index] > KeyMouse_Click_Time)
			{
				/*是则按键按下标志置1*/
				KeyMouse_Press_Flag |= ((uint32_t)1<<Index);
			}
			
			/*是否长按判断*/
			if(KeyMousePressTime[Index] > KeyMouse_Long_Time)
			{
				/*是则长按标志置1*/
				KeyMouse_Long_Press_Flag |= ((uint32_t)1<<Index);
			}
		}
		/*按键未按下判断*/
		else
		{
			/*是否短按判断*/
			if((KeyMousePressTime[Index] > KeyMouse_Click_Time)
					&&(KeyMousePressTime[Index] < KeyMouse_Long_Time))
			{
				/*短按标志置1*/
				KeyMouse_Click_Press_Flag |= ((uint32_t)1<<Index);
			}
			else
			{
				/*否则短按标志置0*/
				KeyMouse_Click_Press_Flag &=~ ((uint32_t)1<<Index);
			}
			
			/*按键未被按下,按下标志、长按标志置0*/
			KeyMouse_Press_Flag &=~ ((uint32_t)1<<Index);
			KeyMouse_Long_Press_Flag	&=~ ((uint32_t)1<<Index);
			
			/*计时清零*/
			KeyMousePressTime[Index] = 0;
		}
	}
}

/**
	* @brief  获取鼠标键盘动作
  * @param	键值  动作
  * @retval 返回键鼠动作状态  0 没有该动作 1 有该动作
  */
uint8_t GetKeyMouseAction(KeyMouseData_e KeyMouse, KeyMouseActionType Action)
{
	switch(Action)
	{
		case Click_Press://单击
			return (KeyMouse_Click_Press_Flag>>KeyMouse) & 1;
			break;
		case Press://按下
			return (KeyMouse_Press_Flag>>KeyMouse) & 1;
			break;
		case Long_Press://长按
			return (KeyMouse_Long_Press_Flag>>KeyMouse) & 1;
			break;
		default:
			return 0;
			break;
	}
}
/**
	* @brief  按键防止同时按下误操作
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
		&&(dr16_rocker_L.x > 400))	//y最上，x最右
	
	  ||((dr16_rocker_L.y > 400)
		&&(dr16_rocker_L.x < -400)) //y最上，x最左
	
	  ||((dr16_rocker_L.x > 400)
		&&(dr16_rocker_L.y < -400)) //x最右，y最下
 
	  ||((dr16_rocker_L.y < -400)
		&&(dr16_rocker_L.x < -400))//y最下，x最左
	)
	{
	  return 0;
	}
	else{
	  return 1;
	}
}