#include "Filter.h"


/**
  * @brief  IIR低通滤波，输入与输出不能是同一个变量
  * @param[in]  *in 输入数据
  *				 LpfAttFactor 低通滤波衰减因子 Attenuation should be between 0 to 1.
  * @param[out]	*out 数据输出
  * @retval None
  */
void Filter_IIRLPF(float *in,float *out, float LpfAttFactor)
{
    *out = *out + LpfAttFactor*(*in - *out);
}

/**
  * @brief  算数平均滤波加低通滤波
  * @param[in]  Num_Data_value_in  输入数据
  * @param[out]	Num_Data_value_out 数据输出
  * @retval None
  */
float Filter_Mean_Value( float Num_Data_value_in )
{
	float data[20];
	int Num_Data=20;
	int Num_couple=5;
	int i,j;
	float Num_Data_value;
	float Num_Data_value_out;
	
	for(i=0;i<Num_Data;i++)
	{
		data[i]=Num_Data_value_in;
		delay_us(20);
	}
		/*挑出Num_couple个最大值*/
	for(i=0;i<Num_couple;i++)
	{
		for(j=i+1;j<Num_Data;j++)
		{
			if(*(data+j)>*(data+i))
			{
				ValueSwap(j,i,data);
			}
		}
	}
	
	/*挑出Num_couple个最小值*/
	for(i=0;i<Num_couple;i++)
	{
		for(j=i+1;j<Num_Data-Num_couple;j++)
		{
			if( *(data+j+Num_couple) < *(data+i+Num_couple) )
			{
				ValueSwap(j+Num_couple,i+Num_couple,data);
			}
		}
	}
	
	/*求和*/
	for(i=0;i<Num_Data-2*Num_couple;i++)
	{
		Num_Data_value+=*(data+i);
	}

	/*求平均值*/
	Num_Data_value/=(Num_Data-2*Num_couple);
	
	for(i=0;i<Num_Data;i++)
	{
		Filter_IIRLPF(&Num_Data_value,&Num_Data_value_out,0.618);
		delay_us(20);
	}
	return Num_Data_value_out;
	
}

/**
  * @brief  滑动平均滤波
  * @param[in]  Data  输入数据
  * @param[in]  limitDepth  滤波深度不宜过大，否则加大数据延时，降低动态效应
  * @param[out]	out 数据输出
  * @retval None
  */
float Filter_Slide_Mean_Value(float Data, uint8_t limitDepth)
{
	static uint8_t count=0;//计数变量
	float sum=0;
	float value_buff[limitDepth];
	
	value_buff[count]=Data;
	
	for(uint8_t i=0;i<limitDepth;i++)
	{
	  sum+=value_buff[i];
	}
	
	count++;
	
	//当数据大于数组长度,替换数据组的一个数据,低位仍掉,先进先出
	if(count==limitDepth) 
		count=0;
	
  return ((float)sum/(float)limitDepth);
}


/**
  * @brief  卡尔曼滤波器初始化
  * @param[in] kalman_lcw：卡尔曼滤波器结构体
  * @param[in] init_x：待测量的初始值
  * @param[in] init_p：后验状态估计值误差的方差的初始值
  * @retval None
  */
void kalman_init ( kalman_struct *kalman_lcw, \
	                 float init_x, \
									 float init_p, \
									 float predict_q, \
									 float measure_q)
{
	kalman_lcw->x = init_x;//待测量的初始值，如有中值一般设成中值
	kalman_lcw->p = init_p;//后验状态估计值误差的方差的初始值
	kalman_lcw->A = 1;
	kalman_lcw->H = 1;
	kalman_lcw->q = predict_q;//预测(过程)噪声方差 实验发现修改此值会影响收敛速率
	kalman_lcw->r = measure_q;//测量(观测)噪声方差
	//这里两个参数是最关键的
}

/**
  * @brief  卡尔曼滤波器
  * @param[in] kalman_lcw:卡尔曼结构体
  * @param[in] measure；测量值
  * @retval 返回滤波后的值
  */
float kalman_filter(kalman_struct *kalman_lcw, float measure)
{
	/* Predict Begin */
	/*x的先验估计由上一个时间点的后验估计值和输入信息给出*/
	kalman_lcw->x = kalman_lcw->A * kalman_lcw->x;
	/*计算先验均方差 p(n|n-1)=A^2*p(n-1|n-1)+q */
	kalman_lcw->p = kalman_lcw->A * kalman_lcw->A * kalman_lcw->p + kalman_lcw->q; 
	/* Predict End */
	
	/* Measurement Begin */
	kalman_lcw->gain = kalman_lcw->p*(kalman_lcw->H / (kalman_lcw->p*kalman_lcw->H
	                  *kalman_lcw->H+kalman_lcw->r));
	/*利用残余的信息改善对x(t)的估计，给出后验估计，这个值也就是输出*/
	kalman_lcw->x = kalman_lcw->x + kalman_lcw->gain * (measure - kalman_lcw->H 
	               *kalman_lcw->x);
	/*计算后验均方差*/
	kalman_lcw->p = (1 - kalman_lcw->gain * kalman_lcw->H) * kalman_lcw->p;
	/* Measurement End */

	return kalman_lcw->x;
}



	