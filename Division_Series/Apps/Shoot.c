/**--------------------------------------------------------------------------*
  * @file   : Shoot.c                                                  *
  * @author : cyk                                                         *
  * @version: v0.0.1                                                         *
  * @date   : 19-January-2019                                                   *
  * @brief  : Shooter Shooting control task file.                                *
  * @refer  : None                                                        *
  *--------------------------------------------------------------------------*/
#include "Shoot.h"

Shooter_t Shooter;
static int num ; //每秒num发
static int ShootFreqMAX=16;			  //最大射频，每秒最多N发弹丸
/**
  * @brief  发射机构初始化
  * @param  None
  * @retval None
  */
void Shoot_Init(void)
{
	/*激光初始化*/
	Laser_Init();
	
	/*摩擦轮初始化*/
	Fric_Init();

	/*射击初始化*/
	Shooter.shootFlag=0;
	Shooter.shootReady=0;
  Shooter.SingleShootFlag=0;
	Shooter.TwoShootFlag=0;
	
	/*拨弹电机参数初始化*/
	Kreload_Init();

	/*双环PID初始化*/
	PositionPID_setPara(&KreloadMotor.pid_angle, 0.3f, 0.0f, 0.4f, 9500, 500);
	PositionPID_setPara(&KreloadMotor.pid_speed, 1.5f, 0.0f, 0.0f, 9800, 1000);

}

/**
  * @brief  发射机构决策
  * @param  None
  * @retval None
  */
void Shoot_deciding(void)
{
	 /*遥控器离线/全部断电/仅发射断电，拨弹和摩擦轮关闭*/
	if(	 DR16.offLineFlag == 1 \
		|| ControlStatus.ControlMode == ControlMode_AllScram)
	{
		/*重启准备*/
		Shooter.ShootInit=ON;
		/*关闭摩擦轮*/
		fric_turnOff();
		/*关闭拨弹电机*/
		Kreload_turnoff();
	}
	
	else{
/*用激光瞄准器识别摩擦轮的射击状态****Begin*/
		if( FrictWorkStatus == FrictShutdown_Status )
		{
      LASER(LaserON);
	    delay_xms(500);
	    LASER(LaserOFF);
	    delay_xms(500);
		}
		if(FrictWorkStatus != FrictShutdown_Status)
		{
      LASER(LaserON);
		}
/*用激光瞄准器识别摩擦轮的射击状态****End*/
		
	 /*收到发射请求、拨弹电机到位*/
	 if( Shooter.shootFlag !=0 && KreloadMotor.loadFinish ==1 )
	 {
		  /*发射未准备就绪，摩擦轮预启动*/
			if(FrictMotor.fricReady != 1)
			{
				fric_PreStart();
			}
		 
		 /*发射准备就绪，摩擦轮启动至最低速度为初始速度后，摩擦轮控制*/
			if(FrictMotor.fricReady == 1)
			{
				FrictWheelControl(FrictWorkStatus);
			}
	 }
	 
	if(!KreloadMotor.offLineFlag)
	{
		/*角度内环PID计算*/
		KreloadMotor.targetSpeed = Position_PID(&KreloadMotor.pid_angle, \
		                                     KreloadMotor.targetAngle, \
		                                     KreloadMotor.totalAngle);
		/*速度外环PID计算*/
		KreloadMotor.outCurrent = Position_PID(&KreloadMotor.pid_speed, \
		                                    KreloadMotor.targetSpeed, \
		                                    KreloadMotor.realSpeed);
		

		/*拨盘判断是否卡弹及反转处理*/
		Kreload_Jam_Handle();

		/*拨弹完毕判断*/
		if(abs(KreloadMotor.pid_angle.err) < 10000)
		{
			KreloadMotor.loadFinish = 1;
			KreloadMotor.blocking = 0;
			KreloadMotor.blockCounter = 0;
		}
		
		/*清零防炸*/
		if(abs(KreloadMotor.turnCount) > 100)
		{
			KreloadMotor.targetAngle -= (8192*KreloadMotor.turnCount);
			KreloadMotor.turnCount = 0;
		}
	}
	else{
		/*关闭摩擦轮*/
		fric_turnOff();
		/*拨弹电机参数初始化*/
		Kreload_Init();
	}
 }
}
 
 /**
  * @brief  拨弹电机-单发模式
  * @param  [in]  dr16_rc_s 遥控器的左右杆
  *         [in]  pos       左右杆的挡位
  * @retval None
  */
void Shoot_single_shot(void)
{
		Shooter.shootFlag=1;
		if(Shooter.shootReady == 0)
		{
			Shooter.shootReady=1;
//反转Bug
//#if Kreload_VEER == 1
//			KreloadMotor.targetAngle += M2006_LOADANGLE*3;
//#endif
//#if Kreload_VEER == 0
//			KreloadMotor.targetAngle -= M2006_LOADANGLE*1;
//#endif
		}
}

/**
  * @brief  拨弹电机-连发模式-摇杆+鼠标控制
  * @param  in		输入值
  * @retval None
  */
