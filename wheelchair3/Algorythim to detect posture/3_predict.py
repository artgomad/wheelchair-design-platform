from dotenv import load_dotenv
import os
import pickle
import serial
import numpy as np

load_dotenv()

# Where to read the model from
MODEL_FILE_NAME = "model.pickle"

# load classifier
with open("model.pickle", 'rb') as file:
    neigh = pickle.load(file)

# classes = ["Not Sitting", "Proper Sitting", "Leaning Forward",
#           "Leaning Backward", "Leaning Left", "Leaning Right"]

classes = ["Not Sitting", "Proper Sitting"]

# Read data from serial port
ser = serial.Serial(
    port=os.environ['SERIAL'],
    baudrate=115200,
    timeout=2)

def predict(values):
    result = neigh.predict(values)
    print(classes[result[0]])

# Real time prediction
def serial_to_property_values():
    line_bytes = ser.readline()
    # If the line is not empty
    if len(line_bytes) > 20:
        # Convert the bytes into string
        try:
            line = line_bytes.decode('utf-8')
        except:
            line = "0,0,0,0,0,0,0,0,0,B,0"

        # Split the string using commas as separator, we get a list of strings
        str_values = line.split(',B')[0].split(',')
        print(str_values)

        values = [float(x) for x in str_values]

        values = [values]
        np.array(values).reshape(1, -1)
        predict(values)


while True:
    serial_to_property_values()
