#include "Cloud_control.h"
/******************************************************************************/
//#define USE_CURRENTANGPID    //串级机械角度电流
//#define  USE_5MSCascade     //串级机械角度速度5MS
//#define  USE_5MSIMUCascade    //串级IMU角度速度5MS
#define  USE_10MSIMUCascade  0   //串级IMU角度速度10MS  0 IMU速度；1 M2006速度
#define  USE_10MSYAWIMU      1   //0 Yaw旧配置  1 Yaw新配置2019.5.9

#define USE_oldLIMIT 

Cloud_t Cloud;

float YAWCENTER,PITCHCENTER;
float PITCHTOPPED;
float TempIMUPitchAngleMin,TempIMUPitchAngleMax;
//not test
int deta_Cloud_Yaw;
int deta_Cloud_Pitch;
int Cloud_returningMID[2]={0,0};
/******************************************************************************/
/*2019.5.9 Begin*/
float IMUYAWSpeed;
float IMUYAWSpeed_1;
float IMUYAWSpeed_2;
float IMUYAWTARGETSPEED;

float IMUPITCHSpeed;
float IMUPITCHSpeed_1;
float IMUPITCHSpeed_2;
float IMUPITCHTARGETSPEED;
/*2019.5.9 End*/

/**
  * @brief  云台初始化（带GYIMU）,配置参数
  * @param  None
  * @retval None
  */
void Cloud_withGYIMU_Init(void)
{
/***************************Cloud_Yaw_Motor Begin******************************/
	/*云台偏移灵敏度*/
	Cloud.Sensity[Yaw] = (float)0.02;
#if USE_10MSYAWIMU == 0
	/*Yaw轴角度内环、速度外环*/ 
	PositionPID_setPara(&Cloud_Yaw_Motor.pid_IMUAngle,0.8f, 0.0f, \
                                                       0.0f, 30, 1000);
	IncrementalPID_SetPara(&Cloud_Yaw_Motor.inc_pid_speed,100.0f, 18.0f, \
                                                       0.0f, 8000, 1000);
#endif
#if USE_10MSYAWIMU == 1
	/*Yaw轴角度内环、速度外环*/ 
	PositionPID_setPara(&Cloud_Yaw_Motor.pid_IMUAngle,11.0f, 0.0f, \
                                                       150.0f, 500, 0);
	PositionPID_setPara(&Cloud_Yaw_Motor.pid_IMUSpeed,30.0f, 0.0f, \
                                                       0.0f, 10000, 0);
#endif
	/*Yaw轴位置初始化*/
  Cloud.IMUtargetYaw = Cloud_IMU.totalYaw;
	Cloud.YAWCENTER = Cloud_IMU.totalYaw;
/*****************************Cloud_Yaw_Motor End******************************/
	
/*************************Cloud_Pitch_Motor Begin******************************/
	/*云台偏移灵敏度*/
	Cloud.Sensity[Pitch] = (((float)1)/((float)660))*((float)500);// 1/660*x
#ifdef USE_10MSIMUCascade
	/*Down*/
	#if USE_10MSIMUCascade == 0
//	   /*调试限幅*/
//		Cloud.IMUPitchAngleMax = 36;
//		Cloud.IMUPitchAngleMin = 9;
   #if USE_10MSYAWIMU == 0
		PositionPID_setPara(&Cloud_Pitch_Motor.pid_IMUAngle, -4.0f, -0.0f, -20.0f, 4000, 1000);
		PositionPID_setPara(&Cloud_Pitch_Motor.pid_IMUSpeed, 10.0f, 0.0f, 60.0f, 8000, 0);
	 #endif
	 #if USE_10MSYAWIMU == 1
		PositionPID_setPara(&Cloud_Pitch_Motor.pid_IMUAngle, -5.5f, -0.0f, -90.0f, 4000, 1000);
		PositionPID_setPara(&Cloud_Pitch_Motor.pid_IMUSpeed, 10.0f, 0.0f, 30.0f, 8000, 0);
	 #endif
	#elif USE_10MSIMUCascade == 1
		PositionPID_setPara(&Cloud_Pitch_Motor.pid_IMUAngle, -38.0f, -0.0f, -80.0f, 4000, 0);
		PositionPID_setPara(&Cloud_Pitch_Motor.pid_speed, 15.0f, 0.0f, 0.0f, 8000, 0);
	#endif
#endif
#ifdef USE_5MSIMUCascade
  PositionPID_setPara(&Cloud_Pitch_Motor.pid_IMUAngle, -100.0f, -0.0f, -1000.0f, 4000, 0);
	PositionPID_setPara(&Cloud_Pitch_Motor.pid_speed, 0.6f, 0.0f, 0.4f, 8000, 0);
#endif
	
#ifdef USE_5MSCascade
  PositionPID_setPara(&Cloud_Pitch_Motor.pid_angle, 12.0f, 0.0f, 16.0f, 4000, 0);
	PositionPID_setPara(&Cloud_Pitch_Motor.pid_speed, 0.6f, 0.0f, 0.4f, 8000, 0);
#endif
#ifdef USE_CURRENTANGPID
	PositionPID_setPara(&Cloud_Pitch_Motor.pid_angle,
		                    1.8f, 0.0f, 24.0f,  M2006LockedRotorCurrent, 2500);
	PositionPID_setPara(&Cloud_Pitch_Motor.pid_current,0.7f, 0.3f, 0.0f, \
	                     M2006LockedRotorCurrent, 2500);
#endif

	/*Pitch轴位置初始化*/
  Cloud.IMUtargetPitch = Cloud_IMU.totalPitch;
	Cloud_Pitch_Motor.targetAngle = Cloud_Pitch_Motor.totalAngle;
	Cloud.PITCHCENTER = Cloud_IMU.totalPitch;
	
/*************************Cloud_Pitch_Motor End********************************/
}	

