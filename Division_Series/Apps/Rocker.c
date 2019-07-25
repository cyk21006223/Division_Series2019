#include "Rocker.h"

/**
  * @brief  		将x，y坐标变成摇杆数据
  * @param[in]  posX		x坐标
  *							posY		y坐标
  * @param[out]	*roc	 	摇杆数据
  * @retval			None
  */
void Rocker_getData(float posX, float posY, rocker_t *roc)
{
    /*roc是参数，对应的是对应的rocker_t结构体*/
    roc->x = posX;
    roc->y = posY;


    roc->radian = atan2(roc->y, roc->x);


    /*180.0f / PI为一度*/
    roc->degrees = roc->radian * 180.0f / PI;

#if __FPU_PRESENT
    roc->distance = __sqrtf(pow(roc->x, 2) + pow(roc->y, 2));
#else
    roc->distance = sqrt(pow(roc->x, 2) + pow(roc->y, 2));
#endif

    if(roc->degrees>180) {
        roc->distance = -roc->distance;
        //rocL->degrees = -rocL->degrees + 360.0f;
    }
}
