#include <auto_generated/App_CanTx.h>
#include "SharedAssert.h"
#include "SharedPwmInput.h"

float SharedPwmInput_GetDutyCycle(struct PwmInput *pwm_input)
{
    shared_assert(pwm_input != NULL);

    return *pwm_input->duty_cycle_ptr;
}

static void SharedPwmInput_SetDutyCycle(struct PwmInput *pwm_input, float value)
{
    shared_assert(pwm_input != NULL);

    *pwm_input->duty_cycle_ptr = value;
}

float SharedPwmInput_GetFrequency(struct PwmInput *pwm_input)
{
    shared_assert(pwm_input != NULL);

    return *pwm_input->frequency_ptr;
}

static void SharedPwmInput_SetFrequency(struct PwmInput *pwm_input, float value)
{
    shared_assert(pwm_input != NULL);

    *pwm_input->frequency_ptr = value;
}

HAL_TIM_ActiveChannel SharedPwmInput_GetTimChannel(struct PwmInput *pwm_input)
{
    shared_assert(pwm_input != NULL);

    return pwm_input->settings.htim->Channel;
}

void SharedPwmInput_Update(struct PwmInput *pwm_input)
{
    shared_assert(pwm_input != NULL);

    uint32_t ic_rising_edge = HAL_TIM_ReadCapturedValue(
        pwm_input->settings.htim, pwm_input->settings.rising_edge_tim_channel);

    if (ic_rising_edge != 0U)
    {
        uint32_t ic_falling_edge = HAL_TIM_ReadCapturedValue(
            pwm_input->settings.htim,
            pwm_input->settings.falling_edge_tim_channel);

        // TODO: Check
        SharedPwmInput_SetDutyCycle(
            pwm_input, ic_falling_edge * 100.0f / ic_rising_edge);

        // TODO: shouldn't divide by 1000, do that in prescaler
        SharedPwmInput_SetFrequency(
            pwm_input,
            pwm_input->settings.timer_frequency_hz / 1000.0f / ic_rising_edge);
    }
    else
    {
        SharedPwmInput_SetDutyCycle(pwm_input, 0.0f);
        SharedPwmInput_SetFrequency(pwm_input, 0.0f);
    }
}

void SharedPwmInput_Init(
    struct PwmInput *        pwm_input,
    struct PwmInputSettings *settings,
    float *                  frequency_ptr,
    float *                  duty_cycle_ptr)
{
    shared_assert(pwm_input != NULL);
    shared_assert(settings != NULL);
    shared_assert(frequency_ptr);
    shared_assert(duty_cycle_ptr);

    pwm_input->settings       = *settings;
    pwm_input->frequency_ptr  = frequency_ptr;
    pwm_input->duty_cycle_ptr = duty_cycle_ptr;

    HAL_TIM_IC_Start_IT(settings->htim, settings->rising_edge_tim_channel);
    HAL_TIM_IC_Start_IT(settings->htim, settings->falling_edge_tim_channel);
}
