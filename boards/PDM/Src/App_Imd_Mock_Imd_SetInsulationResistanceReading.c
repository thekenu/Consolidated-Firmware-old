/* This file is auto-generated. DO NOT MODIFY! */

#include "override_funcs.h"
#include "stdbool.h"
#include "App_Imd.h"

#define WEAK_FUNC __attribute__((weak))
#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

/* Provide function declarations manually for the helper functions: This may
   seem "hacky", but the alternative would be to include header files like
   adc_readings.h in override_funcs.h, and that makes this library more
   diffuclt to use */
void Imd_SetInsulationResistanceReading(
    struct Imd *tl85_imd,
    __uint32_t  tl85_resistance); // This is the original function which we are
                                 // generated stubs for
void Imd_SetInsulationResistanceReading_override_hook(
    struct Imd *tl85_imd,
    __uint32_t
        tl85_resistance); // This is the function that the user should overwrite

static void Imd_SetInsulationResistanceReading_normalSetter(
    struct Imd *tl85_imd,
    __uint32_t  tl85_resistance);
static void Imd_SetInsulationResistanceReading_overrideSetter(
    struct Imd *tl85_imd,
    __uint32_t  tl85_resistance);
static void Imd_SetInsulationResistanceReading_stopOverride(void);

struct setter Imd_SetInsulationResistanceReading_setter = {
    .normal_setter       = Imd_SetInsulationResistanceReading_normalSetter,
    .override_setter     = Imd_SetInsulationResistanceReading_overrideSetter,
    .disable_override    = Imd_SetInsulationResistanceReading_stopOverride,
    .is_override_enabled = false,
};

/*
 * @info Setter function to indicate whether override is in
 *       progress for Imd_SetInsulationResistanceReading
 */
static void Imd_SetInsulationResistanceReading_stopOverride(void)
{
    Imd_SetInsulationResistanceReading_setter.is_override_enabled = false;
}

/**
 * @info Wrapper for the Imd_SetInsulationResistanceReading to only prohibit
 * from being called when an override is in progress
 */
static void Imd_SetInsulationResistanceReading_normalSetter(
    struct Imd *tl85_imd,
    __uint32_t  tl85_resistance)
{
    if (Imd_SetInsulationResistanceReading_setter.is_override_enabled == false)
    {
        Imd_SetInsulationResistanceReading(tl85_imd, tl85_resistance);
    }
}

/**
 * @info Override function
 */
static void Imd_SetInsulationResistanceReading_overrideSetter(
    struct Imd *tl85_imd,
    __uint32_t  tl85_resistance)
{
    Imd_SetInsulationResistanceReading_setter.is_override_enabled = true;

    if (Imd_SetInsulationResistanceReading_setter.is_override_enabled == true)
    {
        Imd_SetInsulationResistanceReading(tl85_imd, tl85_resistance);
    }
}