/**
  * @brief      Pitch云台（5ms），双环模式
  * @param[in]  None
  * @retval     None
  */
void Cloud_deciding_5MSGYIMU(void)
{
	static float delta_pitch;
	
	/*遥控器离线/全部急停，云台断电*/
	if(ControlStatus.ControlMode==ControlMode_AllScram || DR16.offLineFlag == 1)
	{
		/*重启准备*/
		Cloud.CloudInit = ON;
		/*离线清零并断电*/
		Cloud_Yaw_Motor.outCurrent=0;
		Cloud_Pitch_Motor.outCurrent=0;
	}
	
	else{
#ifdef USE_5MSIMUCascade
  	delta_pitch  =  Cloud.Control[Pitch];//0.1,0~66
		delta_pitch *=  0.0151515151515152;//(1/66)
		Cloud.IMUtargetPitch -= delta_pitch;
#endif
#ifdef USE_5MSCascade
	#define targetAngleMAX  13620
	#define targetAngleMIN  -13096
		delta_pitch  =  Cloud.Control[Pitch];
		Cloud_Pitch_Motor.targetAngle += delta_pitch;
		/*对遥控的delta_pitch清零*/
		if(Cloud_Pitch_Motor.targetAngle == targetAngleMIN \
			|| Cloud_Pitch_Motor.targetAngle == targetAngleMAX )
		{
		 if(delta_pitch < 0 && Cloud_Pitch_Motor.targetAngle == targetAngleMAX)delta_pitch=0;
		 if(delta_pitch > 0 && Cloud_Pitch_Motor.targetAngle == targetAngleMIN)delta_pitch=0;
		}
		 /*调试限幅*/
		VAL_LIMIT(Cloud_Pitch_Motor.targetAngle, targetAngleMIN, targetAngleMAX);//most down, most up
#endif
		/*Pitch轴双环*/
		if(!Cloud_Pitch_Motor.offLineFlag)
		{
#ifdef USE_5MSIMUCascade
     Cloud_Pitch_Motor.targetSpeed=Position_PID(
		                            &Cloud_Pitch_Motor.pid_IMUAngle, \
			                          Cloud.IMUtargetPitch, \
		                            Cloud_IMU.totalPitch);
     Cloud_Pitch_Motor.outCurrent=Position_PID
	                            (&Cloud_Pitch_Motor.pid_speed, \
			                        Cloud_Pitch_Motor.targetSpeed, \
															Cloud_Pitch_Motor.realSpeed);
#endif
#ifdef USE_5MSCascade
		Cloud_Pitch_Motor.targetSpeed = Position_PID(&Cloud_Pitch_Motor.pid_angle, \
		                                     Cloud_Pitch_Motor.targetAngle, \
		                                     Cloud_Pitch_Motor.totalAngle);
		Cloud_Pitch_Motor.outCurrent = Position_PID(&Cloud_Pitch_Motor.pid_speed, \
		                                    Cloud_Pitch_Motor.targetSpeed, \
		                                    Cloud_Pitch_Motor.realSpeed);
#endif
			/*清标志位*/
	    Cloud_Pitch_Motor.infoUpdateFlag = 0;
		}
		
	}
}

