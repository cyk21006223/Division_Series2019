#include "Rocker.h"

/**
  * @brief  		��x��y������ҡ������
  * @param[in]  posX		x����
  *							posY		y����
  * @param[out]	*roc	 	ҡ������
  * @retval			None
  */
void Rocker_getData(float posX, float posY, rocker_t *roc)
{
    /*roc�ǲ�������Ӧ���Ƕ�Ӧ��rocker_t�ṹ��*/
    roc->x = posX;
    roc->y = posY;


    roc->radian = atan2(roc->y, roc->x);


    /*180.0f / PIΪһ��*/
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
