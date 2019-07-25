#include "UserMath.h"
/*将 [in_min, in_max]范围内的x等比映射到[out_min, out_max]范围内。
	函数返回值为long型*/
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*交换值*/
void ValueSwap(int p1,int p2,float *data)
{
	float p;
	p=*(data+p1);
	*(data+p1)=*(data+p2);
	*(data+p2)=p;
}
/*四舍五入,保留num位小数*/
void RoundUp(float in, float *out, float num)
{
	if(in>=0)
	{
    *out = (float)((int)(in*10.0f*num+0.5f)/(10.0*num));
	}
	else{
    *out = (float)((int)(in*10.0f*num-0.5f)/(10.0*num));
	}
}



