#include "UserMath.h"
/*�� [in_min, in_max]��Χ�ڵ�x�ȱ�ӳ�䵽[out_min, out_max]��Χ�ڡ�
	��������ֵΪlong��*/
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*����ֵ*/
void ValueSwap(int p1,int p2,float *data)
{
	float p;
	p=*(data+p1);
	*(data+p1)=*(data+p2);
	*(data+p2)=p;
}
/*��������,����numλС��*/
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



