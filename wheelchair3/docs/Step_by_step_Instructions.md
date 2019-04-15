# Step-by-step instructions

1. Download the following files from github. You can find them [here.](/wheelchair3/)
   - Input_FSR_to_Serial_MEGA.ino
   - 1_collect_and_label.py
   - 2_train_and_test.py
   - 3_predict_and_Actuate.py
   - Read_Bluetooth_and_Actuate_FEATHER.ino
2. Download and install the Libraries.
To see how to install the libraries in Arduino click [here.](https://www.arduino.cc/en/guide/libraries)
To install dependencies in Python click [here.](https://datacentricdesign.github.io/wheelchair-design-platform/workshops/Workshop1)(point 3.1)
To see how to install libraries in Python click [here.](https://datacentricdesign.github.io/wheelchair-design-platform/workshops/Workshop1) (point 5.4)
   - In Arduino
      - SPI.h
      - Adafruit_BLE.h
      - Adafruit_BluefruitLE_SPI.h
      - Adafruit_BluefruitLE_UART.h
      - BluefruitConfig.h
      - FastLED.h
   - In Python
      - os
      - serial
      - time
      - io
      - math
      - sklearn
      - pandas
      - numpy
      - pickle
3. Upload Input_FSR_to_Serial_MEGA.ino to the Arduino Mega.
4. Upload Read_Bluetooth_and_Actuate_FEATHER.ino to the Adafruit Feather.
5. Upload to the Raspberry Pi:
   - 1_collect_and_label.py
   - 2_train_and_test.py
   - 3_predict_and_Actuate.py
6. Attach Arduino Mega to the wheelchair main frame.
7. Attach Adafruit Feather to the main frame.
8. Attach Power Bank to the mainframe.
9. Attach Raspberry Pi to the wheelchair mainframe.
10. Attach LED ring to the left armrest.
11. Attach button to the right armrest.
12. Attach one vibrator motor to the sit and two to the backrest of the wheelchair as shown in the image:

![Working Principle](/wheelchair3/docs/images/vibrations.png)

13. Attach the FSR to the wheelchair as it is shown in the image:

![Working Principle](/wheelchair3/docs/images/fsr.png)

14. Connect the wires between the Arduino Mega and the nine Force Sensing Resistors as the image illustrates:

![Working Principle](/wheelchair3/docs/images/arduinoPi.jpg)

15. Connect the wires between the Arduino Mega and the button according to the image shown in step 14.
16. Connect the speaker to the Raspberry Pi according to the image shown in step 14.
17. Connect the Arduino Mega and Raspberry Pi using the a USB B cable according to the image shown in step 14.
18. Connect the Raspberry Pi and Powerbank using a MICRO USB cable according to the image shown in step 14.
19. Connect the wires between the Feather and the LED ring according to the image:

![Working Principle](/wheelchair3/docs/images/featherC.jpg)

20. Connect the wires between the Adafruit Feather and the 3 DC Vibration Motors according to the image shown in step 19.
21. Connect the Adafruit Feather and Powerbank using a Micro USB cable according to the image shown in step 19.
22. Bluetooth connection settings. You can find them [here.](https://github.com/datacentricdesign/wheelchair-design-platform/blob/master/docs/workshops/Workshop2.md#3-bluetooth-low-energy-ble-gatt-service)
23. DCD connection settings (point 1.6). You can find them [here.](https://github.com/datacentricdesign/wheelchair-design-platform/blob/master/docs/workshops/Workshop1.md#2-data-centric-design-hub-per-group)
24. Grafana connection settings (point 3.5). You can find them [here.](https://github.com/datacentricdesign/wheelchair-design-platform/blob/master/docs/workshops/Workshop1.md#2-data-centric-design-hub-per-group)
25. Find the Raspberry Pi on your network.
26. Connect to the Raspberry Pi using ssh.
27. Execute the code 3_predict_and_Actuate.py
28. Read values from the Grafana webpage.
