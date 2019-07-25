#include "Ultrasonic.h"
/******************************************************************************/
Ultrasonic_t  Ultrasonic;
uint8_t IOE_RXBUFF[IOE_PACKAGE_LENGTH];
static bool isInit = false;
/******************************************************************************/
#if USE_ULTRASONIC == 0
/*��¼��ʱ���������*/
unsigned int overcount=0;
#endif
/******************************************************************************/
#if USE_ULTRASONIC == 0
/**
* @brief         ����������,���㲢�������
* @param[in]     TRIG_PORT  ����
* @param[in]     TRIG_PIN		����
* @param[in]     ECHO_PORT	����
* @param[in]     ECHO_PIN		����
* @param[in]     TIMx				���ڼ�ʱ�Ķ�ʱ��
* @param[in]     length			����
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
		
	/*��5�����ݼ���һ��ƽ��ֵ*/
	while(i != num)
	{
		GPIO_SetBits(TRIG_PORT, TRIG_PIN);  //�����źţ���Ϊ�����ź�
		delay_us(20);  						//�ߵ�ƽ�źų���10us
		GPIO_ResetBits(TRIG_PORT, TRIG_PIN);

		/*�ȴ������ź�*/
		while(GPIO_ReadInputDataBit(ECHO_PORT, ECHO_PIN) == RESET);
		TIM_Cmd(TIMx,ENABLE);//�����źŵ�����������ʱ������
		
		/*֡�ʼ��*/
		Ultrasonic.infoUpdateFrame++;
		
		i+=1; //ÿ�յ�һ�λ����ź�+1���յ�5�ξͼ����ֵ
		while(GPIO_ReadInputDataBit(ECHO_PORT, ECHO_PIN) == SET);//�����ź���ʧ
		TIM_Cmd(TIMx, DISABLE);//�رն�ʱ��
		
		tim = TIM_GetCounter(TIMx);//��ȡTIM2�����Ĵ����еļ���ֵ�����������ʱ��
		
		length = (tim + overcount * 1000) / 58.0;//ͨ�������źż������
		
		sum = length + sum;
		TIMx->CNT = 0;  //��TIM2�����Ĵ����ļ���ֵ����
		overcount = 0;  //�ж������������
		delay_us(10);   //��������10us
	}
	length = sum *10 / num;//��λ���ף�����4����Ϊ��Чֵ
}
/**
* @brief         TIM2�жϣ������������
* @param[in]     None
* @return        None
*/
void TIM2_IRQHandler(void) 
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//����жϱ�־
		/*�������źźܳ�ʱ������ֵ������ظ����������ж��������������*/
		overcount++;	
	}
}
/**
* @brief         ��ȡ����������
* @param[in]     None
* @param[out]    Ultrasonic.length[x],�������,��λ����
* @return        None
*/
void Ultrasonic_getDistance(void)
{
	
	Ultrasonic_analyseData( SR04_TRIG_PORT,
												  SR04_TRIG_PIN,
												  SR04_ECHO_PORT,
												  SR04_ECHO_PIN,
												  TIM2,
												  Ultrasonic.length/*�������*/
												 );
	
	GPIO_ToggleBits(LED_D8_1_GPIO_PORT, LED_D8_PIN);
}
#endif
#if USE_ULTRASONIC == 1
/**
  * @brief   IOE_SR05���ݽ���
  * @param   [in]  buff	���ݰ�
	*					 [in]  dataLength
	*					 [in]  IMU's master
  * @retval  NULL
  */
void IOE_getInfo(uint8_t* buff, uint16_t dataLength, Ultrasonic_t* dataOut)
{
	
	for(uint16_t n = 0; n < dataLength; )
		{
			/*ƥ��֡ͷ*/
			if( buff[n] == 0xFF )
			{
				if(IOE_Checksum(&buff[n]))
				{
					/*�Ժ���Ϊ��λ*/
					dataOut->length=(buff[n+1]<<8) | buff[n+2];

					/*֡�ʼ��*/
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
  * @brief   IOE_SR05���ݰ�У��
  * @param   [in]  buff	���ݰ�
  * @retval  0 У��ʧ�ܣ�1У��ͨ��
  */
uint8_t IOE_Checksum(uint8_t* buff)
{
	uint8_t sum = 0;
	
	for(uint8_t i=0; i<IOE_PACKAGE_LENGTH-1; i++){
		sum += buff[i]; 
	}
	
	if(sum == buff[IOE_PACKAGE_LENGTH-1]){
		//У��ͨ��
		return 1;
	}
	else{
		return 0;
	}
}
#endif
/**
* @brief         ��ʼ��������,��ռ���ȼ�3
* @note          A�Ͱ������������޷���SR04
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
	
	/*��ʱ��TIM2��ʼ��*/
	TIM_DeInit(TIM2);
	TIM_TimeBaseInitStructer.TIM_Period = 9999;//��ʱ����Ϊ9999
	TIM_TimeBaseInitStructer.TIM_Prescaler = 999; //��Ƶϵ��999
	TIM_TimeBaseInitStructer.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseInitStructer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructer);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//���������ж�

	/*��ʱ���жϳ�ʼ��*/
	NVIC_QuickInit(TIM2_IRQn,3,0);

	TIM_Cmd(TIM2, DISABLE);//�رն�ʱ��ʹ��

	GPIO_SetBits(SR04_TRIG_PORT , SR04_TRIG_PIN);
#endif
#if USE_ULTRASONIC == 1
  USART6_QuickInit(9600);
  USART6_RXDMA_Config((uint32_t)IOE_RXBUFF,IOE_PACKAGE_LENGTH);
#endif
	/*��ʼ�����*/
	isInit = true;
}
/**
  * @brief  ��������ʼ����֤
  * @param  None
  * @retval isInit 0ʧ�� 1�ɹ�
  */
bool UltrasonicTest(void)
{
	return isInit;
}
