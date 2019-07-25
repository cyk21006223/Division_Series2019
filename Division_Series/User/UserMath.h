#ifndef __USERMATH_H
#define __USERMATH_H

//#include "Users.h"

#define HIGH 	0x1
#define LOW 	0x0

//#define PI 3.1415926535897932384626433832795f
#define M_PI 3.1415926535897932384626433832795f
#define HALF_PI 1.5707963267948966192313216916398f
#define TWO_PI 6.283185307179586476925286766559f
#define DEG_TO_RAD 0.017453292519943295769236907684886f
#define RAD_TO_DEG 57.295779513082320876798154814105f
#define EULER 2.718281828459045235360287471352f

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))//取绝对值
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))//将值归一化到某个区间内
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))//四舍五入取偶
#define radians(deg) ((deg)*DEG_TO_RAD)//弧度
#define degrees(rad) ((rad)*RAD_TO_DEG)//角度
#define sq(x) ((x)*(x))//取平方
#define swap(a, b) { uint8_t t = a; a = b; b = t; }

//限制区间
#define VAL_LIMIT(val, min, max) \
do { \
if((val) <= (min)) \
{ \
  (val) = (min); \
} \
else if((val) >= (max)) \
{ \
  (val) = (max); \
} \
} while(0) \


#define VALUESET_STEP(Value, Target, Step) \
if (Value - Target > Step) \
{ \
	Value -= Step; \
} \
else if (Value - Target < -Step) \
{ \
	Value += Step; \
} \
else \
{ \
	Value = Target; \
} \


#define getBit(value, pos) ((value >> pos) & 1)//取值
#define setBit(value, pos) value|(1 << pos)//赋值
#define clrBit(value, pos) value&(~(1 << pos))//清值置零
#define toggleBit(value, pos) value^(1 << pos)//取反

long map(long, long, long, long, long);// [in_min, in_max]范围内的x等比映射到[out_min, out_max]范围内

void ValueSwap(int p1,int p2,float *data);

void RoundUp(float in, float *out, float num);

#endif /* __USERMATH_H */
