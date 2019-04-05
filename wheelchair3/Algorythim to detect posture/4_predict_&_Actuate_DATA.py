from dotenv import load_dotenv
import os
import pickle
import serial
import pygatt
import numpy as np
import pyaudio
import wave
import time

from dcd.entities.thing import Thing
from dcd.entities.property import PropertyType

# The thing ID and access token
load_dotenv()
THING_ID = os.environ['THING_ID']
THING_TOKEN = os.environ['THING_TOKEN']


# Where to read the model from
MODEL_FILE_NAME = "model.pickle"

# load classifier
with open("model.pickle", 'rb') as file:
    neigh = pickle.load(file)

# classes = ["Not Sitting", "Proper Sitting", "Leaning Forward",
#           "Leaning Backward", "Leaning Left", "Leaning Right"]

classes = ["Not Sitting", "Proper Sitting", "Leg Stretch Right",
           "Leg Stretch Left", "Twist Right", "Twist Left", "Forward Bend"]

LABEL_PROP_NAME = "Yoga Wheelchair"
DATA_PROP_NAME = "fsrYoga"
# Read data from serial port
ser = serial.Serial(
    port=os.environ['SERIAL'],
    baudrate=115200,
    timeout=2)


"""
For connecting to the Bluetooth feather
"""
BLUETOOTH_DEVICE_MAC = os.environ['BLUETOOTH_DEVICE_MAC']

# UUID of the GATT characteristic to subscribe
GATT_CHARACTERISTIC_BUTTON = "00002345-0000-1000-8000-00805f9b34fb"
GATT_CHARACTERISTIC_POSTURE = "02118833-4455-6677-8899-AABBCCDDEEFF"
# Many devices, e.g. Fitbit, use random addressing, this is required to connect.
ADDRESS_TYPE = pygatt.BLEAddressType.random
# Start a BLE adapter
bleAdapter = pygatt.GATTToolBackend()
bleAdapter.start()
# User the BLE adapter to connect to our device
my_device = bleAdapter.connect(BLUETOOTH_DEVICE_MAC, address_type=ADDRESS_TYPE)

print("MY DEVICE: ")
print(my_device)

def timer():
   now = time.localtime(time.time())
   return now[5]

def discover_characteristic(device):
    for uuid in device.discover_characteristics().keys():
        try:
            print("Read UUID" + str(uuid) + "   " + str(device.char_read(uuid)))
        except:
            print("Something wrong with " + str(uuid))


def sendByBluetooth(x):
    x_Bytes = bytes(x)
    # print("x_Bytes = ")
    # print(x_Bytes)

    # my_device.char_write(GATT_CHARACTERISTIC_POSTURE, bytearray([x_Bytes, 0x00, 0x00]))
    my_device.char_write(GATT_CHARACTERISTIC_POSTURE, x_Bytes)
    print("sending :")
    print(x_Bytes)

def audioList(x):
    return {
        0: play_sound('/home/pi/wheelchair-design-platform/docs/workshops/audios/3_posture_1_right_side.wav', 10),
        1: play_sound('/home/pi/wheelchair-design-platform/docs/workshops/audios/4_posture_1_left.wav', 10),
        2: play_sound('/home/pi/wheelchair-design-platform/docs/workshops/audios/5_posture_2_side_1.wav', 10),
        3: play_sound('/home/pi/wheelchair-design-platform/docs/workshops/audios/6_posture_2_side_2.wav', 10),
        4: play_sound('/home/pi/wheelchair-design-platform/docs/workshops/audios/7_pose_3_down.wav', 10),
        5: play_sound('/home/pi/wheelchair-design-platform/docs/workshops/audios/8_back_to_comfortable_position.wav', 10),
    }.get(x, play_sound('/home/pi/wheelchair-design-platform/docs/workshops/audios/9_try_again.wav', 10))
# If default it will play the last audio

starttime = time.time()


def predict(values, prevResult=0, counter=0):

    result = neigh.predict(values)
    print(classes[result[0]])

    current_ts_ms = int(round(time.time() * 1000))

    sendByBluetooth(result+1)

    # prop_label.update_values(result, current_ts_ms)
    # prop_data.update_values(values, current_ts_ms)

    prop_label.update_values(result)



    """
    if result == prevResult and result != 0:
        counter = counter + 1
        prevResult = result
    else:
        counter = 0
        prevResult = result

    if counter == 5:
        # audioList(result+1)
        # sendByBluetooth(result+1)
        # print("tick")
        counter = 0
    """
    # Delay de un segundo
    # time.sleep(60.0 - ((time.time() - starttime) % 60.0))

prev_button_value = 0

# Real time prediction
def serial_to_property_values():
    global prev_button_value

    line_bytes = ser.readline()
    # print("LINE BYTES: ")
    # print(str(line_bytes))
    # print("LINE LENGTH: ")
    # print(len(line_bytes))
    # If the line is not empty
    if len(line_bytes) > 20:
        try:
            # Convert the bytes into string
            line = line_bytes.decode('utf-8')
            str_values = line.split(',B,')
            fsrString_values = str_values.pop(0)

            button_value = str_values.pop(0)

            # print("button_value = " + str(button_value) + "prev_button_value = " + str(prev_button_value))

            fsrValues = fsrString_values.split(',')

            values = [float(x) for x in fsrValues]
            values = [values]

            print("VALUES = " + str(values))


            np.array(values).reshape(1, -1)
            predict(values)

            # If the start button is pressed for the first time
            if button_value != prev_button_value:
                print("Start the Yoga session")
                prev_button_value = button_value

                play_sound('/home/pi/wheelchair-design-platform/docs/workshops/audios/1_intro_yoga.wav', 10)

                # Writes the button value in the BUTTON GATT CHARACTERISTIC
                # my_device.char_write(GATT_CHARACTERISTIC_BUTTON, bytes(button_value))

        except:
            ("cant parse ")

def play_sound(file, duration):

    print("playing audio")
    CHUNK = 1024

    # Load the WAV file
    wf = wave.open(file, 'rb')

    p = pyaudio.PyAudio()

    stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                    channels=wf.getnchannels(),
                    rate=wf.getframerate(),
                    output=True)

    data = wf.readframes(CHUNK)

    start_time = time.time()
    play = True
    while data != '' and play:
        stream.write(data)
        data = wf.readframes(CHUNK)
        if time.time()-start_time > duration:
            play = False

    stream.stop_stream()
    stream.close()

    p.terminate()

# Instantiate a thing with its credential
my_thing = Thing(thing_id=THING_ID, token=THING_TOKEN)

# Read the details of our Thing from the DCD Hub to get property details
my_thing.read()

# Find label and data property by name, create classes if none
prop_label = my_thing.find_or_create_property(LABEL_PROP_NAME, PropertyType.CLASS)
if prop_label.classes is None or len(prop_label.classes) == 0:
    prop_label.create_classes(classes)

prop_data = my_thing.find_or_create_property(DATA_PROP_NAME, PropertyType.NINE_DIMENSIONS)
# I read the characteristics defined by the feather
discover_characteristic(my_device)

# serial_to_property_values()
# while true funciona como la void loop() en arduino,ejecutandose continuamente
while True:
    serial_to_property_values()
