#include "UserTask.h"
#include "Init_Config.h"
#include <stdio.h>
/******************************************************************************/
/*任务*/
TaskHandle_t StartTask_Handler;
TaskHandle_t XCOM_Task_Handler;
TaskHandle_t Shoot_deciding_Handler;
TaskHandle_t GetHeight_Task_Handler;
TaskHandle_t Cloud_deciding_Handler;
TaskHandle_t Cloud_executing_Handler;
TaskHandle_t Control_BySwitch_Handler;
TaskHandle_t Control_ByMouse_Handler;
TaskHandle_t DevicesMonitor_Handler;
TaskHandle_t JudgeSystem_Send_Handler;
/*任务标志*/
BaseType_t   CheckTask_GetHeight;
/*控制模式*/
ControlStatus_t ControlStatus;
/*电调启动成功标志*/
int StartSuccessflag=0;

/******************************************************************************/
/**
  * @brief  开始任务，所有的任务创建
  * @param  None
  * @retval None
  */
void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();
	
	/*外设初始化*/
	BSP_Init();
	
	/*发射机构不启动初始化*/
	Shooter.ShootInit = ON;
	/*云台不启动初始化*/
	Cloud.CloudInit = ON;

	/*任务*/
//	xTaskCreate((TaskFunction_t )XCOM_SEND,             
//							(const char*    )"XCOM_SEND",           
//							(uint16_t       )XCOM_SIZE,        
//							(void*          )NULL,                  
//							(UBaseType_t    )XCOM_PRIO,        
//							(TaskHandle_t*  )&XCOM_Task_Handler);    			

	xTaskCreate((TaskFunction_t )Shoot_deciding_Task,            
						(const char*    )"Shoot_deciding_Task",           
						(uint16_t       )Shoot_deciding_Task_SIZE,        
						(void*          )NULL,                  
						(UBaseType_t    )Shoot_deciding_Task_PRIO,        
						(TaskHandle_t*  )&Shoot_deciding_Handler); 
				
//	CheckTask_GetHeight = xTaskCreate((TaskFunction_t )GetHeight_Task,            
//						(const char*    )"GetHeight_Task",           
//						(uint16_t       )GetHeight_Task_SIZE,        
//						(void*          )NULL,                  
//						(UBaseType_t    )GetHeight_Task_PRIO,        
//						(TaskHandle_t*  )&GetHeight_Task_Handler);
				
	xTaskCreate((TaskFunction_t )Cloud_deciding_Task,            
						(const char*    )"Cloud_deciding_Task",           
						(uint16_t       )Cloud_deciding_SIZE,        
						(void*          )NULL,                  
						(UBaseType_t    )Cloud_deciding_PRIO,        
						(TaskHandle_t*  )&Cloud_deciding_Handler);
														
	xTaskCreate((TaskFunction_t )Cloud_executing_Task,            
						(const char*    )"Cloud_executing_Task",           
						(uint16_t       )Cloud_executing_SIZE,        
						(void*          )NULL,                  
						(UBaseType_t    )Cloud_executing_PRIO,        
						(TaskHandle_t*  )&Cloud_executing_Handler);
			
	xTaskCreate((TaskFunction_t )Control_BySwitch_Task,            
						(const char*    )"Control_BySwitch_Task",           
						(uint16_t       )Control_BySwitch_SIZE,        
						(void*          )NULL,                  
						(UBaseType_t    )Control_BySwitch_PRIO,        
						(TaskHandle_t*  )&Control_BySwitch_Handler);
												
	xTaskCreate((TaskFunction_t )Control_ByMouse_Task,            
						(const char*    )"Control_ByMouse_Task",           
						(uint16_t       )Control_ByMouse_SIZE,        
						(void*          )NULL,                  
						(UBaseType_t    )Control_ByMouse_PRIO,        
						(TaskHandle_t*  )&Control_ByMouse_Handler);
													
	xTaskCreate((TaskFunction_t )DevicesMonitor_Task,            
						(const char*    )"DevicesMonitor_Task",           
						(uint16_t       )DevicesMonitor_SIZE,        
						(void*          )NULL,                  
						(UBaseType_t    )DevicesMonitor_PRIO,        
						(TaskHandle_t*  )&DevicesMonitor_Handler);
		
	xTaskCreate((TaskFunction_t )JudgeSystem_Send,             
							(const char*    )"JudgeSystem_Send",           
							(uint16_t       )JudgeSystem_Send_SIZE,        
							(void*          )NULL,                  
							(UBaseType_t    )JudgeSystem_Send_PRIO,        
							(TaskHandle_t*  )&JudgeSystem_Send_Handler);  
							
	vTaskDelete(StartTask_Handler); 
	StartTask_Handler=NULL;
	taskEXIT_CRITICAL();
}

