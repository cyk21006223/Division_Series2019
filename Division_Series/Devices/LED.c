#include "LED.h"

/******************************************************************************/
/* LED序列优先级 */
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


/*Led 序列*/
ledseq_t const seq_DR16[] = 	/*遥控接收序列*/
{
	{ true, LEDSEQ_WAITMS(1000)},
	{    0, LEDSEQ_LOOP},
};
ledseq_t const seq_YAW[] = 	/*Yaw电机序列*/
{
	{ true, LEDSEQ_WAITMS(1000)},
	{    0, LEDSEQ_LOOP},
};
ledseq_t const seq_PITCH[] = 	/*Pitch电机序列*/
{
	{ true, LEDSEQ_WAITMS(1000)},
	{    0, LEDSEQ_LOOP},
};
ledseq_t const seq_KRELOAD[] = 	/*拨弹电机序列*/
{
	{ true, LEDSEQ_WAITMS(1000)},
	{    0, LEDSEQ_LOOP},
};
ledseq_t const seq_GY_IMU[] = 	/*GY_IMU序列*/
{
	{ true, LEDSEQ_WAITMS(1000)},
	{    0, LEDSEQ_LOOP},
};
ledseq_t const seq_NDOF_IMU[] = 	/*NDOF_IMU序列*/
{
	{ true, LEDSEQ_WAITMS(1000)},
	{    0, LEDSEQ_LOOP},
};
ledseq_t const seq_Ultrasonic_L[] = 	/*Ultrasonic_Left序列*/
{
	{ true, LEDSEQ_WAITMS(1000)},
	{    0, LEDSEQ_LOOP},
};
ledseq_t const seq_Ultrasonic_R[] = 	/*Ultrasonic_Right序列*/
{
	{ true, LEDSEQ_WAITMS(1000)},
	{    0, LEDSEQ_LOOP},
};
ledseq_t const seq_JudgeSystem[] = 	/*JudgeSystem序列*/
{
	{ true, LEDSEQ_WAITMS(1000)},
	{    0, LEDSEQ_LOOP},
};

static void updateActive(LEDMark_e led);		/*更新led的最高优先级序列*/
static int getPrio(const ledseq_t *seq);	/*获取led优先级*/
static void runLedseq(xTimerHandle xTimer);

static bool isInit = false;
static bool ledseqEnabled = true;
static int activeSeq[LED_NUM];		/*每个LED对应的活动优先级序列*/
static int state[LED_NUM][SEQ_NUM];	/*每个LED对应的序列的当前位置*/

static xTimerHandle timer[LED_NUM];	/*定时器句柄*/
static xSemaphoreHandle ledseqSem;	/*信号量*/

void ledseqInit()
{
	int i,j;
	if(isInit) return;
	
	LED_Init();
	
	/*初始化各个序列状态*/
	for(i=0; i<LED_NUM; i++) 
	{
		activeSeq[i] = LEDSEQ_STOP;
		for(j=0; j<SEQ_NUM; j++)
			state[i][j] = LEDSEQ_STOP;
	}
	
	/*创建软件定时器*/
	for(i=0; i<LED_NUM; i++)
		timer[i] = xTimerCreate("ledseqTimer", \
	                          1000, \
	                          pdFALSE, \
	                          (void*)i, \
														runLedseq);
 
	vSemaphoreCreateBinary(ledseqSem);	/*创建一个2值信号量*/

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

/*运行led的sequence序列*/
void ledseqRun(LEDMark_e led, const ledseq_t *sequence)
{
	int prio = getPrio(sequence);	/*获取led优先级序列*/

	if(prio<0) return;

	xSemaphoreTake(ledseqSem, portMAX_DELAY);
	state[led][prio] = 0; 
	updateActive(led);
	xSemaphoreGive(ledseqSem);

	if(activeSeq[led] == prio)	/*当前序列优先级等于活动序列优先级*/
		runLedseq(timer[led]);
}

/*停止led的sequence序列*/
void ledseqStop(LEDMark_e led, const ledseq_t *sequence)
{
	int prio = getPrio(sequence);/*获取led优先级序列*/

	if(prio<0) return;

	xSemaphoreTake(ledseqSem, portMAX_DELAY);
	state[led][prio] = LEDSEQ_STOP;  
	updateActive(led);
	xSemaphoreGive(ledseqSem);

	runLedseq(timer[led]);
}

/*FreeRTOS 定时器回调函数*/
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
			default:  /*LED定时*/
				LedSet(led, step->value);	/*定时step->value*/
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

/*获取led序列优先级*/
static int getPrio(const ledseq_t *seq)
{
	int prio;

	for(prio=0; prio<SEQ_NUM; prio++)
		if(sequences[prio]==seq) return prio;

	return -1; /*无效序列*/
}

/*更新led的最高优先级序列*/
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
/* LED测试 */
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
  * @brief  LED灯初始化
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
  * @brief  LED_State,用于MCU与代码运行情况监控检测
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
		while(cnt++ < 5)	/*初始化通过 绿灯快闪5次*/
		{
			LedFlashOne(LED_GREEN, 50, 50);
		}
	}
	else{
		while(cnt++ < 5)	/*初始化失败 红灯快闪5次*/
		{
			LedFlashOne(LED_RED, 50, 50);
		}
	}
}


/**
  * @brief  关闭所有LED
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
  * @brief  打开所有LED
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
  * @brief  LED闪烁1次
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
  * @brief  设置某个LED的状态
  * @param  [in] led    led标号  LEDMark_e
  * @param  [in] status  ON亮  OFF关
  * @retval None
  */
void LedSet(LEDMark_e led, LEDState_e status)
{
	if (led>LED_NUM+1)
		return;

	GPIO_WriteBit(leds[led].GPIOx,leds[led].GPIO_Pin,(BitAction)status);
}




