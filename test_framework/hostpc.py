import cantools
import can
import os
from time import sleep
from pprint import pprint

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

for Parameter in range (90, 110, 2):

    # send signal injection values
    data = encoded_msg.encode({'Parameter': Parameter, 'Test_ID': test_id, 'Run_Test': 1})
    message = can.Message(arbitration_id = encoded_msg.frame_id, data = data)
    bus.send(message)

    # delay to allow MCU to dothatshit!
    sleep(1)

    # filter received messages for Insulation_Monitoring_Device
    message = bus.recv()

    while message.arbitration_id != receive_msg.frame_id:
        message = bus.recv()

    decoded_msg = dbc.decode_message(message.arbitration_id, message.data)

    # check value against trip_threshold, ensure that it fails/passes when expected
    print("Sent resistance = " + str(Parameter) + ", MCU resistance = " + str(decoded_msg['Resistance']) + ", Fault = " + str(decoded_msg['Insulation_Fault']))

# send a stop command

data = encoded_msg.encode({'Parameter': 0, 'Test_ID': test_id, 'Run_Test': 0})
message = can.Message(arbitration_id = encoded_msg.frame_id, data = data)
bus.send(message)
