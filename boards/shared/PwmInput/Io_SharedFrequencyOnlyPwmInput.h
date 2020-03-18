#pragma once

#include <stm32f3xx_hal.h>

// Application specific configuration options.
#include "Io_SharedFrequencyOnlyPwmInputConfig.h"

// Check all the required application specific macros have been defined in
// <Io_SharedPwmInputConfig.h>.
#ifndef MAX_NUM_OF_FREQUENCY_ONLY_PWM_INPUTS
#error Missing definition: MAX_NUM_OF_FREQUENCY_ONLY_PWM_INPUTS must be defined in Io_SharedFrequencyOnlyPwmInputConfig.h.
#endif

struct FrequencyOnlyPwmInput;

/**
 * Allocate and initialize a frequency-only PWM input (no duty cycle).
 *
 * @note Reset Mode *must not* be selected as Slave Mode. This library assumes
 *       that the timer is free-running from 0 to Auto-Reload Register.
 *
 * @htim: The handle of the timer measuring the frequency-only PWM input
 * @timer_frequency_hz: The frequency of the timer measuring the frequency-only
 *                      PWM input
 * @timer_auto_reload_register: The value that is reloaded into the counter once
 *                              the counter has overflowed
 * @rising_edge_tim_channel: The rising edge channel of the timer measuring the
 *                           frequency-only PWM input
 * @return Pointer to the allocated and initialized frequency-only PWM input
 */
struct FrequencyOnlyPwmInput *Io_SharedFrequencyOnlyPwmInput_Create(
    TIM_HandleTypeDef *htim,
    uint32_t           timer_frequency_hz,
    uint32_t           timer_auto_reload_register,
    uint32_t           rising_edge_tim_channel);

/**
 * Update the frequency the given frequency-only PWM input
 * @param pwm_input: The frequency-only PWM input to update for
 */
void Io_SharedFrequencyOnlyPwmInput_Tick(
    struct FrequencyOnlyPwmInput *const pwm_input);

/**
 * Get the frequency for the given frequency-only PWM input
 * @param pwm_input: The frequency-only PWM input to get frequency for
 * @return The frequency for the given frequency-only PWM input
 */
float Io_SharedFrequencyOnlyPwmInput_GetFrequency(
    const struct FrequencyOnlyPwmInput *const pwm_input);
