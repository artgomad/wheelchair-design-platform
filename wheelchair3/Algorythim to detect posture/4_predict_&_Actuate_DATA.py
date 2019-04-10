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

    my_device.char_write(GATT_CHARACTERISTIC_POSTURE, x_Bytes)


def audioList(x):
    if x is 1:
        play_sound('/home/pi/wheelchair-design-platform/docs/workshops/audios/3_posture_1_right_side.wav', 23)
    elif x is 2:
        play_sound('/home/pi/wheelchair-design-platform/docs/workshops/audios/4_posture_1_left.wav', 26)
    elif x is 3:
        play_sound('/home/pi/wheelchair-design-platform/docs/workshops/audios/5_posture_2_side_1.wav', 20)
    elif x is 4:
        play_sound('/home/pi/wheelchair-design-platform/docs/workshops/audios/6_posture_2_side_2.wav', 26)
    elif x is 5:
        play_sound('/home/pi/wheelchair-design-platform/docs/workshops/audios/7_pose_3_down.wav', 22)
    elif x is 6:
        play_sound('/home/pi/wheelchair-design-platform/docs/workshops/audios/8_back_to_comfortable_position.wav', 7)

    #    play_sound('/home/pi/wheelchair-design-platform/docs/workshops/audios/9_try_again.wav', 10))
# If default it will play the last audio

starttime = time.time()

prevResult = 0
counter = 0
expectedPos = 1

def predict(values):

    global prevResult
    global counter
    global expectedPos

    result = neigh.predict(values)

    currentPos = int(result[0])

    """
    7. Updates the value of the 'Yoga Wheelchair' property in the DCD Hub
    """
    prop_label.update_values([currentPos])

    if result == prevResult:
        counter = counter + 1
        prevResult = result
    else:
        counter = 0
        prevResult = result

    """
    8. Check if the current posture equals the expected one without
       changing during 100 loop repetitions
    """
    if counter >= 100 and currentPos == expectedPos:
        """
        9. Send the required position via Bluetooth to the Feather
        """
        sendByBluetooth(result + 1)
        """"
        10. Play audio, guiding the user to start with the next posture
        """
        audioList(currentPos)
        expectedPos += 1
        counter = 0

    print("                                                            " + str(counter) + "        CURRENT POSITION = " + str(classes[result[0]]) + "               EXPECTED POSITION = " + str(classes[expectedPos]))


prev_button_value = 0


def serial_to_property_values():

    global prev_button_value

    line_bytes = ser.readline()

    # If the line is not corrupted
    if len(line_bytes) > 20:
        try:
            # Convert the bytes into string
            line = line_bytes.decode('utf-8')
            str_values = line.split(',B,')
            fsrString_values = str_values.pop(0)

            button_value = int(str_values.pop(0))

            print("button_value = " + str(button_value) + "prev_button_value = " + str(prev_button_value))

            fsrValues = fsrString_values.split(',')

            values = [float(x) for x in fsrValues]

            """
            4. Updates the values of the 'fsrYoga' property in the DCD Hub
            """
            prop_data.update_values(values)

            values = [values]

            """
            5. Starts Yoga Session
            """
            if button_value == 1 and button_value != prev_button_value:
                print("Start the Yoga session")
                prev_button_value = button_value
                sendByBluetooth([1])

                play_sound('/home/pi/wheelchair-design-platform/docs/workshops/audios/1_intro_yoga.wav', 47)
                play_sound('/home/pi/wheelchair-design-platform/docs/workshops/audios/2_intro_postures.wav', 11)

            """
            6. Uses the algorithm to predict the current posture of the user
            based on the readings of the 9 FSRs
            """
            np.array(values).reshape(1, -1)
            predict(values)

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


"""
 1. Connects with the Data Centric Design Hub
"""
# Instantiate a thing with its credential
my_thing = Thing(thing_id=THING_ID, token=THING_TOKEN)

# Read the details of our Thing from the DCD Hub to get property details
my_thing.read()

# Find label and data property by name, create classes if none
prop_label = my_thing.find_or_create_property(LABEL_PROP_NAME, PropertyType.CLASS)
if prop_label.classes is None or len(prop_label.classes) == 0:
    prop_label.create_classes(classes)

prop_data = my_thing.find_or_create_property(DATA_PROP_NAME, PropertyType.NINE_DIMENSIONS)


"""
2. Reads the characteristics defined by the feather
"""
discover_characteristic(my_device)


"""
3. Reads the Serial Port uninterruptedly extracting the value of the 9 FSRs
and the button in separate variables.
"""
while True:
    serial_to_property_values()
