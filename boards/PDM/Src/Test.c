#include "Test.h"
#include "App_Imd.h"
#include "auto_generated/App_CanTx.h"

static void Test_StartTest(uint32_t test_id, uint32_t parameter)
{
    switch (test_id)
    {
        case CANMSGS_SIGNAL_INJECTION_TEST_TEST_ID_IMD_TEST_CHOICE:
        {
            Imd_InjectInsulationResistance(&hv_imd, parameter);
        }
        break;
        default:
            break;
    }
}

static void Test_StopTest(uint32_t test_id)
{
    switch (test_id)
    {
        case CANMSGS_SIGNAL_INJECTION_TEST_TEST_ID_IMD_TEST_CHOICE:
        {
            Imd_StopInjectInsulationResistance();
        }
        break;
        default:
            break;
    }
}

void Test_CanCallback(uint32_t stdid, uint8_t data[8])
{
    if (stdid == CANMSGS_SIGNAL_INJECTION_TEST_FRAME_ID)
    {
        struct CanMsgs_signal_injection_test_t request;

        CanMsgs_signal_injection_test_unpack(
            &request, data, CANMSGS_SIGNAL_INJECTION_TEST_LENGTH);

        uint32_t test_id = request.test_id;

        if (request.run_test)
        {
            Test_StartTest(test_id, request.parameter);
        }
        else
        {
            Test_StopTest(test_id);
        }
    }
}
