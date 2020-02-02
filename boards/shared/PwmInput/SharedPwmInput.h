#pragma once
#include <stm32f3xx_hal.h>

struct PwmInputSettings
{
    TIM_HandleTypeDef *htim;
    uint32_t           timer_frequency_hz; // Hz
    uint32_t           rising_edge_tim_channel;
    uint32_t           falling_edge_tim_channel;
};

struct PwmInput
{
    struct PwmInputSettings settings;
    float *                 duty_cycle_ptr;
    float *                 frequency_ptr; // mHz
};

void SharedPwmInput_Init(
    struct PwmInput *        pwm_input,
    struct PwmInputSettings *settings,
    float *                  frequency_ptr,
    float *                  duty_cycle_ptr);
void                  SharedPwmInput_Update(struct PwmInput *pwm_input);
float                 SharedPwmInput_GetDutyCycle(struct PwmInput *pwm_input);
float                 SharedPwmInput_GetFrequency(struct PwmInput *pwm_input);
HAL_TIM_ActiveChannel SharedPwmInput_GetTimChannel(struct PwmInput *pwm_input);
