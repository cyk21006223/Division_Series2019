#include "delay.h"


static u32 fac_us=0;							//us延时倍乘数
static u16 fac_ms=0;				      //ms延时倍乘数,在os下,代表每个节拍的ms数

/**
  * @brief  初始化延迟函数
  * @param  SYSCLK:系统时钟频率
  * @retval None
  */
void delay_init(void)
{
	u32 reload;					                //每秒钟的计数次数 单位为K	
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//SysTick频率为HCLK
	
	fac_us=SystemCoreClock/1000000;			//不论是否使用OS,fac_us都需要使用 
	
	reload=SystemCoreClock/1000000;
	reload*=1000000/configTICK_RATE_HZ;		//根据configTICK_RATE_HZ设定溢出时间
	fac_ms=1000/configTICK_RATE_HZ;			//代表OS可以延时的最少单位
	
  SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//开启SYSTICK中断
	SysTick->LOAD=reload; 					//每1/configTICK_RATE_HZ断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //开启SYSTICK
}


/**
  * @brief  微秒级延时
  * @param  _us 延时微秒数
  * @retval None
  */
void delay_us(u32 _us)
{
	u32 ticks=_us*fac_us;						//需要的节拍数
	u32 told=SysTick->VAL;        	//刚进入时的计数器值
	u32 reload=SysTick->LOAD;				//装载值
	u32 tnow=SysTick->VAL;					//当前计数数值
	u32 tcnt=0;
	while(1)
	{
	  tnow=SysTick->VAL;	          //当前计数数值
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		} 
	};
}

/**
  * @brief  毫秒级延时,会引起任务调度
  * @param  _ms 延时毫秒数
  * @retval None
  */
void delay_ms(u32 _ms)
{	
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
	{		
		if(_ms>=fac_ms)						//延时的时间大于OS的最少时间周期 
		{ 
   			vTaskDelay(_ms/fac_ms);	 		//FreeRTOS延时
		}
		_ms%=fac_ms;						//OS已经无法提供这么小的延时了,采用普通方式延时    
	}
	delay_us((u32)(_ms*1000));				//普通方式延时
}

/**
  * @brief  毫秒级延时,不会引起任务调度
  * @param  nms 延时毫秒数
  * @retval None
  */
void delay_xms(u32 nms)
{
	u32 i;
	for(i=0;i<nms;i++) delay_us(1000);
}
