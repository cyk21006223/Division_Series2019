#include "PowerCtrl.h"


static bool isInit = false;

/**
  * @brief  设置某个可控电源的状态
  * @param  [in] PowerCtrl_N    N标号  PowerCtrlMark_e
	*              PowerCtrl_1    J20    右下
	*              PowerCtrl_2    J22    左上
	*              PowerCtrl_3    J19    左下
	*              PowerCtrl_4    J21    右上
  * @param  [in] value          Open开    Close关
  * @retval None
  */
void PowerCtrlSet(PowerCtrlMark_e PowerCtrl_N, PowerCtrl_e value)
{
	uint16_t GPIO_Pin;
	
	switch (PowerCtrl_N)
	{
		case PowerCtrl_1:GPIO_Pin=GPIO_Pin_2;
	                   break;
		case PowerCtrl_2:GPIO_Pin=GPIO_Pin_3;
  		               break;
		case PowerCtrl_3:GPIO_Pin=GPIO_Pin_4;
  		               break;
		case PowerCtrl_4:GPIO_Pin=GPIO_Pin_5;
  		               break;
  	default: break;
	}

	GPIO_WriteBit(POWERCTRL4_1_GPIO_PORT,GPIO_Pin,(BitAction)value);
}

/**
  * @brief  可控电源输出全部失能
  * @param  None
  * @retval None
  */
void PowerCtrl_AllDISABLE(void)
{
	GPIO_ResetBits(POWERCTRL4_1_GPIO_PORT, POWERCTRL4_1_PIN);
}

/**
  * @brief  可控电源输出全部使能
  * @param  None
  * @retval None
  */
void PowerCtrl_AllENABLE(void)
{
	GPIO_SetBits(POWERCTRL4_1_GPIO_PORT, POWERCTRL4_1_PIN);
}

/**
  * @brief  可控电源输出初始化
  * @param  None
  * @retval None
  */
void PowerCtrl_Init(void)
{
	if (isInit) return;
	
  GPIO_QuickInit(POWERCTRL4_1_GPIO_CLK, 
								 POWERCTRL4_1_GPIO_PORT, 
								 POWERCTRL4_1_PIN, 
								 GPIO_Mode_OUT, 
								 GPIO_OType_PP, 
								 GPIO_PuPd_NOPULL);

	PowerCtrl_AllENABLE();
	
	isInit = true;
}

/**
  * @brief  可控电源输出初始化验证
  * @param  None
  * @retval isInit 0失败 1成功
  */
bool PowerCtrlTest(void)
{
	return isInit;
}
