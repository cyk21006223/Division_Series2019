#include "LED.h"

/******************************************************************************/
/* LED�������ȼ� */
static ledseq_t const * sequences[] = 
{
	seq_DR16,
	seq_YAW,
	seq_PITCH,
	seq_KRELOAD,
	seq_GY_IMU,
	seq_NDOF_IMU,
	seq_Ultrasonic_L,
	seq_Ultrasonic_R,
	seq_JudgeSystem,
};


/*Led ����*/
ledseq_t const seq_DR16[] = 	/*ң�ؽ�������*/
{
	{ true, LEDSEQ_WAITMS(1000)},
	{    0, LEDSEQ_LOOP},
};
ledseq_t const seq_YAW[] = 	/*Yaw�������*/
{
	{ true, LEDSEQ_WAITMS(1000)},
	{    0, LEDSEQ_LOOP},
};
ledseq_t const seq_PITCH[] = 	/*Pitch�������*/
{
	{ true, LEDSEQ_WAITMS(1000)},
	{    0, LEDSEQ_LOOP},
};
ledseq_t const seq_KRELOAD[] = 	/*�����������*/
{
	{ true, LEDSEQ_WAITMS(1000)},
	{    0, LEDSEQ_LOOP},
};
ledseq_t const seq_GY_IMU[] = 	/*GY_IMU����*/
{
	{ true, LEDSEQ_WAITMS(1000)},
	{    0, LEDSEQ_LOOP},
};
ledseq_t const seq_NDOF_IMU[] = 	/*NDOF_IMU����*/
{
	{ true, LEDSEQ_WAITMS(1000)},
	{    0, LEDSEQ_LOOP},
};
ledseq_t const seq_Ultrasonic_L[] = 	/*Ultrasonic_Left����*/
{
	{ true, LEDSEQ_WAITMS(1000)},
	{    0, LEDSEQ_LOOP},
};
ledseq_t const seq_Ultrasonic_R[] = 	/*Ultrasonic_Right����*/
{
	{ true, LEDSEQ_WAITMS(1000)},
	{    0, LEDSEQ_LOOP},
};
ledseq_t const seq_JudgeSystem[] = 	/*JudgeSystem����*/
{
	{ true, LEDSEQ_WAITMS(1000)},
	{    0, LEDSEQ_LOOP},
};

static void updateActive(LEDMark_e led);		/*����led��������ȼ�����*/
static int getPrio(const ledseq_t *seq);	/*��ȡled���ȼ�*/
static void runLedseq(xTimerHandle xTimer);

static bool isInit = false;
static bool ledseqEnabled = true;
static int activeSeq[LED_NUM];		/*ÿ��LED��Ӧ�Ļ���ȼ�����*/
static int state[LED_NUM][SEQ_NUM];	/*ÿ��LED��Ӧ�����еĵ�ǰλ��*/

static xTimerHandle timer[LED_NUM];	/*��ʱ�����*/
static xSemaphoreHandle ledseqSem;	/*�ź���*/

void ledseqInit()
{
	int i,j;
	if(isInit) return;
	
	LED_Init();
	
	/*��ʼ����������״̬*/
	for(i=0; i<LED_NUM; i++) 
	{
		activeSeq[i] = LEDSEQ_STOP;
		for(j=0; j<SEQ_NUM; j++)
			state[i][j] = LEDSEQ_STOP;
	}
	
	/*���������ʱ��*/
	for(i=0; i<LED_NUM; i++)
		timer[i] = xTimerCreate("ledseqTimer", \
	                          1000, \
	                          pdFALSE, \
	                          (void*)i, \
														runLedseq);
 
	vSemaphoreCreateBinary(ledseqSem);	/*����һ��2ֵ�ź���*/

	isInit = true;
}
bool ledseqTest(void)
{
	bool status;

	status = isInit & ledTest();
	ledseqEnable(true);
	return status;
}

