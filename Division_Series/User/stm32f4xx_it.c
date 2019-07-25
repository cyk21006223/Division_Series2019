#include "Users.h"
#include "stm32f4xx_it.h"

static u32 sysTickCnt=0;

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{	
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )
      if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
      {/*系统已经运行*/
    #endif  /* INCLUDE_xTaskGetSchedulerState */  
        xPortSysTickHandler();
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )
      }else
	    {
		   sysTickCnt++;	/*调度开启之前计数*/
	    }
    #endif  /* INCLUDE_xTaskGetSchedulerState */
}
/**
  * @brief  获取系统时间
  * @param  None
  * @retval None
  */
uint32_t getSysTickCnt(void)
{
	//调度开启之前 返回 xTaskGetTickCount()
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)	/*系统已经运行*/
		return xTaskGetTickCount();
	else//调度开启之前 返回 sysTickCnt
		return sysTickCnt;
}

/**
  * @brief  USART1中断服务函数，处理DR16的数据
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{			

	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		/*关闭DMA*/
		DMA_Cmd(USART1_RX_DMA_STREAM, DISABLE);
		/*检测是否失能成功，DMA失能时需要等待少许时间才失能成功*/
		while(DMA_GetCmdStatus(USART1_RX_DMA_STREAM));
	
		/*获取DMAbuff剩余大小，是否匹配*/
		if (DMA_GetCurrDataCounter(USART1_RX_DMA_STREAM) == 2)
		{		
			DR16_processData(DR16_rxBuff);
		}

		/*设置传输数据长度，+2保证安全*/
		DMA_SetCurrDataCounter(USART1_RX_DMA_STREAM,DR16_DBUS_PACKSIZE+2);
		DMA_Cmd(USART1_RX_DMA_STREAM, ENABLE);

		/*清除空闲中断标志位*/
		(void)USART1->DR;
		(void)USART1->SR;
	}

}
#if USE_BOARDTYPE == 1
/**
  * @brief  USART3中断服务函数，云台IMU数据处理
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		/*关闭DMA*/
		DMA_Cmd(USART3_RX_DMA_STREAM, DISABLE);
		/*检测是否失能成功，DMA失能时需要等待少许时间才失能成功*/
		while(DMA_GetCmdStatus(USART3_RX_DMA_STREAM));

		GY_Counter = DMA_GetCurrDataCounter(USART3_RX_DMA_STREAM);
		
		GY_IMU_getInfo(	Cloud_GY_IMU_RXBUFF, 
										GY_IMU_BUFFSIZE-GY_Counter,
										&Cloud_IMU);

		
		/*设置传输数据长度*/
    DMA_SetCurrDataCounter(USART3_RX_DMA_STREAM,GY_IMU_BUFFSIZE);

		/*打开DMA*/
		DMA_Cmd(USART3_RX_DMA_STREAM, ENABLE);
	}
					
		/*清除空闲中断标志位*/
		(void)USART3->DR;
		(void)USART3->SR;
}
#endif

#if USE_BOARDTYPE == 0
/**
  * @brief  USART3中断服务函数,裁判系统
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		/*关闭DMA*/
		DMA_Cmd(USART3_RX_DMA_STREAM, DISABLE);
	  /*检测是否失能成功，DMA失能时需要等待少许时间才失能成功*/
		while(DMA_GetCmdStatus(USART3_RX_DMA_STREAM));
		
		JudgeDMALenth = DMA_GetCurrDataCounter(USART3_RX_DMA_STREAM);
		
		JudgeInfo_Process(Judge_DataBuff, Judge_BuffSIZE - JudgeDMALenth);
		
		/*设置传输数据长度*/
    DMA_SetCurrDataCounter(USART3_RX_DMA_STREAM,GY_IMU_BUFFSIZE);

		/*打开DMA*/
		DMA_Cmd(USART3_RX_DMA_STREAM, ENABLE);
	}
					
		/*清除空闲中断标志位*/
		(void)USART3->DR;
		(void)USART3->SR;
}
/**
  * @brief  串口8-DMA中断，云台IMU数据处理
  * @param  None
  * @retval None
  */
