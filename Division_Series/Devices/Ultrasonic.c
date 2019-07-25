#include "Ultrasonic.h"
/******************************************************************************/
Ultrasonic_t  Ultrasonic;
uint8_t IOE_RXBUFF[IOE_PACKAGE_LENGTH];
static bool isInit = false;
/******************************************************************************/
#if USE_ULTRASONIC == 0
/*记录定时器溢出次数*/
unsigned int overcount=0;
#endif
/******************************************************************************/
#if USE_ULTRASONIC == 0
/**
* @brief         超声波解析,计算并保存距离
* @param[in]     TRIG_PORT  触发
* @param[in]     TRIG_PIN		触发
* @param[in]     ECHO_PORT	回响
* @param[in]     ECHO_PIN		回响
* @param[in]     TIMx				用于计时的定时器
* @param[in]     length			距离
* @param[out]    None
* @return        None
*/
static void Ultrasonic_analyseData( GPIO_TypeDef*  TRIG_PORT, 
																	  uint32_t       TRIG_PIN,
																	  GPIO_TypeDef*  ECHO_PORT,
																	  uint32_t       ECHO_PIN,
																	  TIM_TypeDef*   TIMx,
																	  float          length)
{
		float sum = 0;
		u16 tim;
		unsigned int  i = 0;
		int num=5;
		
	/*测5次数据计算一次平均值*/
	while(i != num)
	{
		GPIO_SetBits(TRIG_PORT, TRIG_PIN);  //拉高信号，作为触发信号
		delay_us(20);  						//高电平信号超过10us
		GPIO_ResetBits(TRIG_PORT, TRIG_PIN);

		/*等待回响信号*/
		while(GPIO_ReadInputDataBit(ECHO_PORT, ECHO_PIN) == RESET);
		TIM_Cmd(TIMx,ENABLE);//回响信号到来，开启定时器计数
		
		/*帧率检测*/
		Ultrasonic.infoUpdateFrame++;
		
		i+=1; //每收到一次回响信号+1，收到5次就计算均值
		while(GPIO_ReadInputDataBit(ECHO_PORT, ECHO_PIN) == SET);//回响信号消失
		TIM_Cmd(TIMx, DISABLE);//关闭定时器
		
		tim = TIM_GetCounter(TIMx);//获取TIM2计数寄存器中的计数值，并计算回响时间
		
		length = (tim + overcount * 1000) / 58.0;//通过回响信号计算距离
		
		sum = length + sum;
		TIMx->CNT = 0;  //将TIM2计数寄存器的计数值清零
		overcount = 0;  //中断溢出次数清零
		delay_us(10);   //测量周期10us
	}
	length = sum *10 / num;//单位厘米，大于4厘米为有效值
}
/**
* @brief         TIM2中断，保存溢出次数
* @param[in]     None
* @return        None
*/
void TIM2_IRQHandler(void) 
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清除中断标志
		/*当回响信号很长时，计数值溢出后重复计数，用中断来保存溢出次数*/
		overcount++;	
	}
}
/**
* @brief         获取超声波距离
* @param[in]     None
* @param[out]    Ultrasonic.length[x],保存距离,单位厘米
* @return        None
*/
void Ultrasonic_getDistance(void)
{
	
	Ultrasonic_analyseData( SR04_TRIG_PORT,
												  SR04_TRIG_PIN,
												  SR04_ECHO_PORT,
												  SR04_ECHO_PIN,
												  TIM2,
												  Ultrasonic.length/*保存距离*/
												 );
	
	GPIO_ToggleBits(LED_D8_1_GPIO_PORT, LED_D8_PIN);
}
#endif
#if USE_ULTRASONIC == 1
/**
  * @brief   IOE_SR05数据接收
  * @param   [in]  buff	数据包
	*					 [in]  dataLength
	*					 [in]  IMU's master
  * @retval  NULL
  */
void IOE_getInfo(uint8_t* buff, uint16_t dataLength, Ultrasonic_t* dataOut)
{
	
	for(uint16_t n = 0; n < dataLength; )
		{
			/*匹配帧头*/
			if( buff[n] == 0xFF )
			{
				if(IOE_Checksum(&buff[n]))
				{
					/*以毫米为单位*/
					dataOut->length=(buff[n+1]<<8) | buff[n+2];

					/*帧率检测*/
					dataOut->infoUpdateFrame++;
					dataOut->infoUpdateFlag = 1;
					
					return;
				}
				else{
					n++;
				}
				
			}
			else{
				n++;
			}
		}
	
}
/**
  * @brief   IOE_SR05数据包校验
  * @param   [in]  buff	数据包
  * @retval  0 校验失败，1校验通过
  */
uint8_t IOE_Checksum(uint8_t* buff)
{
	uint8_t sum = 0;
	
	for(uint8_t i=0; i<IOE_PACKAGE_LENGTH-1; i++){
		sum += buff[i]; 
	}
	
	if(sum == buff[IOE_PACKAGE_LENGTH-1]){
		//校验通过
		return 1;
	}
	else{
		return 0;
	}
}
#endif
/**
* @brief         初始化超声波,抢占优先级3
* @note          A型板无上拉电阻无法用SR04
* @param[in]     None
* @param[out]    None
* @return        None
*/
void Ultrasonic_Init(void)
{
	if (isInit) return;
#if USE_ULTRASONIC == 0
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	GPIO_QuickInit(	SR04_ECHO_RCC,
									SR04_ECHO_PORT,
									SR04_ECHO_PIN,
									GPIO_Mode_IN,
									0x00,
									GPIO_PuPd_UP);
	
	GPIO_QuickInit(	SR04_TRIG_RCC,
									SR04_TRIG_PORT,
									SR04_TRIG_PIN,
									GPIO_Mode_OUT,
									GPIO_OType_PP,
									GPIO_PuPd_NOPULL);
	
	/*定时器TIM2初始化*/
	TIM_DeInit(TIM2);
	TIM_TimeBaseInitStructer.TIM_Period = 9999;//定时周期为9999
	TIM_TimeBaseInitStructer.TIM_Prescaler = 999; //分频系数999
	TIM_TimeBaseInitStructer.TIM_ClockDivision = TIM_CKD_DIV1;//不分频
	TIM_TimeBaseInitStructer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructer);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//开启更新中断

	/*定时器中断初始化*/
	NVIC_QuickInit(TIM2_IRQn,3,0);

	TIM_Cmd(TIM2, DISABLE);//关闭定时器使能

	GPIO_SetBits(SR04_TRIG_PORT , SR04_TRIG_PIN);
#endif
#if USE_ULTRASONIC == 1
  USART6_QuickInit(9600);
  USART6_RXDMA_Config((uint32_t)IOE_RXBUFF,IOE_PACKAGE_LENGTH);
#endif
	/*初始化完成*/
	isInit = true;
}
/**
  * @brief  超声波初始化验证
  * @param  None
  * @retval isInit 0失败 1成功
  */
bool UltrasonicTest(void)
{
	return isInit;
}