void ledseqEnable(bool enable)
{
	ledseqEnabled = enable;
}

void ledseqSetTimes(ledseq_t *sequence, int onTime, int offTime)
{
	sequence[0].action = onTime;
	sequence[1].action = offTime;
}

/*����led��sequence����*/
void ledseqRun(LEDMark_e led, const ledseq_t *sequence)
{
	int prio = getPrio(sequence);	/*��ȡled���ȼ�����*/

	if(prio<0) return;

	xSemaphoreTake(ledseqSem, portMAX_DELAY);
	state[led][prio] = 0; 
	updateActive(led);
	xSemaphoreGive(ledseqSem);

	if(activeSeq[led] == prio)	/*��ǰ�������ȼ����ڻ�������ȼ�*/
		runLedseq(timer[led]);
}

/*ֹͣled��sequence����*/
void ledseqStop(LEDMark_e led, const ledseq_t *sequence)
{
	int prio = getPrio(sequence);/*��ȡled���ȼ�����*/

	if(prio<0) return;

	xSemaphoreTake(ledseqSem, portMAX_DELAY);
	state[led][prio] = LEDSEQ_STOP;  
	updateActive(led);
	xSemaphoreGive(ledseqSem);

	runLedseq(timer[led]);
}

/*FreeRTOS ��ʱ���ص�����*/
static void runLedseq( xTimerHandle xTimer )
{
	bool leave = false;
	const ledseq_t *step;
	LEDMark_e led = (LEDMark_e)pvTimerGetTimerID(xTimer);

	if (!ledseqEnabled) return;

	while(!leave) 
	{
		int prio = activeSeq[led];

		if (prio == LEDSEQ_STOP)
			return;

		step = &sequences[prio][state[led][prio]];

		state[led][prio]++;

		xSemaphoreTake(ledseqSem, portMAX_DELAY);
		switch(step->action)
		{
			case LEDSEQ_LOOP:
				state[led][prio] = 0;
				break;
			case LEDSEQ_STOP:
				state[led][prio] = LEDSEQ_STOP;
				updateActive(led);
				break;
			default:  /*LED��ʱ*/
				LedSet(led, step->value);	/*��ʱstep->value*/
				if (step->action == 0)
					break;
				xTimerChangePeriod(xTimer, step->action, 0);
				xTimerStart(xTimer, 0);
				leave=true;
				break;
		}
		xSemaphoreGive(ledseqSem);
	}
}

/*��ȡled�������ȼ�*/
static int getPrio(const ledseq_t *seq)
{
	int prio;

	for(prio=0; prio<SEQ_NUM; prio++)
		if(sequences[prio]==seq) return prio;

	return -1; /*��Ч����*/
}

