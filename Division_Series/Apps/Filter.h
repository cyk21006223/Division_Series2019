#ifndef __FILTER_H
#define __FILTER_H


#include "Users.h"

//1维的卡尔曼滤波
typedef struct {
    float x;  // 系统的状态量
    float A;  // x(n)=A*x(n-1)+u(n),u(n)~N(0,q)
    float H;  // z(n)=H*x(n)+w(n),w(n)~N(0,r)
    float q;  // 预测过程噪声协方差
    float r;  // 测量过程噪声协方差
    float p;  // 估计误差协方差
    float gain;//卡尔曼增益
}kalman_struct;

void Filter_IIRLPF(float *in,float *out, float LpfAttFactor);
float Filter_Mean_Value( float Num_Data_value_in );
float Filter_Slide_Mean_Value(float Data, uint8_t limitDepth);
void kalman_init ( kalman_struct *kalman_lcw, \
	                 float init_x, \
									 float init_p, \
									 float predict_q, \
									 float measure_q);
float kalman_filter(kalman_struct *kalman_lcw, float measure);
#endif /*__FILTER_H*/

