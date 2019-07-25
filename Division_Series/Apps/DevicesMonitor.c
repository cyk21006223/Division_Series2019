#include "DevicesMonitor.h"

/**
  * @brief  外接设备帧率检测，每100ms检测一次
	* @note   LED_D1  DR16
	*         LED_D2  KreloadMotor
	*         LED_D3  Cloud_Yaw_Motor
	*         LED_D4  Cloud_Pitch_Motor
	*         LED_D5  GY_IMU
	*         LED_D6  NDOF055_IMU
	*         LED_D7  Ultrasonic_Left
	*         LED_D8  Ultrasonic_Right
	*         LED_RED JudgeSystem
  * @param  None
  * @retval None
  */
void DevicesMonitor_update(void)
{
	/*遥控器DR16帧率处理*/
	if(DR16.infoUpdateFrame < 5){
		DR16.offLineFlag = 1;
		LedSet(LED_D1,ON);
		//ledseqStop(LED_D1,seq_DR16);
	}
	else{
		DR16.offLineFlag = 0;
		//ledseqRun(LED_D1,seq_DR16);
		GPIO_ToggleBits(LED_D8_1_GPIO_PORT, LED_D1_PIN);
	}
	DR16.infoUpdateFrame = 0;

	/*拨弹电机帧率处理*//*2500ms后启动*/
	if(KreloadMotor.infoUpdateFrame < 5){
		KreloadMotor.offLineFlag = 1;
		LedSet(LED_D2,ON);
		//ledseqStop(LED_D2,seq_KRELOAD);
	}
	else{
		KreloadMotor.offLineFlag = 0;
		//ledseqRun(LED_D2,seq_KRELOAD);
		GPIO_ToggleBits(LED_D8_1_GPIO_PORT, LED_D2_PIN);
	}
	KreloadMotor.infoUpdateFrame = 0;
	
	/*Yaw电机帧率处理*/
	if(Cloud_Yaw_Motor.infoUpdateFrame < 5){
		Cloud_Yaw_Motor.offLineFlag = 1;
		LedSet(LED_D3,ON);
		//ledseqStop(LED_D3,seq_YAW);
	}
	else{
		Cloud_Yaw_Motor.offLineFlag = 0;
		//ledseqRun(LED_D3,seq_YAW);
		GPIO_ToggleBits(LED_D8_1_GPIO_PORT, LED_D3_PIN);
	}
	Cloud_Yaw_Motor.infoUpdateFrame = 0;
	
	/*Pitch电机帧率处理*/
	if(Cloud_Pitch_Motor.infoUpdateFrame < 5){
		Cloud_Pitch_Motor.offLineFlag = 1;
		LedSet(LED_D4,ON);
		//ledseqStop(LED_D4,seq_PITCH);
	}
	else{
		Cloud_Pitch_Motor.offLineFlag = 0;
		//ledseqRun(LED_D4,seq_PITCH);
		GPIO_ToggleBits(LED_D8_1_GPIO_PORT, LED_D4_PIN);
	}
	Cloud_Pitch_Motor.infoUpdateFrame = 0;
	
	#if USE_IMUBYUSART == 1
	/*云台IMU帧率处理*/
	if(Cloud_IMU.infoUpdateFrame < GY_IMU_FRAMEMIN){
		Cloud_IMU.offLineFlag = 1;
		LedSet(LED_D5,ON);
		//ledseqStop(LED_D5,seq_GY_IMU);
	}
	else{
		Cloud_IMU.offLineFlag = 0;
		//ledseqRun(LED_D5,seq_GY_IMU);
		GPIO_ToggleBits(LED_D8_1_GPIO_PORT, LED_D5_PIN);
	}
	Cloud_IMU.infoUpdateFrame = 0;
#elif USE_IMUBYUSART == 0
		if(JY61Data.infoUpdateFrame < GY_IMU_FRAMEMIN){
		JY61Data.offLineFlag = 1;
		LedSet(LED_D5,ON);
	}
	else{
		JY61Data.offLineFlag = 0;
		GPIO_ToggleBits(LED_D8_1_GPIO_PORT, LED_D5_PIN);
	}
	JY61Data.infoUpdateFrame = 0;
#elif USE_IMUBYUSART == 2
		if(JY901Data.infoUpdateFrame < GY_IMU_FRAMEMIN){
		JY901Data.offLineFlag = 1;
		LedSet(LED_D5,ON);
	}
	else{
		JY901Data.offLineFlag = 0;
		GPIO_ToggleBits(LED_D8_1_GPIO_PORT, LED_D5_PIN);
	}
	JY901Data.infoUpdateFrame = 0;
	#endif
		/*摩擦轮6号灯*/
	if(FrictMotor.onLineFlag==1)
	{
		GPIO_ToggleBits(LED_D8_1_GPIO_PORT, LED_D6_PIN);
	}
	else{
		LedSet(LED_D6,ON);
	}

//	/*NDOF055_IMU帧率处理*/
//	if(NDOF055_data.infoUpdateFrame < GY_IMU_FRAMEMIN){
//		NDOF055_data.offLineFlag = 1;
//		LedSet(LED_D6,ON);
//		//ledseqStop(LED_D6,seq_NDOF_IMU);
//	}
//	else{
//		NDOF055_data.offLineFlag = 0;
//		//ledseqRun(LED_D6,seq_NDOF_IMU);
//		GPIO_ToggleBits(LED_D8_1_GPIO_PORT, LED_D6_PIN);
//	}
//	NDOF055_data.infoUpdateFrame = 0;
//	
//	/*超声波帧率处理*/
//	if(Ultrasonic.infoUpdateFrame < Ultrasonic_FRAMEMIN){
//		Ultrasonic.offLineFlag = 1;
//		LedSet(LED_D7,ON);
//		//ledseqStop(LED_D7,seq_Ultrasonic_L);
//	}
//	else{
//		Ultrasonic.offLineFlag = 0;
//		//ledseqRun(LED_D7,seq_Ultrasonic_L);
//		GPIO_ToggleBits(LED_D8_1_GPIO_PORT, LED_D7_PIN);
//	}
//	Ultrasonic.infoUpdateFrame = 0;
//	
	/*裁判系统帧率处理*//*3180ms后启动*/
	if(JudgeMonitor.infoUpdateFrame < JudgeSystem_FRAMEMIN){
		JudgeMonitor.offLineFlag = 1;
		LedSet(LED_RED,ON);
		//ledseqStop(LED_RED,seq_JudgeSystem);
	}
	else{
		JudgeMonitor.offLineFlag = 0;
		//ledseqRun(LED_RED,seq_JudgeSystem);
		GPIO_ToggleBits(LED_RED_GPIO_PORT, LED_RED_PIN);
	}
	JudgeMonitor.infoUpdateFrame = 0;
}