/**
  * @brief      云台决策-10ms（带GYIMU），双环模式
  * @param[in]  None
  * @retval     None
  */
void Cloud_deciding_10MSGYIMU(void)
{
	static float delta_yaw;
	static float delta_pitch;
	
	/*遥控器离线/全部急停，云台断电*/
	if(ControlStatus.ControlMode==ControlMode_AllScram || DR16.offLineFlag == 1)
	{
		/*重启准备*/
		Cloud.CloudInit = ON;
		/*离线清零并断电*/
		Cloud_Yaw_Motor.outCurrent=0;
		Cloud_Pitch_Motor.outCurrent=0;
	}
	
	else{
/***************************Cloud_Yaw_Motor Begin******************************/
	/*Yaw轴移动量*/
	delta_yaw=Cloud.Control[Yaw];
	delta_yaw *= (Pluse_per_Deg_6020);
	Cloud.IMUtargetYaw += delta_yaw;
#ifdef USE_oldLIMIT
	/*设定Yaw轴云台IMU限幅*/
	YAWCENTER=Cloud_getYawAngleToCenter();
	Cloud.IMUYawAngleCenter = Cloud_IMU.totalYaw - YAWCENTER;
//	/*热身赛Yaw视野*/
//	Cloud.IMUYawAngleMin = Cloud.IMUYawAngleCenter - 64;
//	Cloud.IMUYawAngleMax = Cloud.IMUYawAngleCenter + 64;
  
	/*调试2019.5.14*/
	Cloud.IMUYawAngleMin = Cloud.IMUYawAngleCenter - 114;//左，有线，必须限幅
	Cloud.IMUYawAngleMax = Cloud.IMUYawAngleCenter + 114;//右
#endif

	VAL_LIMIT(Cloud.IMUtargetYaw,Cloud.IMUYawAngleMin,Cloud.IMUYawAngleMax);
	
	/*Yaw轴双环*/
	if(!Cloud_Yaw_Motor.offLineFlag)
	{
		/*对遥控的delta_yaw清零*/
		if(Cloud.IMUtargetYaw == Cloud.IMUYawAngleMin \
			|| Cloud.IMUtargetYaw == Cloud.IMUYawAngleMax )
		{
			 if(delta_yaw <0&&Cloud.IMUtargetYaw == Cloud.IMUYawAngleMin)
			 {
				 delta_yaw=0;
			 }
			 if(delta_yaw >0&&Cloud.IMUtargetYaw == Cloud.IMUYawAngleMax)
			 {
				 delta_yaw=0;
			 }
		}
	/*角度PID计算(外环)*/
#if USE_10MSYAWIMU == 0
	Cloud_Yaw_Motor.speedCurrent = Position_PID(&Cloud_Yaw_Motor.pid_IMUAngle, \
                                Cloud.IMUtargetYaw, \
		                            Cloud_IMU.totalYaw);
		
	/*速度PID计算(内环)*/
	Cloud_Yaw_Motor.outCurrent = Incremental_Integral_Separation_PID
	                            (&Cloud_Yaw_Motor.inc_pid_speed, \
		                           0.5f,
			                        Cloud_Yaw_Motor.speedCurrent, \
		                          Cloud_Yaw_Motor.realSpeed);
#endif
#if USE_10MSYAWIMU == 1
	IMUYAWTARGETSPEED = Position_PID(&Cloud_Yaw_Motor.pid_IMUAngle, \
                                Cloud.IMUtargetYaw, \
		                            Cloud_IMU.totalYaw);
		
	/*消除gyro.z*/
	IMUYAWSpeed=Filter_Slide_Mean_Value(Cloud_IMU.gyro.z,6);
	Filter_IIRLPF(&IMUYAWSpeed,&IMUYAWSpeed_1,0.5);
	RoundUp(IMUYAWTARGETSPEED, &Cloud_Yaw_Motor.speedCurrent, 2);
	RoundUp(IMUYAWSpeed_1, &IMUYAWSpeed_2, 2);
		
	Cloud_Yaw_Motor.outCurrent = Position_PID
	                            (&Cloud_Yaw_Motor.pid_IMUSpeed, \
			                        Cloud_Yaw_Motor.speedCurrent, \
		                          IMUYAWSpeed_2);

#endif
	/*清标志位*/
	Cloud_Yaw_Motor.infoUpdateFlag = 0;
	}
/*****************************Cloud_Yaw_Motor End******************************/

/*************************Cloud_Pitch_Motor Begin******************************/
	/*Pitch轴移动量*/
  	delta_pitch  =  Cloud.Control[Pitch];//((1*x)/660)
		delta_pitch *=  (Pluse_per_Deg_2006);//[(1*x)/(660*Pluse_per_Deg_2006)]
		Cloud.IMUtargetPitch -= delta_pitch;

#ifdef USE_oldLIMIT
/*热身赛-第一代云台的Pitch轴限幅处理*/
{
//	/*设定Pitch轴云台IMU限幅*/
//	PITCHCENTER = Cloud_getPitchAngleToCenter();
//  /*Cloud.IMUPitchAngleCenter 云台不碰地, 减30==消去补偿值*/
//	Cloud.IMUPitchAngleCenter = Cloud_IMU.totalPitch-PITCHCENTER-30.0f;
//	if(Cloud.BeCloseToGND==0)
//	{
//	  Cloud.IMUPitchAngleMax = Cloud.IMUPitchAngleCenter + 29.0f;
//	  Cloud.IMUPitchAngleMin = Cloud.IMUPitchAngleCenter - 20.0f;
//	}
//	else if(Cloud.BeCloseToGND==1)
//	{
//		Cloud.IMUPitchAngleMax=Cloud.IMUPitchAngleMin=Cloud.IMUPitchAngleCenter;
//	}
//  VAL_LIMIT(Cloud.IMUtargetPitch,Cloud.IMUPitchAngleMin,Cloud.IMUPitchAngleMax);
}
	 #if USE_10MSYAWIMU == 0
/*调试2019.5.3*/
{
  PITCHTOPPED = Cloud_getPitchAngleTopped();
	/*Cloud.IMUPitchAngleTopped 云台不碰地, 加2.67==消去补偿值*/
	Cloud.IMUPitchAngleTopped = Cloud_IMU.totalPitch+PITCHTOPPED+2.67;
	TempIMUPitchAngleMin = Cloud.IMUPitchAngleTopped;
	TempIMUPitchAngleMax = TempIMUPitchAngleMin + 40.8f;//最大移动俯仰角
	SetPitchLimit(TempIMUPitchAngleMax,TempIMUPitchAngleMin);
	VAL_LIMIT(Cloud.IMUtargetPitch,Cloud.IMUPitchAngleMin,Cloud.IMUPitchAngleMax);
} 
	 #endif


	/*设定Pitch轴云台IMU限幅*/
#endif	
	/*Pitch轴双环*/
	if(!Cloud_Pitch_Motor.offLineFlag)
	{
		/*对遥控的delta_pitch清零*/
		if(Cloud.IMUtargetPitch == Cloud.IMUPitchAngleMin \
			|| Cloud.IMUtargetPitch == Cloud.IMUPitchAngleMax )
		{
			 if(delta_pitch <0&&Cloud.IMUtargetPitch == Cloud.IMUPitchAngleMax)
			 {
				 delta_pitch=0;
			 }
			 if(delta_pitch >0&&Cloud.IMUtargetPitch == Cloud.IMUPitchAngleMin)
			 {
				 delta_pitch=0;
			 }
		}
#ifdef USE_10MSIMUCascade
	#if USE_10MSIMUCascade == 0
//	/*调试Pitch限幅*/
//	SetPitchLimit(39,0);
//	VAL_LIMIT(Cloud.IMUtargetPitch,Cloud.IMUPitchAngleMin,Cloud.IMUPitchAngleMax);
   #if USE_10MSYAWIMU == 0
		 Cloud_Pitch_Motor.targetSpeed=Position_PID(&Cloud_Pitch_Motor.pid_IMUAngle, \
			                          Cloud.IMUtargetPitch, \
		                            Cloud_IMU.totalPitch);
		
     Cloud_Pitch_Motor.outCurrent =Position_PID
	                            (&Cloud_Pitch_Motor.pid_IMUSpeed, \
			                        Cloud_Pitch_Motor.targetSpeed, \
															Cloud_IMU.gyro.y);
   #endif
	 #if USE_10MSYAWIMU == 1
  PITCHTOPPED = Cloud_getPitchAngleTopped();
	/*Cloud.IMUPitchAngleTopped 云台不碰地*/
	Cloud.IMUPitchAngleTopped = Cloud_IMU.totalPitch+PITCHTOPPED;
	Cloud.IMUPitchAngleMin = Cloud.IMUPitchAngleTopped - 10.0f;
	Cloud.IMUPitchAngleMax = Cloud.IMUPitchAngleMin + 103.8f;//最大移动俯仰角
	VAL_LIMIT(Cloud.IMUtargetPitch,Cloud.IMUPitchAngleMin,Cloud.IMUPitchAngleMax);
		
	IMUPITCHTARGETSPEED=Position_PID(&Cloud_Pitch_Motor.pid_IMUAngle, \
			                          Cloud.IMUtargetPitch, \
		                            Cloud_IMU.totalPitch);	
		
	/*消除gyro.y*/
	IMUPITCHSpeed=Filter_Slide_Mean_Value(Cloud_IMU.gyro.y,6);
	Filter_IIRLPF(&IMUPITCHSpeed,&IMUPITCHSpeed_1,0.5);
	RoundUp(IMUPITCHTARGETSPEED, &Cloud_Pitch_Motor.targetSpeed, 2);
	RoundUp(IMUPITCHSpeed_1, &IMUPITCHSpeed_2, 2);
		
  Cloud_Pitch_Motor.outCurrent =Position_PID
	                            (&Cloud_Pitch_Motor.pid_IMUSpeed, \
			                        Cloud_Pitch_Motor.targetSpeed, \
															IMUPITCHSpeed_2);
	 #endif
	#elif USE_10MSIMUCascade == 1
     Cloud_Pitch_Motor.targetSpeed=Position_PID(
		                            &Cloud_Pitch_Motor.pid_IMUAngle, \
			                          Cloud.IMUtargetPitch, \
		                            Cloud_IMU.totalPitch);
     Cloud_Pitch_Motor.outCurrent =Position_PID
	                            (&Cloud_Pitch_Motor.pid_speed, \
			                        Cloud_Pitch_Motor.targetSpeed, \
															Cloud_Pitch_Motor.realSpeed);
	#endif
#endif

	/*清标志位*/
	 Cloud_Pitch_Motor.infoUpdateFlag = 0;
	}
/*************************Cloud_Pitch_Motor End********************************/
 }
}


