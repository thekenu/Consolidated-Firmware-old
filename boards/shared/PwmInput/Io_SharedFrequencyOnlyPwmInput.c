#include <stdbool.h>

#include "SharedAssert.h"
#include "Io_SharedFrequencyOnlyPwmInput.h"

struct FrequencyOnlyPwmInput
{
    float              frequency_hz;
    TIM_HandleTypeDef *htim;
    float              timer_frequency_hz;
    uint32_t           rising_edge_tim_channel;
    uint32_t           auto_reload_register;
};

/**
 * Set the frequency for the given frequency-only PWM input
 * @param pwm_input: The frequency-only PWM input to set frequency for
 * @param frequency_hz: Frequency, in Hz
 */
static void Io_SetFrequency(
    struct FrequencyOnlyPwmInput *const pwm_input,
    const float                         frequency_hz);

static void Io_SetFrequency(
    struct FrequencyOnlyPwmInput *const pwm_input,
    const float                         frequency_hz)
{
    shared_assert(pwm_input != NULL);
    shared_assert(frequency_hz >= 0.0f);

    pwm_input->frequency_hz = frequency_hz;
}

struct FrequencyOnlyPwmInput *
    Io_SharedFrequencyOnlyPwmInput_CreateFrequencyOnly(
        TIM_HandleTypeDef *htim,
        uint32_t           timer_frequency_hz,
        uint32_t           timer_auto_reload_register,
        uint32_t           rising_edge_tim_channel)
{
    shared_assert(htim != NULL);

    static struct FrequencyOnlyPwmInput
                  pwm_inputs[MAX_NUM_OF_FREQUENCY_ONLY_PWM_INPUTS];
    static size_t alloc_index = 0;

    shared_assert(alloc_index < MAX_NUM_OF_FREQUENCY_ONLY_PWM_INPUTS);

    struct FrequencyOnlyPwmInput *const pwm_input = &pwm_inputs[alloc_index++];

    pwm_input->htim                    = htim;
    pwm_input->timer_frequency_hz      = timer_frequency_hz;
    pwm_input->rising_edge_tim_channel = rising_edge_tim_channel;
    pwm_input->auto_reload_register    = timer_auto_reload_register;

    HAL_TIM_IC_Start_IT(htim, rising_edge_tim_channel);

    return pwm_input;
}

void Io_SharedFrequencyOnlyPwmInput_TickFrequencyOnly(
    struct FrequencyOnlyPwmInput *const pwm_input)
{
    shared_assert(pwm_input != NULL);

    static bool first_tick           = true;
    uint32_t    previous_rising_edge = 0;
    uint32_t    current_rising_edge  = 0;
    uint32_t    delta_rising_edge    = 0;

    if (first_tick)
    {
        current_rising_edge = HAL_TIM_ReadCapturedValue(
            pwm_input->htim, pwm_input->rising_edge_tim_channel);
        Io_SetFrequency(pwm_input, 0.0f);
        first_tick = false;
    }
    else
    {
        previous_rising_edge = current_rising_edge;
        current_rising_edge  = HAL_TIM_ReadCapturedValue(
            pwm_input->htim, pwm_input->rising_edge_tim_channel);

        // Compute the difference between the current and previous
        // input-captured counter value
        if (current_rising_edge > previous_rising_edge)
        {
            delta_rising_edge = current_rising_edge - previous_rising_edge;
            Io_SetFrequency(
                pwm_input, pwm_input->timer_frequency_hz / delta_rising_edge);
        }
        else if (current_rising_edge < previous_rising_edge)
        {
            // Handle timer overflow
            delta_rising_edge = (pwm_input->auto_reload_register + 1) -
                                previous_rising_edge + current_rising_edge;
            Io_SetFrequency(
                pwm_input, pwm_input->timer_frequency_hz / delta_rising_edge);
        }
        else
        {
            // We should never get here, but if we do it means we the frequency
            // we're trying to measure is either too slow or too fast. Adjust
            // the timer prescaler so we don't get here.
            Io_SetFrequency(pwm_input, 0.0f);
        }
    }
}

float Io_SharedFrequencyOnlyPwmInput_GetFrequency(
    const struct FrequencyOnlyPwmInput *const pwm_input)
{
    shared_assert(pwm_input != NULL);

    return pwm_input->frequency_hz;
}
