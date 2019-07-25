#include "N3_DJI_ASSISITANT.h"
/******************************************************************************/

#define N3_Value_Low   1000
#define N3_Value_Mid   2000
#define N3_Value_High  2600

/******************************************************************************/

int FilterNum = 4;
N3_DJI_ASSISITANT_t N3_Semaphore;
static bool isInit = false;

/******************************************************************************/

/**
* @brief         N3_DJI_ASSISITANT信号获取
* @param[in]     N3_Semaphore.Value   接收到的PWM信号
* @param[out]    战术模式 0默认战术 1战术 2战术
* @return        None
*/
float FlagMode(float Value)
{
  if(Value>=0 && Value<=N3_Value_Low)
	{
	  return 0;
	}
	
	if(Value>=N3_Value_Low && Value<=N3_Value_Mid)
	{
	  return 1;
	}
	
	if(Value>=N3_Value_Mid && Value<=N3_Value_High)
	{
	  return 2;
	}
}

/**
* @brief         N3_DJI_ASSISITANT信号获取
* @param[in]     None
* @param[out]    N3_Semaphore.Value   接收到的PWM信号
* @return        None
*/
float N3_Semaphore_getValue(void){
	return N3_Semaphore.Value;
}

/**
* @brief         初始化N3_DJI_ASSISITANT,抢占优先级3
* @Note          接线G蓝色引脚
* @param[in]     None
* @return        None
*/
void N3_DJI_ASSISITANT_Init(void)
{
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	GPIO_QuickInit(N3_DJI_GPIO_CLK, \
	               N3_DJI_GPIO_PORT, \
								 N3_DJI_PIN, \
								 GPIO_Mode_AF, \
								 GPIO_OType_PP, \
								 GPIO_PuPd_DOWN);
				   
	GPIO_PinAFConfig(N3_DJI_GPIO_PORT, N3_DJI_PinSource, GPIO_AF_TIM4);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	//配置定时器时钟分频
	TIM_TimeBaseStructure.TIM_Prescaler = 89;
	//配置自动重装载寄存器的值
	TIM_TimeBaseStructure.TIM_Period = 1000-1;
	//配置死区时间与采样滤波分频，不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//配置计数模式，向上计数
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	//输入捕获配置
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	/* Enable the CC2 Interrupt Request */
	TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
	
	TIM_Cmd(TIM4, ENABLE);
   
  /* Enable the TIM4 global Interrupt */
	NVIC_QuickInit(TIM4_IRQn,3,0);
	
	/*初始化完成*/
	isInit = true;
}
/**
  * @brief  N3_DJI_ASSISITANT初始化验证
  * @param  None
  * @retval isInit 0失败 1成功
  */
bool N3_DJI_InitTest(void)
{
	return isInit;
}

/**
  * @brief  This function handles TIM4 global interrupt request.
  * @param  None
  * @retval None
  */
//void TIM4_IRQHandler(void)
//{
//	if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET){
//		
//		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
//		if(N3_Semaphore.captureFlag == 0){
//			//捕获到上升沿
//			N3_Semaphore.captureRising = TIM_GetCapture2(TIM4);
//			TIM_OC4PolarityConfig(TIM4, TIM_ICPolarity_Falling);//设置为下降沿捕获
//			N3_Semaphore.captureFlag = 1;
//		}
//		else{
//			//捕获到下降沿
//			N3_Semaphore.captureFalling = TIM_GetCapture2(TIM4);
//			
//			//下降沿的计数值比上升沿还小证明计数器溢出过
//			if (N3_Semaphore.captureFalling < N3_Semaphore.captureRising)
//				N3_Semaphore.captureValue = 65535;
//			else
//				N3_Semaphore.captureValue = 0;
//			
//			/*定时循环采样*/
//			N3_Semaphore.captureValue += N3_Semaphore.captureFalling \
//			                             - N3_Semaphore.captureRising;
//			
////			N3_Semaphore.Value = Filter_Slide_Mean_Value(N3_Semaphore.captureValue,FilterNum);
//			N3_Semaphore.Value = N3_Semaphore.captureValue;
//			
//			TIM_OC4PolarityConfig(TIM4, TIM_ICPolarity_Rising); //设置为上升沿捕获
//			N3_Semaphore.captureFlag = 0;
//		}
//		
//	}
//}


