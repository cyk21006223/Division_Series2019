#include "Init_Config.h"

/**
  * @brief  系统启动
  * @param  None
  * @retval None
  */
void OS_Start(void)
{
	//创建开始任务
	xTaskCreate((TaskFunction_t )start_task,            
							(const char*    )"start_task",          
							(uint16_t       )START_STK_SIZE,        
							(void*          )NULL,                  
							(UBaseType_t    )START_TASK_PRIO,       
							(TaskHandle_t*  )&StartTask_Handler);              
	vTaskStartScheduler();   
}

/**
  * @brief  底层硬件初始化
  * @param  None
  * @retval None
  */
void BSP_Init(void)
{	
  /*中断初始化*/
  NVIC_RTOSInit();

	/*delay初始化*/
	delay_init();
	
	/*可控电源初始化*/
	PowerCtrl_Init();
  
	/*led灯序列初始化*/
	ledseqInit();
	
	/*串口调试初始化*/
	ComAssistant_Init();
	
	/*DR16接收机初始化*/
	DR16_receiverInit();	
	
	/*RM比赛专用电机初始化*/
  RM_MotorInit();
	
	/*IMU初始化*/
  IMU_Init();
	
	/*超声波初始化*/
//	Ultrasonic_Init();
	
	/*官方RM2019裁判系统初始化*/
	JudgeSystem_Init();	
	
  /*N3_DJI接收初始化*/
//  N3_DJI_ASSISITANT_Init();


//	TIM6_CounterMode(179, 9999);//10ms循环，抢占优先级3
		
//	TIM7_CounterMode(179, 9999);//10ms循环，循环执行


		/*测试图传位置,开激光*/
		Laser_Init();
		LASER(LaserON);
		
	/*初始化完毕*/
	LED_State();

}

