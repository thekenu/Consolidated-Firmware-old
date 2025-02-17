#include "main.h"
#include "App_SharedMacros.h"
#include "App_SharedAssert.h"
#include "Io_StackWaterMark.h"
#include "Io_SharedStackWaterMark.h"
#include "auto_generated/App_CanTx.h"

// We check the stack water mark for the following tasks
extern TaskHandle_t Task1HzHandle;
extern TaskHandle_t Task1kHzHandle;
extern TaskHandle_t TaskCanRxHandle;
extern TaskHandle_t TaskCanTxHandle;

static struct DcmCanTxInterface *_can_tx_interface = NULL;

/** @brief The stack watermark threshold as a percentage of the stack size */
#define STACK_HIGH_WATERMARK_THRESHOLD 0.7f

void App_StackWaterMark_Init(struct DcmCanTxInterface *can_tx_interface)
{
    shared_assert(can_tx_interface != NULL);
    _can_tx_interface = can_tx_interface;
}

static void logWaterMarkAboveThresholdTask1kHz(uint8_t error)
{
    shared_assert(_can_tx_interface != NULL);
    App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_KHZ(
        _can_tx_interface, error);
}

static void logWaterMarkAboveThresholdTask1Hz(uint8_t error)
{
    shared_assert(_can_tx_interface != NULL);
    App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ(
        _can_tx_interface, error);
}

static void logWaterMarkAboveThresholdTaskCanRx(uint8_t error)
{
    shared_assert(_can_tx_interface != NULL);
    App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX(
        _can_tx_interface, error);
}

static void logWaterMarkAboveThresholdTaskCanTx(uint8_t error)
{
    shared_assert(_can_tx_interface != NULL);
    App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX(
        _can_tx_interface, error);
}

/** @brief Iterate through this table to check stack watermarks for each task */
static struct stack_watermark stack_watermarks[] = {
    {
        .handle              = &Task1HzHandle,
        .stack_size          = TASK1HZ_STACK_SIZE,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error           = logWaterMarkAboveThresholdTask1Hz,
    },
    {
        .handle              = &Task1kHzHandle,
        .stack_size          = TASK1KHZ_STACK_SIZE,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error           = logWaterMarkAboveThresholdTask1kHz,
    },
    {
        .handle              = &TaskCanRxHandle,
        .stack_size          = TASKCANRX_STACK_SIZE,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error           = logWaterMarkAboveThresholdTaskCanRx,
    },
    {
        .handle              = &TaskCanTxHandle,
        .stack_size          = TASKCANTX_STACK_SIZE,
        .watermark_threshold = STACK_HIGH_WATERMARK_THRESHOLD,
        .log_error           = logWaterMarkAboveThresholdTaskCanTx,
    },
};

void App_StackWaterMark_Check(void)
{
    Io_SharedStackWaterMark_Check(
        stack_watermarks, NUM_ELEMENTS_IN_ARRAY(stack_watermarks));
}
