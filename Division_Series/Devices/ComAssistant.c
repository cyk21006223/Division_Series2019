#include "ComAssistant.h"

static bool isInit = false;

/**
  * @brief  串口调试初始化
  * @param  None
  * @retval None
  */
void ComAssistant_Init(void)
{
	if (isInit) return;

	/*串口2初始化-XCOM、Bluetooth*/
	USART2_QuickInit(115200);//抢占优先级3

	/*初始化完成*/
	isInit = true;
}


/**
  * @brief  串口调试初始化验证
  * @param  None
  * @retval isInit 0失败 1成功
  */
bool ComAssistantTest(void)
{
	return isInit;
}