/*����led��������ȼ�����*/
static void updateActive(LEDMark_e led)
{
	int prio;

	LedSet(led, false);
	activeSeq[led]=LEDSEQ_STOP;
	
	for(prio=0;prio<SEQ_NUM;prio++)
	{
		if (state[led][prio] != LEDSEQ_STOP)
		{
			activeSeq[led]=prio;
			break;
		}
	}
}
/* LED���� */
bool ledTest(void)
{
	LedSet(LED_D8_1, ON);
	LedSet(LED_GREEN, ON);
	LedSet(LED_RED, ON);
	delay_xms(2500);
	
	LedSet(LED_D8_1, OFF);
	LedSet(LED_GREEN, OFF);
	LedSet(LED_RED, OFF);
	delay_xms(2500);

	LedAllOFF();

	return isInit;
}
/******************************************************************************/
static led_t leds[LED_NUM+1] =
{
#if USE_BOARDTYPE == 0
	[LED_D1]	    = {LED_D8_1_GPIO_PORT, LED_D1_PIN, OFF},
	[LED_D2]	    = {LED_D8_1_GPIO_PORT, LED_D2_PIN, OFF},
	[LED_D3]	    = {LED_D8_1_GPIO_PORT, LED_D3_PIN, OFF},
	[LED_D4]	    = {LED_D8_1_GPIO_PORT, LED_D4_PIN, OFF},
	[LED_D5]	    = {LED_D8_1_GPIO_PORT, LED_D5_PIN, OFF},
	[LED_D6]	    = {LED_D8_1_GPIO_PORT, LED_D6_PIN, OFF},
	[LED_D7]	    = {LED_D8_1_GPIO_PORT, LED_D7_PIN, OFF},
	[LED_D8]	    = {LED_D8_1_GPIO_PORT, LED_D8_PIN, OFF},
	[LED_D8_1]	  = {LED_D8_1_GPIO_PORT, LED_D8_1_GPIO_PIN, OFF},
#endif
	[LED_GREEN] 	= {LED_GREEN_GPIO_PORT, LED_GREEN_PIN, OFF},
	[LED_RED]	    = {LED_RED_GPIO_PORT, LED_RED_PIN, OFF},
};
/**
  * @brief  LED�Ƴ�ʼ��
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(LED_ALL_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed;
	
	GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN;
	GPIO_Init(LED_GREEN_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LED_RED_PIN;
	GPIO_Init(LED_RED_GPIO_PORT, &GPIO_InitStructure);
#if USE_BOARDTYPE == 0	
	GPIO_InitStructure.GPIO_Pin = LED_D8_1_GPIO_PIN;
	GPIO_Init(LED_D8_1_GPIO_PORT, &GPIO_InitStructure);
	LedSet(LED_D8_1,OFF);
#endif
	LedSet(LED_GREEN,OFF);
	LedSet(LED_RED,OFF);
}


/**
  * @brief  LED_State,����MCU��������������ؼ��
  * @param  None
  * @retval None
  */
void LED_State(void)
{
	bool pass = true;
	
	pass &= ledseqTest();
	pass &= PowerCtrlTest();
//	pass &= ComAssistantTest();
	pass &= DR16_receiverTest();
	pass &= RM_MotorTest();	
	pass &= IMUTest();
//	pass &= UltrasonicTest();
	pass &= JudgeSystemTest();
//	pass &= N3_DJI_InitTest();
	
	static uint8_t cnt;
	if(pass == true)
	{
		while(cnt++ < 5)	/*��ʼ��ͨ�� �̵ƿ���5��*/
		{
			LedFlashOne(LED_GREEN, 50, 50);
		}
	}
	else{
		while(cnt++ < 5)	/*��ʼ��ʧ�� ��ƿ���5��*/
		{
			LedFlashOne(LED_RED, 50, 50);
		}
	}
}


/**
  * @brief  �ر�����LED
  * @param  None
  * @retval None
  */
void LedAllOFF(void)
{	
	for(u8 i=0; i<LED_NUM; i++)
	{
		LedSet((LEDMark_e)i, OFF);
	}
}


/**
  * @brief  ������LED
  * @param  None
  * @retval None
  */
void LedAllON(void)
{
	for(u8 i=0; i<LED_NUM; i++)
	{
		LedSet((LEDMark_e)i, ON);
	}
}

/**
  * @brief  LED��˸1��
  * @param  None
  * @retval None
  */
void LedFlashOne(LEDMark_e led, u32 onTime, u32 offTime)
{
	LedSet(led, ON);
	delay_xms(onTime);
	LedSet(led, OFF);
	delay_xms(offTime);
}
/**
  * @brief  ����ĳ��LED��״̬
  * @param  [in] led    led���  LEDMark_e
  * @param  [in] status  ON��  OFF��
  * @retval None
  */
void LedSet(LEDMark_e led, LEDState_e status)
{
	if (led>LED_NUM+1)
		return;

	GPIO_WriteBit(leds[led].GPIOx,leds[led].GPIO_Pin,(BitAction)status);
}




