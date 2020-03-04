#pragma once

#include "App_SharedStateMachine.h"

/*
 * Initialize the state machine by adding states to it.
 */
void App_StateMachine_Init(void);

/**
 * Run a single tick of the state machine.
 */
void App_StateMachine_Tick(void);
