#pragma once
#include <stdbool.h>
#include "SharedPwmInput.h"

struct Imd;
struct InsulationResistanceDCp;

extern struct Imd hv_imd;

void                           Imd_Init(void);
struct PwmInput *              Imd_GetPwmInput(struct Imd *imd);
struct InsulationResistanceDcp Imd_GetInsulationResistance(struct Imd *imd);
void                           Imd_SetInsulationResistance(
                              struct Imd *                   imd,
                              struct InsulationResistanceDcp resistance);
void Imd_Update(struct Imd *imd);

void Imd_InjectInsulationResistance(struct Imd *imd, uint32_t resistance);
void Imd_StopInjectInsulationResistance(void);
