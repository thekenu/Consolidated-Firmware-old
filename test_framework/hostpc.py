import cantools
import can
import os
import time
from termcolor import colored

test_to_run = 'imd_test'

os.system("sudo ip link set can0 type can bitrate 500000")
os.system("sudo ip link set up can0")

current_dir = os.path.dirname(os.path.realpath(__file__))

dbc = cantools.database.load_file(current_dir + '/../boards/shared/CAN/CanMsgs.dbc')

bus = can.interface.Bus(bustype='socketcan', channel='can0', bitrate=500000)

trip_threshold = 100.0

# prepare receive test CAN ID
receive_msg = dbc.get_message_by_name('Insulation_Monitoring_Device')

# prepare signal injection test CAN message
encoded_msg = dbc.get_message_by_name('SIGNAL_INJECTION_TEST')
choices = encoded_msg.signal_choices_string()
test_id = int(choices[choices.find(test_to_run) - 2])

test_cnt = 1

for Parameter in range (91, 111, 2):

    # send signal injection values
    data = encoded_msg.encode({'Parameter': Parameter, 'Test_ID': test_id, 'Run_Test': 1})
    message = can.Message(arbitration_id = encoded_msg.frame_id, data = data, is_extended_id = False)
    bus.send(message)

    start_ms = int(round(time.time() * 1000))

    # filter received messages for Insulation_Monitoring_Device
    delay_time = 1000
    while (int(round(time.time() * 1000))- start_ms) < delay_time:
        message = bus.recv()

    while message.arbitration_id != receive_msg.frame_id:
        message = bus.recv()

    decoded_msg = dbc.decode_message(message.arbitration_id, message.data)

    # check value against trip_threshold, ensure that it fails/passes when expected

    print(colored("\n[Test " + str(test_cnt) + "]", attrs=['bold']))

    print(colored("[INJECT] IMD Insulation Resistance = " + str(Parameter) + " kOhm", 'yellow'))

    print(colored("[EXPECT] Fault = " + str(int(Parameter < trip_threshold)), 'blue'))

    print(colored("[ACTUAL] Fault = " + str(decoded_msg['Insulation_Fault']), 'blue'))

    if decoded_msg['Insulation_Fault'] != (Parameter < trip_threshold):
        print(colored("[RESULT] Fail", 'red'))
    else:
        print(colored("[RESULT] Success", 'green'))
    
    test_cnt = test_cnt + 1

# send a stop command
data = encoded_msg.encode({'Parameter': 0, 'Test_ID': test_id, 'Run_Test': 0})
message = can.Message(arbitration_id = encoded_msg.frame_id, data = data, is_extended_id = False)
bus.send(message)