void UART8_IRQHandler(void)
{
	if(USART_GetITStatus(UART8, USART_IT_IDLE) != RESET)
	{
		
		/*关闭DMA*/
		DMA_Cmd(UART8_RX_DMA_STREAM, DISABLE);
	  /*检测是否失能成功，DMA失能时需要等待少许时间才失能成功*/
		while(DMA_GetCmdStatus(UART8_RX_DMA_STREAM));
		
		#if USE_IMUBYUSART == 1
		GY_Counter = DMA_GetCurrDataCounter(UART8_RX_DMA_STREAM);
		
		GY_IMU_getInfo(	Cloud_GY_IMU_RXBUFF, 
										GY_IMU_BUFFSIZE-GY_Counter,
										&Cloud_IMU);
		/*设置传输数据长度*/
    DMA_SetCurrDataCounter(UART8_RX_DMA_STREAM,GY_IMU_BUFFSIZE);
		
		#elif USE_IMUBYUSART == 0
		JY61_Counter = DMA_GetCurrDataCounter(UART8_RX_DMA_STREAM);
		
		if(JY61_Counter!=0)
		JY61_DataProcess( JY61_reBuff, 
										  JY61_PACKAGE_LENGTH+2,
										  &JY61Data);
		/*设置传输数据长度*/
    DMA_SetCurrDataCounter(UART8_RX_DMA_STREAM,JY61_PACKAGE_LENGTH);
		#elif USE_IMUBYUSART == 2
		JY901_Counter = DMA_GetCurrDataCounter(UART8_RX_DMA_STREAM);
		
		if(JY901_Counter!=0)
		JY901_DataProcess( JY901_reBuff, 
										  JY901_PACKAGE_LENGTH+2,
										  &JY901Data);
		/*设置传输数据长度*/
    DMA_SetCurrDataCounter(UART8_RX_DMA_STREAM,JY901_PACKAGE_LENGTH);
		#endif


		/*打开DMA*/
		DMA_Cmd(UART8_RX_DMA_STREAM, ENABLE);
	}
					
		/*清除空闲中断标志位*/
		(void)UART8->DR;
		(void)UART8->SR;
}
#endif

/**
  * @brief  CAN1 FIFO0 接收中断服务函数
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	
	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0))
	{
		
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		
		switch (RxMessage.StdId)
		{
		case YAWMOTOR_RECEIVEID: M6020_getInfo(RxMessage.Data,RxMessage.StdId);
																	break;
		case PITCHMOTOR_RECEIVEID: M2006_getInfo(RxMessage.Data,RxMessage.StdId);
																	break;
		case KRELOADMOTOR_RECEIVEID: M2006_getInfo(RxMessage.Data,RxMessage.StdId);
																	break;
		default: break;
		}

    /*CAN1接收清除*/		
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
	}	
}
#if USE_BOARDTYPE == 0 && USE_NDOF055_CAN ==1
/**
  * @brief  CAN2 FIFO0 接收中断服务函数
  * @param  None
  * @retval None
  */
void CAN2_RX0_IRQHandler(void){

	if(CAN_GetITStatus(CAN2, CAN_IT_FMP0))
	{
		CanRxMsg RxMessage;	
		CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
		
		NDOF055_getInfo(RxMessage);

		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
	}
	
}
#endif
/**
  * @brief  TIM6 中断函数10ms
  * @param  None
  * @retval None
  */
void TIM6_DAC_IRQHandler(void)
{	
	 if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)                                  
	{ 
		
   TIM_ClearITPendingBit(TIM6,TIM_IT_Update);                              
	}   
 
}

/**
  * @brief  TIM7 中断函数10ms
  * @param  None
  * @retval None
  */
void TIM7_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
  {		
			
		
		
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  		 
	}	
}
