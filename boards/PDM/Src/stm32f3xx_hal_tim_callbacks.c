#include <stm32f3xx_hal.h>
#include "App_Imd.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (SharedPwmInput_GetTimChannel(Imd_GetPwmInput(&hv_imd)) == htim->Channel)
        Imd_Update(&hv_imd);
}