void Shoot_repeated_fire(int in)
{
	/*射频设定并限幅*/
if(FrictWorkStatus == FrictNormalSpeed_Status)
{
	ShootFreqMAX = 16;
}
else{
	ShootFreqMAX = 16;
}
	
	if( ControlStatus.ControlMode==ControlMode_ByPC_andRocker )
	{
		num = DR16.thumbwheel/(DR16_ROCKER_MAXVALUE/ShootFreqMAX)+in;
	}
	else if( ControlStatus.ControlMode==ControlMode_ByPC_notRocker )
	{
		num = in;
	}
	VAL_LIMIT(num,0,ShootFreqMAX);

		Shooter.shootFlag=1;
		 if(Shooter.shootReady==0)
	  {	
			static int count=0;
			
		  if(num==0)
			{
				KreloadMotor.targetAngle = KreloadMotor.totalAngle;
			}
		  else if(count++>(Shoot_Freq_Mode(num)))
		  {
#if Kreload_VEER == 1
		 	 KreloadMotor.targetAngle += M2006_LOADANGLE;
#endif
#if Kreload_VEER == 0
		 	 KreloadMotor.targetAngle -= M2006_LOADANGLE;
#endif
			 count=0;
			 Shooter.shootReady=0;
		  }
	  }
	
}


/**
  * @brief  拨弹电机-射频配置
  * @param  Shoot_NumPerSecond   每秒N发
  * @retval None
  */
double Shoot_Freq_Mode(double Shoot_NumPerSecond)
{
	double Shoot_time=((1.0/(double)Shoot_NumPerSecond)*100.0)-1.0;
	return Shoot_time;
}


/**
  * @brief  拨弹电机-卡弹处理
  * @param  None
  * @retval None
  */
void Kreload_Jam_Handle(void)
{	
	/*outCurrent is likely to be random*/
	if(abs(KreloadMotor.outCurrent)>(M2006LockedRotorCurrent+1+1000)/*+1000补偿*/\
		  && abs(KreloadMotor.realSpeed) < 40 \
		  && KreloadMotor.blockCounter++>(Shoot_Freq_Mode(num)) )
	{
		if(getKreloadVeer()==Kreload_VEER)//原方向卡弹
		{
		   KreloadMotor.blocking = 1;
		}
	}	
	else {	
		KreloadMotor.blocking = 0;
	}
	
	/*满足卡弹条件*/
	if(KreloadMotor.blocking!=0)
	{
		/*拨弹电机没到位*/
		KreloadMotor.loadFinish = 0;
		/*卡弹一定时间后恢复原来转向*/
		if(KreloadMotor.blockCounter++>(Shoot_Freq_Mode(1)))
		{
			KreloadMotor.targetAngle = KreloadMotor.totalAngle;
			if(KreloadMotor.RecoverCount>=0)
			{
			  KreloadMotor.RecoverCount++;
			}
		}
		else if(KreloadMotor.blocking==1)
		{
		/*卡弹后反转到原来位置*/
#if Kreload_VEER == 1
		KreloadMotor.targetAngle -= (M2006_LOADANGLE*2);
#endif
#if Kreload_VEER == 0
		KreloadMotor.targetAngle += (M2006_LOADANGLE*2);
#endif
		} 
	}
}
/**
  * @brief  获取拨盘转向
  * @param  None
  * @retval 1逆时针 0 顺时针
  */
bool getKreloadVeer(void)
{
  return KreloadMotor.KreloadVeer;
}
/**
  * @brief  卡弹次数检测拨盘是否故障
  * @param  None
  * @retval None
  */
void RecoverTimeMonitor(void)
{
	/*超过5次则判断拨盘故障*/
  if(KreloadMotor.RecoverCount <= 5 ){
      KreloadMotor.KreloadError = 0;
	}
	else{
      KreloadMotor.KreloadError = 1;
		  /*清零*/
			KreloadMotor.RecoverCount = 0;
	}
}
/**
  * @brief  拨弹电机关闭
  * @param  None
  * @retval None
  */
void Kreload_turnoff(void)
{
	Shooter.shootReady=0;
	Shooter.shootFlag=0;
	KreloadMotor.loadFinish=0;
	KreloadMotor.targetSpeed=KreloadMotor.realSpeed;
	KreloadMotor.outCurrent=0;
}
/**
  * @brief  射速校正
  * @param  None
  * @retval None
  */
void ShootSpeed(void)
{
	if(FrictWorkStatus != FrictShutdown_Status)
	{
		/*强制提速*/
	   if(Get_realShotSpeed()<26.0)
		 {
		   Shooter.SpeedUnqualified=1;
		 }
		 else{
			 Shooter.SpeedUnqualified=0;
		 }
	}
	/*设置射速*/
	if(Shooter.SpeedUnqualified==1)
	{
//	  FrictWheelParam.Maxcount += 1; 
	}
}
/**
  * @brief  拨弹电机参数初始化
  * @param  None
  * @retval None
  */
void Kreload_Init(void)
{
	KreloadMotor.BulletLaunched = 0;
	KreloadMotor.blocking = 0;
	KreloadMotor.blockCounter = 0;
	KreloadMotor.loadFinish = 0;
	KreloadMotor.RecoverCount = 0;
	KreloadMotor.targetAngle=KreloadMotor.totalAngle;
}
