#include <cmsis_os.h>
#include <stm32f3xx_hal.h>
#include "auto_generated/App_CanTx.h"
#include "SharedAssert.h"
#include "SharedPwmInput.h"
#include "App_Imd.h"
#include "override_funcs.h"

enum Imd_Condition
{
    IMD_NORMAL,
    IMD_UNDERVOLTAGE_DETECTED,
    IMD_SST,
    IMD_DEVICE_ERROR,
    IMD_FAULT,
    NUM_OF_IMD_CONDITIONS,
    IMD_NO_MATCHING_CONDITION
};

#define CONDITION_FREQUENCY_TOLERANCE 2U // +/- Hz
static uint8_t imd_condition_frequency_mapping[NUM_OF_IMD_CONDITIONS] = {
    [IMD_NORMAL] = 10U, [IMD_UNDERVOLTAGE_DETECTED] = 20U,
    [IMD_SST] = 30U,    [IMD_DEVICE_ERROR] = 40U,
    [IMD_FAULT] = 50U,
};

extern TIM_HandleTypeDef htim2;
extern struct setter     Imd_SetInsulationResistanceIsValid_setter;
extern struct setter     Imd_SetInsulationResistanceReading_setter;

struct InsulationResistanceDcp
{
    uint16_t *reading_ptr; // Ohm
    bool *    is_valid_ptr;
};

struct ImdSettings
{
    TickType_t response_time_dcp;
    uint32_t   response_value_kohm; // kOhm
};

struct Imd
{
    struct PwmInput                pwm_input;
    struct ImdSettings             settings;
    struct InsulationResistanceDcp insulation_resistance;
    bool *active_fault_ptr; // remove this for formula b/c it's covered by OKHS
    TickType_t start_time;
};

struct Imd hv_imd;

static enum Imd_Condition Imd_GetCondition(struct Imd *imd)
{
    shared_assert(imd != NULL);

    const float frequency = SharedPwmInput_GetFrequency(Imd_GetPwmInput(imd));

    enum Imd_Condition condition = IMD_NO_MATCHING_CONDITION;

    for (uint32_t i = 0U; i < NUM_OF_IMD_CONDITIONS; i++)
    {
        if (frequency < imd_condition_frequency_mapping[i] +
                            CONDITION_FREQUENCY_TOLERANCE &&
            frequency > imd_condition_frequency_mapping[i] -
                            CONDITION_FREQUENCY_TOLERANCE)
        {
            condition = i;
        }
    }

    return condition;
}

struct PwmInput *Imd_GetPwmInput(struct Imd *imd)
{
    shared_assert(imd != NULL);

    return &imd->pwm_input;
}

struct InsulationResistanceDcp Imd_GetInsulationResistance(struct Imd *imd)
{
    shared_assert(imd != NULL);

    return imd->insulation_resistance;
}

TL85 void Imd_SetInsulationResistanceReading(
    struct Imd *tl85_imd,
    uint16_t    tl85_resistance)
{
    shared_assert(tl85_imd != NULL);

    *(tl85_imd->insulation_resistance.reading_ptr) = tl85_resistance;
}

TL85 uint16_t Imd_GetInsulationResistanceReading(struct Imd *imd)
{
    shared_assert(imd != NULL);

    return *(imd->insulation_resistance.reading_ptr);
}
// TODO: Change bool to other data types to see if codegen issue still occurs
static void
    Imd_SetInsulationResistanceIsValid(struct Imd *tl85_imd, bool is_valid)
{
    shared_assert(tl85_imd != NULL);

    *(tl85_imd->insulation_resistance.is_valid_ptr) = is_valid;
}

static bool Imd_GetInsulationResistanceIsValid(struct Imd *imd)
{
    shared_assert(imd != NULL);

    return *(imd->insulation_resistance.is_valid_ptr);
}

static void Imd_SetActiveFault(struct Imd *imd, bool active_fault)
{
    shared_assert(imd != NULL);

    *(imd->active_fault_ptr) = active_fault;
}

void Imd_Update(struct Imd *imd)
{
    shared_assert(imd != NULL);

    static bool first_time = true;

    if (first_time)
    {
        imd->start_time = osKernelSysTick();
        first_time      = false;
    }

    SharedPwmInput_Update(&imd->pwm_input);

    if (Imd_GetCondition(imd) == IMD_NORMAL)
    {
        TickType_t elapsed_time = osKernelSysTick() - imd->start_time;
        if (!first_time && !Imd_GetInsulationResistanceIsValid(imd) &&
            (elapsed_time > imd->settings.response_time_dcp))
        {
            Imd_SetInsulationResistanceIsValid(imd, true);
        }
    }
    else
    {
        Imd_SetInsulationResistanceIsValid(imd, false);
    }

    // Consult datasheet on how to convert duty cycle to resistance
    uint16_t resistance =
        1080.0f /
            (SharedPwmInput_GetDutyCycle(&imd->pwm_input) / 100.0f - 0.05f) -
        1200.0f;

    Imd_SetInsulationResistanceReading_setter.normal_setter(imd, resistance);

    if ((Imd_GetInsulationResistanceReading(imd) <
         imd->settings.response_value_kohm))
        Imd_SetActiveFault(imd, true);
    else
        Imd_SetActiveFault(imd, false);
}

void _Imd_Init(
    struct Imd *             imd,
    struct ImdSettings *     imd_settings,
    struct PwmInputSettings *pwm_input_settings)
{
    shared_assert(imd != NULL);
    shared_assert(imd_settings != NULL);
    shared_assert(pwm_input_settings != NULL);

    SharedPwmInput_Init(
        &imd->pwm_input, pwm_input_settings,
        App_CanTx_GetPeriodicSignalPointer_FREQUENCY(),
        App_CanTx_GetPeriodicSignalPointer_DUTY_CYCLE());

    imd->settings = *imd_settings;

    imd->insulation_resistance.is_valid_ptr =
        (bool *)App_CanTx_GetPeriodicSignalPointer_VALID_RESISTANCE();
    imd->insulation_resistance.reading_ptr =
        App_CanTx_GetPeriodicSignalPointer_RESISTANCE();
    imd->active_fault_ptr =
        (bool *)App_CanTx_GetPeriodicSignalPointer_INSULATION_FAULT();

    App_CanTx_SetPeriodicSignal_MINIMUM_RESISTANCE(
        imd->settings.response_value_kohm);

    Imd_SetInsulationResistanceReading(imd, 0U);
    Imd_SetInsulationResistanceIsValid(imd, false);
    Imd_SetActiveFault(imd, false);

    imd->start_time = 0U;
}

void Imd_Init(void)
{
    struct PwmInputSettings pwm_input_settings = {
        .htim                     = &htim2,
        .timer_frequency_hz       = 64000000U,
        .rising_edge_tim_channel  = TIM_CHANNEL_2,
        .falling_edge_tim_channel = TIM_CHANNEL_1,
    };

    struct ImdSettings imd_settings = {
        .response_value_kohm = 100U,
        .response_time_dcp   = pdMS_TO_TICKS(20000U),
    };

    _Imd_Init(&hv_imd, &imd_settings, &pwm_input_settings);
}

void Imd_InjectInsulationResistance(struct Imd *imd, uint32_t resistance)
{
    Imd_SetInsulationResistanceReading_setter.override_setter(imd, resistance);
}

void Imd_StopInjectInsulationResistance(void)
{
    Imd_SetInsulationResistanceReading_setter.disable_override();
}