/**
	* @brief  设定云台电机的目标角度(机械角度)
  * @param[in]  motorName 		电机名字 @ref CloudName_e
  *	@param[in]	angle_current	电流值，范围 0~8191
  * @retval None
  */
void Cloud_setTargetAngle(CloudName_e motorName, float angle_current)
{
	switch ( motorName )
	{
	case Yaw: Cloud_Yaw_Motor.targetAngle = angle_current;
					break;
	case Pitch: Cloud_Pitch_Motor.targetAngle = angle_current;
					break;
	default : break;
	}
}
/**
  * @brief  归位云台\位置归中
  * @param  None
  * @retval None
  */
void Cloud_return_MID(void)
{
	//Yaw
	deta_Cloud_Yaw = Cloud_Yaw_Motor.totalAngle - Cloud_Yaw_Center;
  if( abs(deta_Cloud_Yaw) > (3/Pluse_per_Deg_6020) ){
		Cloud_returningMID[Yaw] = 1;//正在归中,精度1度
	}
  else {
		Cloud_returningMID[Yaw] = 0;
	}
	//Pitch
	deta_Cloud_Pitch = Cloud_Pitch_Motor.totalAngle - Cloud_Pitch_Center;
	if( abs(deta_Cloud_Pitch) > (3/Pluse_per_Deg_2006) ){
		Cloud_returningMID[Pitch] = 1;//正在归中,精度1度
	}
  else {
		Cloud_returningMID[Pitch] = 0;
	}
}
/**
  * @brief  获取云台Yaw旋转角度（取Yaw电机机械角度为中点）
  * @param[in]  None
  * @retval 角度
  */