/**
  * @brief  发射决策任务
  * @param  None
  * @retval None
  */
void Shoot_deciding_Task(void *pvParameters)
{
	
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = pdMS_TO_TICKS(10);
  xLastWakeTime = xTaskGetTickCount();
	
	while(1)
	{
		
		/*发射机构初始化*/
		if(Shooter.ShootInit==ON \
			&& KreloadMotor.infoUpdateFlag!=0 \
		  /*&& JudgeMonitor.offLineFlag==0*/)
		{
			taskENTER_CRITICAL();
			Shoot_Init();
//      Shooter.SingleShootFlag=0;//test
			taskEXIT_CRITICAL();
			
			Shooter.ShootInit=OFF;
		}

		if(Shooter.ShootInit==OFF)
		{
			Shoot_deciding();//校准油门
		}

					
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

/**
  * @brief  UAV高度获取任务
  * @param  None
  * @retval None
  */
void GetHeight_Task(void *pvParameters)
{	
	TickType_t xLastWakeTime;
  const TickType_t xFrequency = pdMS_TO_TICKS(10);
  xLastWakeTime = xTaskGetTickCount();
	
	while(1)
	{
#if USE_ULTRASONIC == 0
		/*获取距离*/
		Ultrasonic_getDistance();
#endif
		/*云台保护*/
//		if(Ultrasonic.length<20)
//		{
//			Cloud.BeCloseToGND = 1;
//		}
		
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
/**
  * @brief  云台决策任务
  * @param  None
  * @retval None
  */
//#define  USE_5MSCascade
bool test=0;
void Cloud_deciding_Task(void *pvParameters)
{
	TickType_t xLastWakeTime;
  const TickType_t xFrequency = pdMS_TO_TICKS(10);
  xLastWakeTime = xTaskGetTickCount();
	
	while(1)
	{
		/*云台初始化*/
		if(Cloud.CloudInit==ON&&Cloud_IMU.devStatus!=0)
		{
			taskENTER_CRITICAL();
			#if USE_IMUBYUSART == 1
			Cloud_withGYIMU_Init();
			#elif USE_IMUBYUSART == 2
			
			#endif
			taskEXIT_CRITICAL();
			
			Cloud.CloudInit=OFF;
		}	
		if(Cloud.CloudInit==OFF)
		{
			/*云台控制入口*/
			if(ControlStatus.ControlMode==ControlMode_ByPC_andRocker)
			{
				Cloud.Control[Yaw]=dr16_rocker_R.x*Cloud.Sensity[Yaw]+DR16.mouse.x;
				Cloud.Control[Pitch]=dr16_rocker_R.y*Cloud.Sensity[Pitch]-DR16.mouse.y*26;
			}
			if(ControlStatus.ControlMode==ControlMode_ByPC_notRocker)
			{
				Cloud.Control[Yaw]=DR16.mouse.x;
				Cloud.Control[Pitch]=-DR16.mouse.y*26;
			}
			/*云台进程*/
#if (!defined USE_5MSCascade) && (!defined USE_5MSIMUCascade)
			#if USE_IMUBYUSART == 1
			  Cloud_deciding_10MSGYIMU();
			#elif USE_IMUBYUSART == 2
			
			#endif
		}
#else
			test = 1;
		}else{
		 test=0;
		}
#endif
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
/**
  * @brief  云台执行任务
  * @param  None
  * @retval None
  */
void Cloud_executing_Task(void *pvParameters)
{
	TickType_t xLastWakeTime;
	
	#if (!defined USE_5MSCascade) && (!defined USE_5MSIMUCascade)
	 const TickType_t xFrequency = pdMS_TO_TICKS(10);
   xLastWakeTime = xTaskGetTickCount();
	#else
  const TickType_t xFrequency = pdMS_TO_TICKS(5);
  xLastWakeTime = xTaskGetTickCount();
	#endif
	
	while(1)
	{

		if( FrictWorkStatus == FrictShutdown_Status
			/*|| KreloadMotor.KreloadError==1*/ )
		{
/*确保摩擦轮开启后，才能拨弹*/
			KreloadMotor.outCurrent=0;
		}

		Aerial_setCurrent(Cloud_Yaw_Motor.outCurrent, \
											Cloud_Pitch_Motor.outCurrent, \
											KreloadMotor.outCurrent);
//				Aerial_setCurrent(0, Cloud_Pitch_Motor.outCurrent,0);
		#if (defined USE_5MSCascade) || (defined USE_5MSIMUCascade)
		 if(test==1)
			#if USE_IMUBYUSART == 1
				Cloud_deciding_5MSGYIMU();
			#elif USE_IMUBYUSART == 2
			
			#endif
		#endif

		
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

/**
  * @brief   遥控器摇杆控制任务
  * @param   None
  * @retval	 None
  */
void Control_BySwitch_Task(void *pvParameters)
{
	TickType_t xLastWakeTime;
  const TickType_t xFrequency = pdMS_TO_TICKS(10);
  xLastWakeTime = xTaskGetTickCount();
	
	while(1)
	{
		if( DR16.rc.s_left == DR16_SWITCH_UP )
		{
			switch (DR16.rc.s_right)
			{
				case DR16_SWITCH_UP :  /*左上右上，云台和发射机构都急停*/
														   ControlStatus.ControlMode=ControlMode_AllScram;
															 break;
				case DR16_SWITCH_MID:  /*左上右中，仅发射机构断电不重启*/
															 fric_turnOff();
				                       Kreload_turnoff();
															 break;
				default:break;
			}
		}
		if( DR16.rc.s_left == DR16_SWITCH_MID )
		{
			/*电脑和摇杆都控制*/
			ControlStatus.ControlMode=ControlMode_ByPC_andRocker;
			
			switch (DR16.rc.s_right)
			{
			case DR16_SWITCH_UP  :/*左中右上，连发*/
													if(!GetKeyMouseAction(MOUSE_RIGHT,Press))//右键未按下
													{
														/*左键未按下不打*/
														if(GetKeyMouseAction(MOUSE_LEFT,Press)==0)
														{
														  Shoot_repeated_fire(0);
														}
														/*左键单点二连发*/
														if(GetKeyMouseAction(MOUSE_LEFT,Click_Press)==1)
														{
															Shooter.shootReady = 0;
															Shooter.TwoShootFlag=0;
			                      
                              if(Shooter.TwoShootFlag==0)
															{
															  Shooter.TwoShootFlag=1;
																Shoot_repeated_fire(2);
															}  														  
														}
														/*左键长按射频叠加到上限*/
														if(GetKeyMouseAction(MOUSE_LEFT,Long_Press)==1)
														{
														  Shoot_repeated_fire(KeyMousePressTime[16]/4);  
														}
													}
													else if(GetKeyMouseAction(MOUSE_LEFT,Press)
																&&Shooter.SingleShootFlag==0)//左右键同时按下
													{
														Shooter.SingleShootFlag=1;
														Shoot_single_shot();
													}
      if(dr16_rocker_L.y > 400 && KeyPRO() )//y最上，//关闭摩擦轮
			{
			  FrictWorkStatus = FrictShutdown_Status;
				FrictMotor.fricPro = 0;
			}
			if(dr16_rocker_L.x > 400 && KeyPRO() ) //x最右，//正常模式
			{
				FrictMotor.fricPro = 1;
			  FrictWorkStatus = FrictNormalSpeed_Status;
			}
      if(dr16_rocker_L.y < -400 && KeyPRO() && FrictMotor.fricPro)//y下高速模式
			{
			  FrictWorkStatus = FrictHighSpeed_Status;
			}
			if(dr16_rocker_L.x < -400 && KeyPRO() && FrictMotor.fricPro)//x左无敌超速状态
			{
			  FrictWorkStatus = FrictInvincibleSpeed_Status;
			}
														break;
			case DR16_SWITCH_MID :/*左中右中，仅拨弹电机关闭*/
														Kreload_turnoff();
														break;
			case DR16_SWITCH_DOWN:/*左中右下，单发*/
														Shoot_single_shot();
														break;
			default:break;
			}
		}
		if( DR16.rc.s_left == DR16_SWITCH_DOWN )
		{

			//遥控器校准摩擦轮油门
			switch (DR16.rc.s_right)
			{
			case DR16_SWITCH_UP: 
//				                  fricMotor_setSpeed(1000);
													break;
			case DR16_SWITCH_MID: 
//			                    fricMotor_setSpeed(2000);
				                  /*JudgeMonitor.SendObject = 0; */
													break;
			case DR16_SWITCH_DOWN: /*JudgeMonitor.SendObject = 1;
			                       RobotInteraction_Data.sign = 1;*/
													break;
			default:break;
			}
		}	
	
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
/**
  * @brief   键鼠控制任务
  * @param   None
  * @retval	 None
  */
void Control_ByMouse_Task(void *pvParameters)
{
	TickType_t xLastWakeTime;
  const TickType_t xFrequency = pdMS_TO_TICKS(10);
  xLastWakeTime = xTaskGetTickCount();
	
	while(1)
	{
		if(!GetKeyMouseAction(MOUSE_LEFT,Press))
		{
			if(GetKeyMouseAction(MOUSE_RIGHT,Press)
		  &&DR16.rc.s_right==DR16_SWITCH_UP)
		  {
		    Shooter.shootReady = 0;
		  }
			Shooter.SingleShootFlag=0;
		}
		
{
//		//告知哨兵【碉堡】有敌人【1】
//		if( GetKeyMouseAction(KEY_W,Press) && KeyPRO() )
//		{
//		  JudgeMonitor.SendObject = 1;
//			RobotInteraction_Data.sign = 1;
//		}
//		//告知哨兵【桥头】有敌人【2】
//		if( GetKeyMouseAction(KEY_S,Press) && KeyPRO() )
//		{
//		  JudgeMonitor.SendObject = 1;
//			RobotInteraction_Data.sign = 2;
//		}
//		//告知哨兵【哨兵轨道】旁边的【公路】有敌人【3】
//		if( GetKeyMouseAction(KEY_A,Press) && KeyPRO() )
//		{
//		  JudgeMonitor.SendObject = 1;
//      RobotInteraction_Data.sign = 3;
//		}
//		//告知哨兵【基地】有敌人【4】
//		if( GetKeyMouseAction(KEY_D,Press) && KeyPRO() )
//		{
//		  JudgeMonitor.SendObject = 1;
//			RobotInteraction_Data.sign = 4;
//		}
//		//告知哨兵【野区】有敌人【5】
//		if( GetKeyMouseAction(KEY_F,Press) && KeyPRO() )
//		{
//		  JudgeMonitor.SendObject = 1;
//			RobotInteraction_Data.sign = 5;
//		}
//		if( GetKeyMouseAction(KEY_X,Press) )
//		{
//			//关摩擦轮
//		  FrictWorkStatus = FrictShutdown_Status;
//		}
}
		if( GetKeyMouseAction(KEY_C,Press) && KeyPRO() )
		{
			//摩擦轮保护
			FrictMotor.fricPro = 1;
			//常速
		  FrictWorkStatus = FrictNormalSpeed_Status;
		}
		if( GetKeyMouseAction(KEY_V,Press) && FrictMotor.fricPro && KeyPRO() )
		{
			//高速
		  FrictWorkStatus = FrictHighSpeed_Status;
		}
		if( GetKeyMouseAction(KEY_B,Press) && FrictMotor.fricPro && KeyPRO() )
		{
			//超速
		  FrictWorkStatus = FrictInvincibleSpeed_Status;
		}
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}


/**
  * @brief  串口输出任务
  * @param  None
  * @retval None
  */
extern float IMUYAWSpeed_2;
extern float IMUPITCHSpeed_2;
void XCOM_SEND(void *pvParameters)
{	
	TickType_t xLastWakeTime;
#if USE_BLUETOOTH == 1 
  const TickType_t xFrequency = pdMS_TO_TICKS(200);
#else
  const TickType_t xFrequency = pdMS_TO_TICKS(10);
#endif
  xLastWakeTime = xTaskGetTickCount();
	
	while(1)
	{
//		if(Get_realShotSpeed()!=0)
//		printf("V=%0.2f...\r\n",Get_realShotSpeed());
//		
//		if((Cloud_IMU.totalYaw-Cloud.YAWCENTER)!=0)
//		printf("dYaw=%0.2f...\r\n",(Cloud_IMU.totalYaw-Cloud.YAWCENTER));
//		
//		if((Cloud_IMU.totalPitch-Cloud.PITCHCENTER)!=0)
//		printf("dPit=%0.2f...\r\n",(Cloud_IMU.totalPitch-Cloud.PITCHCENTER));
//		
//		if((500-JudgeMonitor.BulletLaunched)!=0)
//		printf("load=%d...\r\n",(500-JudgeMonitor.BulletLaunched));
//		
//		if((KeyMousePressTime[16])!=0)
//		printf("left=%d...\r\n",(KeyMousePressTime[16]));
		
//	ANO_Send_Data_V1(N3_Semaphore.captureValue*100, N3_Semaphore.Value*100, 0, 0);

#if USE_BLUETOOTH == 1 		
//		if(StartSuccessflag==1)
//		{
//		printf("roll=%0.2f pit=%0.2f ",Cloud_IMU.eular.pitch, 
//		                                  Cloud_IMU.totalPitch);
//		printf("YawT=%0.2f YawR=%0.2f ",Cloud_Yaw_Motor.pid_IMUAngle.Target, 
//		                                  Cloud_Yaw_Motor.pid_IMUAngle.Measured);
//		printf("PitT=%0.2f PitR=%0.2f\r\n",Cloud_Pitch_Motor.pid_IMUAngle.Target, 
//		                                  Cloud_Pitch_Motor.pid_IMUAngle.Measured);
//		}
#endif
		/*打印剩余堆栈大小*/
//		printf("FreeHeap: %d bytes\r\n", xPortGetFreeHeapSize());

		
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
/**
  * @brief  裁判系统发送数据任务
  * @param  None
  * @retval None
  */
extern uint8_t Robottest[RobotInteractionLength];
void JudgeSystem_Send(void *pvParameters)
{	
	TickType_t xLastWakeTime;
  const TickType_t xFrequency = pdMS_TO_TICKS(200);
  xLastWakeTime = xTaskGetTickCount();

	float YawMovement;
	float PitchMovement;
	float Mode;
	
	while(1)
	{
		if(StartSuccessflag==1
			&& JudgeMonitor.SendObject==0)
		{		
			/*数据发送前先处理一下再上传*/
			RoundUp((Cloud_IMU.totalYaw-Cloud.YAWCENTER), &YawMovement, 2);
			RoundUp((Cloud_IMU.totalPitch-Cloud.PITCHCENTER), &PitchMovement, 2);
//			Mode = FlagMode(N3_Semaphore_getValue());/*0默认战术 1战术 2战术*/
			
		/*发送到客户端*/
				for(int j=0; j<3; j++)
				{
				 /*Yaw移动量*//*左负右正*/
				 setClient_showData(YawMovement, 1);
				 /*Pitch移动量*//*上负下正*/
				 setClient_showData(PitchMovement,2);
				 /*战术模式*/
				 setClient_showData(Mode, 3);
				}
		/*界面亮灯--Begin*/			
				/*DR16 1号灯*/
				if( DR16.offLineFlag == 0 )
				{
					setClient_pilotLamp(1,1);
				}
				else{
					setClient_pilotLamp(1,0);
				}
				
				/*拨盘 2号灯*/
				if( KreloadMotor.infoUpdateFlag==1 \
					 && Shooter.ShootInit==OFF )
				{
					setClient_pilotLamp(2,1);
				}
				else{
					setClient_pilotLamp(2,0);
				}
				
				/*Yaw 3号灯*/
				if( Cloud_Yaw_Motor.offLineFlag == 0 )
				{
					setClient_pilotLamp(3,1);
				}
				else{
					setClient_pilotLamp(3,0);
				}
				
				/*Pitch 4号灯*/
				if( Cloud_Pitch_Motor.offLineFlag == 0 )
				{
					setClient_pilotLamp(4,1);
				}
				else{
					setClient_pilotLamp(4,0);
				}
				
				/*IMU 5号灯*/
				if( Cloud_IMU.offLineFlag == 0 )
				{
					setClient_pilotLamp(5,1);
				}
				else{
					setClient_pilotLamp(5,0);
				}
				/*摩擦轮6号灯*/
				if(FrictMotor.onLineFlag==1)
				{
					setClient_pilotLamp(6,1);
				}
				else{
					setClient_pilotLamp(6,0);
				}
		/*界面亮灯--End*/
	#if 	GAME_ROBOT == 0
    JudgeClientCustom_SendData(client_custom_data, \
		                           robot_id.Red_Aerial, \
		                           client_id.Red_Aerial);
  #elif GAME_ROBOT == 1
		JudgeClientCustom_SendData(client_custom_data, \
		                           robot_id.Blue_Aerial, \
		                           client_id.Blue_Aerial);		
	#endif
		}
		

   /*测试所发的数据包*/
		{
//		int k;
//		for(k=0; k<RobotInteractionLength-1; k++)
//		{
//		  printf("0x%x ",Robottest[k]);
//		} printf("0x%x\r\n",Robottest[RobotInteractionLength-1]);
		}
		if( StartSuccessflag==1
			&& JudgeMonitor.SendObject==1)
		{
//			/*发送到哨兵*/
//   #if 	GAME_ROBOT == 0	
//			JudgeRobotInteraction_SendData(RobotInteraction_Data, \
//																		 robot_id.Red_Aerial, \
//																		 robot_id.Red_Sentry);
////			//发送给无人机
////						JudgeRobotInteraction_SendData(RobotInteraction_Data, \
////																		 robot_id.Red_Aerial, \
////																		 robot_id.Red_Aerial);
//   #elif GAME_ROBOT == 1  
//      JudgeRobotInteraction_SendData(RobotInteraction_Data, \
//																		 robot_id.Blue_Aerial, \
//																		 robot_id.Blue_Sentry);
//	 #endif
			JudgeMonitor.SendObject=0;
		}

		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
/**
  * @brief  设备检测任务
  * @param  None
  * @retval None
  */
void DevicesMonitor_Task(void *pvParameters)
{
	/*设备在线计时*/
	int DeviceSurvivalCount=0;
	/*Pitch电调启动完成时间2.5s，+0.3补偿*/
	int StartSuccessTime=28;
	/*重启准备标志*/
	int RestartReadyflag[2]={0,0};
	/*摩擦轮电调启动完成时间*/
	int fricStartSuccessTime=0;
	/*离线计时*/
	int offlineFlag[2]={0,0};
	/*重启计时*/
	int RestartCount[2]={0,0};
	
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = pdMS_TO_TICKS(10);
  xLastWakeTime = xTaskGetTickCount();
	
	while(1)
	{
		
		DeviceSurvivalCount++;

		if(DeviceSurvivalCount==10)
		{
			DeviceSurvivalCount=0;
		  /*帧率检测*/
      DevicesMonitor_update();
			/*拨盘检测*/
			RecoverTimeMonitor();
			
			StartSuccessTime--;
		}
		
		/*摩擦轮检测*/
		if(Get_shooterOutput()==1 \
			&& FrictMotor.fric_prestart==1 \
		  && fricStartSuccessTime++>500)//test摩擦轮电调启动时间4.80s
		{
		  FrictMotor.onLineFlag=1;
		}
		else{
			FrictMotor.onLineFlag=0;
		}
		
		if(StartSuccessTime==0)
		{
			StartSuccessflag=1;
			StartSuccessTime=28;
		}
		if(StartSuccessflag==1)
		{
			/*第一次启动，假定拨盘正常*/
			if(KreloadMotor.KreloadError!=1){
				KreloadMotor.KreloadError = 0;
			}
    }
			/*Yaw电机离线断电*/
			if(Cloud_Yaw_Motor.offLineFlag)
			{
				if(offlineFlag[Yaw]++>100 \
					&& RestartReadyflag[Yaw]==0)//离线1秒后
				{
					PowerCtrlSet(PowerCtrl_1,Close);
					RestartReadyflag[Yaw]=1;
				}
			}
			/*Pitch电机离线断电*/
			if(Cloud_Pitch_Motor.offLineFlag)
			{
				if(offlineFlag[Pitch]++>100 \
					&& RestartReadyflag[Pitch]==0)//离线1秒后
				{
					PowerCtrlSet(PowerCtrl_2,Close);
					RestartReadyflag[Pitch]=1;
				}
			}
			/*超声波离线删任务*//*分区赛没用到超声波模块*/
//			if(Ultrasonic.offLineFlag[Left]==1)
//			{
//				if(pdPASS == CheckTask_GetHeight)
//			    vTaskDelete(GetHeight_Task_Handler);
//			}
		}
	
		if(RestartReadyflag[Yaw]==1 \
			 &&Cloud.CloudInit == ON)
		{
			/*Yaw电机离线后手动重启*/
			PowerCtrlSet(PowerCtrl_1,Open);//右下
			/*重启2.8秒后清除标志*//*修复离线管理的BUG*/
			if(RestartCount[Yaw]++>280){
				RestartReadyflag[Yaw]=0;
			}
		}
		
		if(RestartReadyflag[Pitch]==1 \
			 &&Cloud.CloudInit == ON)
		{
			/*Pitch电机离线后手动重启*/
			PowerCtrlSet(PowerCtrl_2,Open);//左上
			/*重启2.8秒后清除标志*//*修复离线管理的BUG*/
			if(RestartCount[Pitch]++>280){
				RestartReadyflag[Pitch]=0;
			}
		}

		
		/*键鼠检测*/
		KeyMouseActionMonitor();
		
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	
}

