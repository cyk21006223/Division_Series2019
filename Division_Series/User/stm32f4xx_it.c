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
      {/*ϵͳ�Ѿ�����*/
    #endif  /* INCLUDE_xTaskGetSchedulerState */  
        xPortSysTickHandler();
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )
      }else
	    {
		   sysTickCnt++;	/*���ȿ���֮ǰ����*/
	    }
    #endif  /* INCLUDE_xTaskGetSchedulerState */
}
/**
  * @brief  ��ȡϵͳʱ��
  * @param  None
  * @retval None
  */
uint32_t getSysTickCnt(void)
{
	//���ȿ���֮ǰ ���� xTaskGetTickCount()
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)	/*ϵͳ�Ѿ�����*/
		return xTaskGetTickCount();
	else//���ȿ���֮ǰ ���� sysTickCnt
		return sysTickCnt;
}

/**
  * @brief  USART1�жϷ�����������DR16������
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{			

	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		/*�ر�DMA*/
		DMA_Cmd(USART1_RX_DMA_STREAM, DISABLE);
		/*����Ƿ�ʧ�ܳɹ���DMAʧ��ʱ��Ҫ�ȴ�����ʱ���ʧ�ܳɹ�*/
		while(DMA_GetCmdStatus(USART1_RX_DMA_STREAM));
	
		/*��ȡDMAbuffʣ���С���Ƿ�ƥ��*/
		if (DMA_GetCurrDataCounter(USART1_RX_DMA_STREAM) == 2)
		{		
			DR16_processData(DR16_rxBuff);
		}

		/*���ô������ݳ��ȣ�+2��֤��ȫ*/
		DMA_SetCurrDataCounter(USART1_RX_DMA_STREAM,DR16_DBUS_PACKSIZE+2);
		DMA_Cmd(USART1_RX_DMA_STREAM, ENABLE);

		/*��������жϱ�־λ*/
		(void)USART1->DR;
		(void)USART1->SR;
	}

}
#if USE_BOARDTYPE == 1
/**
  * @brief  USART3�жϷ���������̨IMU���ݴ���
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		/*�ر�DMA*/
		DMA_Cmd(USART3_RX_DMA_STREAM, DISABLE);
		/*����Ƿ�ʧ�ܳɹ���DMAʧ��ʱ��Ҫ�ȴ�����ʱ���ʧ�ܳɹ�*/
		while(DMA_GetCmdStatus(USART3_RX_DMA_STREAM));

		GY_Counter = DMA_GetCurrDataCounter(USART3_RX_DMA_STREAM);
		
		GY_IMU_getInfo(	Cloud_GY_IMU_RXBUFF, 
										GY_IMU_BUFFSIZE-GY_Counter,
										&Cloud_IMU);

		
		/*���ô������ݳ���*/
    DMA_SetCurrDataCounter(USART3_RX_DMA_STREAM,GY_IMU_BUFFSIZE);

		/*��DMA*/
		DMA_Cmd(USART3_RX_DMA_STREAM, ENABLE);
	}
					
		/*��������жϱ�־λ*/
		(void)USART3->DR;
		(void)USART3->SR;
}
#endif

#if USE_BOARDTYPE == 0
/**
  * @brief  USART3�жϷ�����,����ϵͳ
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		/*�ر�DMA*/
		DMA_Cmd(USART3_RX_DMA_STREAM, DISABLE);
	  /*����Ƿ�ʧ�ܳɹ���DMAʧ��ʱ��Ҫ�ȴ�����ʱ���ʧ�ܳɹ�*/
		while(DMA_GetCmdStatus(USART3_RX_DMA_STREAM));
		
		JudgeDMALenth = DMA_GetCurrDataCounter(USART3_RX_DMA_STREAM);
		
		JudgeInfo_Process(Judge_DataBuff, Judge_BuffSIZE - JudgeDMALenth);
		
		/*���ô������ݳ���*/
    DMA_SetCurrDataCounter(USART3_RX_DMA_STREAM,GY_IMU_BUFFSIZE);

		/*��DMA*/
		DMA_Cmd(USART3_RX_DMA_STREAM, ENABLE);
	}
					
		/*��������жϱ�־λ*/
		(void)USART3->DR;
		(void)USART3->SR;
}
/**
  * @brief  ����8-DMA�жϣ���̨IMU���ݴ���
  * @param  None
  * @retval None
  */
void UART8_IRQHandler(void)
{
	if(USART_GetITStatus(UART8, USART_IT_IDLE) != RESET)
	{
		
		/*�ر�DMA*/
		DMA_Cmd(UART8_RX_DMA_STREAM, DISABLE);
	  /*����Ƿ�ʧ�ܳɹ���DMAʧ��ʱ��Ҫ�ȴ�����ʱ���ʧ�ܳɹ�*/
		while(DMA_GetCmdStatus(UART8_RX_DMA_STREAM));
		
		#if USE_IMUBYUSART == 1
		GY_Counter = DMA_GetCurrDataCounter(UART8_RX_DMA_STREAM);
		
		GY_IMU_getInfo(	Cloud_GY_IMU_RXBUFF, 
										GY_IMU_BUFFSIZE-GY_Counter,
										&Cloud_IMU);
		/*���ô������ݳ���*/
    DMA_SetCurrDataCounter(UART8_RX_DMA_STREAM,GY_IMU_BUFFSIZE);
		
		#elif USE_IMUBYUSART == 0
		JY61_Counter = DMA_GetCurrDataCounter(UART8_RX_DMA_STREAM);
		
		if(JY61_Counter!=0)
		JY61_DataProcess( JY61_reBuff, 
										  JY61_PACKAGE_LENGTH+2,
										  &JY61Data);
		/*���ô������ݳ���*/
    DMA_SetCurrDataCounter(UART8_RX_DMA_STREAM,JY61_PACKAGE_LENGTH);
		#elif USE_IMUBYUSART == 2
		JY901_Counter = DMA_GetCurrDataCounter(UART8_RX_DMA_STREAM);
		
		if(JY901_Counter!=0)
		JY901_DataProcess( JY901_reBuff, 
										  JY901_PACKAGE_LENGTH+2,
										  &JY901Data);
		/*���ô������ݳ���*/
    DMA_SetCurrDataCounter(UART8_RX_DMA_STREAM,JY901_PACKAGE_LENGTH);
		#endif


		/*��DMA*/
		DMA_Cmd(UART8_RX_DMA_STREAM, ENABLE);
	}
					
		/*��������жϱ�־λ*/
		(void)UART8->DR;
		(void)UART8->SR;
}
#endif

/**
  * @brief  CAN1 FIFO0 �����жϷ�����
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

    /*CAN1�������*/		
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
	}	
}
#if USE_BOARDTYPE == 0 && USE_NDOF055_CAN ==1
/**
  * @brief  CAN2 FIFO0 �����жϷ�����
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
  * @brief  TIM6 �жϺ���10ms
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
  * @brief  TIM7 �жϺ���10ms
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