float Cloud_getYawAngleToCenter(void){
	return (Cloud_Yaw_Motor.totalAngle - Cloud_Yaw_Center)*Pluse_per_Deg_6020;
}
/**
  * @brief  获取云台Pitch旋转角度（取Pitch电机机械角度为中点）
  * @param[in]  None
  * @retval 角度
  */
float Cloud_getPitchAngleToCenter(void){
	return (Cloud_Pitch_Motor.totalAngle - Cloud_Pitch_Center)*Pluse_per_Deg_2006;
}
/**
  * @brief  获取云台Pitch旋转角度（以IMU最高点取Pitch电机机械角度为起点）
  * @param[in]  None
  * @retval 角度
  */
float Cloud_getPitchAngleTopped(void){
	return (Cloud_Pitch_Motor.totalAngle - Cloud_Pitch_UP)*Pluse_per_Deg_2006;
}

/**
  * @brief  设置IMU云台Pitch限幅
  * @param[in]  pitchMAX=IMU最低点 pitchMIN=IMU最高点
  * @retval None
  */
void SetPitchLimit(float pitchMAX, float pitchMIN)
{
	static bool PitchLimitMAX = 0;
	static bool PitchLimitMIN = 0;
	/*PitchLimitMAX*/
	if(PitchLimitMAX == 0){
	 Cloud.IMUPitchAngleMax = pitchMAX;
	}
  if(Cloud_Pitch_Motor.pid_IMUAngle.err>=15
//		&& Cloud_Pitch_Motor.pid_IMUAngle.Target==TempIMUPitchAngleMax)
			&& Cloud_Pitch_Motor.pid_IMUAngle.Target==pitchMAX)
	{
	  PitchLimitMAX = 1;
	}
	if(PitchLimitMAX==1)
	{
		pitchMAX += 22.0;//静差补偿值
	  Cloud.IMUPitchAngleMax = pitchMAX;
	}
	/*PitchLimitMIN*/
	if(PitchLimitMIN == 0){
	 Cloud.IMUPitchAngleMin = pitchMIN;
	}
  if(abs(Cloud_Pitch_Motor.pid_IMUAngle.err)>=6
//		&& Cloud_Pitch_Motor.pid_IMUAngle.Target==TempIMUPitchAngleMin)
			&& Cloud_Pitch_Motor.pid_IMUAngle.Target==pitchMIN)
	{
	  PitchLimitMIN = 1;
	}
	if(PitchLimitMIN==1)
	{
		pitchMIN -= 14.67;//静差补偿值
	  Cloud.IMUPitchAngleMin = pitchMIN;
	}
}


