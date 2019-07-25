#include "Fric_Motor.h"

FrictWheelParam_t FrictWheelParam;
/*摩擦轮电机速度限幅1000~2000*/
/*1200 21.8*/
/*1400 23*/
/*1600 26*/
const uint16_t FricSpeed[4] = { 1000, 1200, 1400, 1600 };
/*摩擦轮状态*/
FrictWorkStatus_e FrictWorkStatus;
FrictMotor_t     FrictMotor;

/**
	* @brief  获取摩擦轮状态
  * @param	None
  * @retval FrictWorkStatus 摩擦轮状态
  */
FrictWorkStatus_e GetFrictWorkStatus(void)
{
	return FrictWorkStatus;
}

/**
	* @brief  摩擦轮控制
  * @param	[in] FrictStatus                摩擦轮状态
  *         0 FrictShutdown_Status          摩擦轮关闭状态
  *         1 FrictNormalSpeed_Status       摩擦轮正常速度状态  
  *         2 FrictHighSpeed_Status         摩擦轮高速状态
  *         3 FrictInvincibleSpeed_Status   摩擦轮无敌超速状态
  * @retval None
  */
void FrictWheelControl(FrictWorkStatus_e FrictStatus)
{
	switch(FrictStatus)
	{	
		case FrictNormalSpeed_Status: //正常模式
			FrictWorkStatus = FrictNormalSpeed_Status;
		  FrictWheelParam.RampRate = 1;
			FrictWheelParam.Maxcount = FricSpeed[FrictWorkStatus];
			VALUESET_STEP(FrictWheelParam.Rampcount, \
		                FrictWheelParam.Maxcount, \
		                FrictWheelParam.RampRate);
			break;
		
		case FrictHighSpeed_Status: //高速模式
			FrictWorkStatus = FrictHighSpeed_Status;
		  FrictWheelParam.RampRate = 1;
			FrictWheelParam.Maxcount = FricSpeed[FrictWorkStatus];
			VALUESET_STEP(FrictWheelParam.Rampcount, \
		                FrictWheelParam.Maxcount, \
		                FrictWheelParam.RampRate);
			break;
		
    case FrictInvincibleSpeed_Status://摩擦轮无敌超速状态
			FrictWorkStatus = FrictInvincibleSpeed_Status;
			FrictWheelParam.RampRate = 1;
			FrictWheelParam.Maxcount = FricSpeed[FrictWorkStatus];
			VALUESET_STEP(FrictWheelParam.Rampcount, \
		                FrictWheelParam.Maxcount, \
		                FrictWheelParam.RampRate);
		break;
		
		case FrictShutdown_Status://关闭摩擦轮
			FrictWorkStatus = FrictShutdown_Status;
			FrictWheelParam.RampRate = 1;
			FrictWheelParam.Maxcount = FricSpeed[FrictWorkStatus];
			FrictWheelParam.Rampcount = 1000;
			FrictMotor.fricReady = 0;
		break;
		
		default:
			break;
	}		
	
	if(FrictWheelParam.Rampcount == FrictWheelParam.Maxcount)
	{
		FrictMotor.fricReady = 1;
	}
	
	fricMotor_setSpeed(FrictWheelParam.Rampcount);//摩擦轮速度设置

}

/**
  * @brief  摩擦轮初始化
  * @param  None
  * @retval None
  */
void Fric_Init(void)
{
	/*发射未准备就绪*/
	FrictMotor.fricReady = 0;
	/*摩擦轮停止状态*/
	FrictWorkStatus = FrictShutdown_Status;
	/*摩擦轮等待预启动*/
	FrictMotor.fric_prestart = 0;
	/*摩擦轮保护*/
	FrictMotor.fricPro=0;

#if USE_BOARDTYPE == 1
	//PB7、PH6
	TIM12_PWMOutput(179,19999,1000);//180M/180/20000=50HZ
#endif
#if USE_BOARDTYPE == 0
	TIM8_PWMOutput(179,19999,1000);
#endif

}

/**
  * @brief  1秒摩擦轮预启动
  * @param  None
  * @retval None
  * @notice 摩擦轮启动后，激光启动
  */
void fric_PreStart(void)
{
	/*启动速度*/
	static int fri_count = 0; 
	fricMotor_setSpeed(fri_count);
	
	/*1秒预启动，y=(fri_count/x)*0.01[x=1,y=10]*/
	fri_count += 10;
	
	/*摩擦轮启动至最低速度为初始速度*/
	if(fri_count > FricSpeed[FrictWorkStatus])
	{
		fri_count = FricSpeed[FrictWorkStatus];
		
		FrictWorkStatus = FrictShutdown_Status;
		FrictWheelControl(FrictWorkStatus);
		
		FrictMotor.fricReady = 1;
		FrictMotor.fric_prestart = 1;
	}
	
}


/**
  * @brief  关摩擦轮
  * @param  None
  * @retval None
  * @notice 摩擦轮关闭后，激光关闭
  */
void fric_turnOff(void)
{
	FrictWorkStatus = FrictShutdown_Status;
	fricMotor_setSpeed(FricSpeed[FrictShutdown_Status]);
	FrictMotor.fricReady = 0;
}

/**
  * @brief  设置摩擦轮电机速度
  * @param  speed 	电机速度，范围1000~2000
  * @retval None
  */
void fricMotor_setSpeed(uint16_t speed)
{
	VAL_LIMIT(speed,FRICMOTOR_SPEEDMIN,FRICMOTOR_SPEEDMAX);
	
#if USE_BOARDTYPE == 1
	TIM_SetComparex[0](TIM12, speed);
	TIM_SetComparex[1](TIM12, speed);
#endif
#if USE_BOARDTYPE == 0
	TIM_SetComparex[0](TIM8, speed);
	TIM_SetComparex[1](TIM8, speed);
#endif
}	


