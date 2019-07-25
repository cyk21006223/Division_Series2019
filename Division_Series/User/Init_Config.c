#include "Init_Config.h"

/**
  * @brief  ϵͳ����
  * @param  None
  * @retval None
  */
void OS_Start(void)
{
	//������ʼ����
	xTaskCreate((TaskFunction_t )start_task,            
							(const char*    )"start_task",          
							(uint16_t       )START_STK_SIZE,        
							(void*          )NULL,                  
							(UBaseType_t    )START_TASK_PRIO,       
							(TaskHandle_t*  )&StartTask_Handler);              
	vTaskStartScheduler();   
}

/**
  * @brief  �ײ�Ӳ����ʼ��
  * @param  None
  * @retval None
  */
void BSP_Init(void)
{	
  /*�жϳ�ʼ��*/
  NVIC_RTOSInit();

	/*delay��ʼ��*/
	delay_init();
	
	/*�ɿص�Դ��ʼ��*/
	PowerCtrl_Init();
  
	/*led�����г�ʼ��*/
	ledseqInit();
	
	/*���ڵ��Գ�ʼ��*/
	ComAssistant_Init();
	
	/*DR16���ջ���ʼ��*/
	DR16_receiverInit();	
	
	/*RM����ר�õ����ʼ��*/
  RM_MotorInit();
	
	/*IMU��ʼ��*/
  IMU_Init();
	
	/*��������ʼ��*/
//	Ultrasonic_Init();
	
	/*�ٷ�RM2019����ϵͳ��ʼ��*/
	JudgeSystem_Init();	
	
  /*N3_DJI���ճ�ʼ��*/
//  N3_DJI_ASSISITANT_Init();


//	TIM6_CounterMode(179, 9999);//10msѭ������ռ���ȼ�3
		
//	TIM7_CounterMode(179, 9999);//10msѭ����ѭ��ִ��


		/*����ͼ��λ��,������*/
		Laser_Init();
		LASER(LaserON);
		
	/*��ʼ�����*/
	LED_State();

}

