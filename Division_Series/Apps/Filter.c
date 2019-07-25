#include "Filter.h"


/**
  * @brief  IIR��ͨ�˲������������������ͬһ������
  * @param[in]  *in ��������
  *				 LpfAttFactor ��ͨ�˲�˥������ Attenuation should be between 0 to 1.
  * @param[out]	*out �������
  * @retval None
  */
void Filter_IIRLPF(float *in,float *out, float LpfAttFactor)
{
    *out = *out + LpfAttFactor*(*in - *out);
}

/**
  * @brief  ����ƽ���˲��ӵ�ͨ�˲�
  * @param[in]  Num_Data_value_in  ��������
  * @param[out]	Num_Data_value_out �������
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
		/*����Num_couple�����ֵ*/
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
	
	/*����Num_couple����Сֵ*/
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
	
	/*���*/
	for(i=0;i<Num_Data-2*Num_couple;i++)
	{
		Num_Data_value+=*(data+i);
	}

	/*��ƽ��ֵ*/
	Num_Data_value/=(Num_Data-2*Num_couple);
	
	for(i=0;i<Num_Data;i++)
	{
		Filter_IIRLPF(&Num_Data_value,&Num_Data_value_out,0.618);
		delay_us(20);
	}
	return Num_Data_value_out;
	
}

/**
  * @brief  ����ƽ���˲�
  * @param[in]  Data  ��������
  * @param[in]  limitDepth  �˲���Ȳ��˹��󣬷���Ӵ�������ʱ�����Ͷ�̬ЧӦ
  * @param[out]	out �������
  * @retval None
  */
float Filter_Slide_Mean_Value(float Data, uint8_t limitDepth)
{
	static uint8_t count=0;//��������
	float sum=0;
	float value_buff[limitDepth];
	
	value_buff[count]=Data;
	
	for(uint8_t i=0;i<limitDepth;i++)
	{
	  sum+=value_buff[i];
	}
	
	count++;
	
	//�����ݴ������鳤��,�滻�������һ������,��λ�Ե�,�Ƚ��ȳ�
	if(count==limitDepth) 
		count=0;
	
  return ((float)sum/(float)limitDepth);
}


/**
  * @brief  �������˲�����ʼ��
  * @param[in] kalman_lcw���������˲����ṹ��
  * @param[in] init_x���������ĳ�ʼֵ
  * @param[in] init_p������״̬����ֵ���ķ���ĳ�ʼֵ
  * @retval None
  */
void kalman_init ( kalman_struct *kalman_lcw, \
	                 float init_x, \
									 float init_p, \
									 float predict_q, \
									 float measure_q)
{
	kalman_lcw->x = init_x;//�������ĳ�ʼֵ��������ֵһ�������ֵ
	kalman_lcw->p = init_p;//����״̬����ֵ���ķ���ĳ�ʼֵ
	kalman_lcw->A = 1;
	kalman_lcw->H = 1;
	kalman_lcw->q = predict_q;//Ԥ��(����)�������� ʵ�鷢���޸Ĵ�ֵ��Ӱ����������
	kalman_lcw->r = measure_q;//����(�۲�)��������
	//����������������ؼ���
}

/**
  * @brief  �������˲���
  * @param[in] kalman_lcw:�������ṹ��
  * @param[in] measure������ֵ
  * @retval �����˲����ֵ
  */
float kalman_filter(kalman_struct *kalman_lcw, float measure)
{
	/* Predict Begin */
	/*x�������������һ��ʱ���ĺ������ֵ��������Ϣ����*/
	kalman_lcw->x = kalman_lcw->A * kalman_lcw->x;
	/*������������� p(n|n-1)=A^2*p(n-1|n-1)+q */
	kalman_lcw->p = kalman_lcw->A * kalman_lcw->A * kalman_lcw->p + kalman_lcw->q; 
	/* Predict End */
	
	/* Measurement Begin */
	kalman_lcw->gain = kalman_lcw->p*(kalman_lcw->H / (kalman_lcw->p*kalman_lcw->H
	                  *kalman_lcw->H+kalman_lcw->r));
	/*���ò������Ϣ���ƶ�x(t)�Ĺ��ƣ�����������ƣ����ֵҲ�������*/
	kalman_lcw->x = kalman_lcw->x + kalman_lcw->gain * (measure - kalman_lcw->H 
	               *kalman_lcw->x);
	/*������������*/
	kalman_lcw->p = (1 - kalman_lcw->gain * kalman_lcw->H) * kalman_lcw->p;
	/* Measurement End */

	return kalman_lcw->x;
}



	