# YOGI.SENSE

*A connected wheelchair to enable wheelchair users to exercise through a yoga guided session*.

### Current situation

Sitting in a wheelchair during long periods might increase problems of posture, blood circulation, and stiffness in the users’ body. There are so many reasons why yoga can be considered as a therapy for this kind of users, not to mention that it brings together mind and body, increasing well being in general. With this concept, posture is improved through different exercises which can also be considered as healing to joints and muscles avoiding stiffness and increasing blood circulation for wheelchair users. Furthermore, it must be noted that this practice link exercises with respiration, which will not just increase flexibility but also lung capacity.

### Concept description

This connected wheelchair guides the user through different yoga postures using nine FSR distributed along the seat and backrest; they sense the position of the user, this data is collected making possible a recognition of the different postures the user performs. This way the user will be provided by feedback depending on the prediction made by machine learning.

### Flow of interaction of a yoga session with Yogi.Sense

The user presses the button whenever she or he wants to start a yoga session. The voice of the instructor welcomes the user and explains the first part of the session, which is a respiration meditation guided by different vibration motors distributed along the seat and backrest. The speaker plays the voice of the instructor explaining the first posture and the corresponding image of the position lights up in the simulated screen composed by illustrations of the different poses and LEDs. Succeeding, the FSR sensors start sensing the data depending on how the user is sitting. If the posture is correct, the session continues with the next posture. When the user finishes doing the last pose, the speaker closes the session.

![Working Principle](/wheelchair3/docs/images/WorkingPrinciple.png)

---

## System Architecture

### Sensors

* _**Button:**_
The start button will be placed in the armrest. The user can press it to initiate the yoga session.

* _**Force Sensing Resistor (FSR):**_
These sensors will be used to detect the different yoga postures of the user. The recommended setup is 3 FSR in the backrest and 6 FSR distributed in the bottom cushion.

### Actuators

* _**NeoPixel LED Ring - 16:**_
It will be attached in the left side of the left armrest and will show the current posture as well as the progress achieved.

* _**Speaker:**_
The instructions reproduced by the speaker will guide the user through the different yoga postures and will give feedback in case the position is incorrect.

* _**DC Vibration motors:**_
Two vibration motors will be placed on the backseat and 1 in the seat to provide with guidance for breathing during the whole session.

### Microcontrollers

* _**Arduino MEGA:**_
It’s located in the wheelchair mainframe and it is connected to: Raspberry Pi (USB B cable), LED (no cables, used to verify connection), Breadboard with a button (wires), Breadboard with FSR sensors (wires).

* _**Adafruit Feather Bluefruit 32u4:**_
It is located in a breadboard on the wheelchair mainframe, and it is connected to: DC vibration motors (wires), NeoPixel ring (wires), Powerbank (MICRO USB cable).

### Microprocessor

* _**Raspberry Pi:**_
It’s attached to the wheelchair mainframe, and it is connected to: Arduino MEGA (USB B cable), Speaker (USB cable), Powerbank (MICRO USB cable).

### Flow of data

1. The button is pressed and read by Arduino.
2. The nine FSR on the seat and backrest start sensing the posture.
3. Arduino transmits the data to the Raspberry Pi
4. Raspberry Pi transfers the data values to the DCD Hub (using a WiFi connection), which allows us to visualise and track the different postures in Grafana. (Each of the lines in the first chart represents an FSR value)

![Working Principle](/wheelchair3/docs/images/PosturesAndGrafana.png)

5. At the same time, the Raspberry Pi processes the data with a machine learning algorithm to predict the current position.
6. The algorithm’s outcome is sent through Bluetooth to Adafruit Feather.
7. The Adafruit Feather transmits a signal to the three actuators:
   - Speaker to guide the user with instructions on how to do the postures.
   - LED ring to show the current posture.
   - Three vibration motors placed in both the seat and backrest to set the breathing rhythm during the whole session.

![Working Principle](/wheelchair3/docs/images/SystemArch.png)

---

## Step-by-step instructions

1. Download the following files from github. You can find them [here.](/wheelchair3/)
   - Input_FSR_to_Serial_MEGA.ino
   - 1_collect_and_label.py
   - 2_train_and_test.py
   - 3_predict_and_Actuate.py
   - Read_Bluetooth_and_Actuate_FEATHER.ino
2. Download and install the Libraries.
To see how to install the libraries in Arduino click [here.](how to install arduino libraries in pr…)
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

---

## What code is running where and for what purpose

### Code running in each device

* [Arduino Mega: Get FSR input and sent through the Serial port](/wheelchair3/docs/1_Mega_Code_Documentation.md)
* [Raspberry Pi: Predict yoga posture and send it through Bluetooth](/wheelchair3/docs/4_Pi_Code_Documentation.md)
* [Adafruit Feather: Read posture through Bluetooth and actuate](/wheelchair3/docs/5_Feather_Code_Documentation.md)

### Machine learning code

* [Collect and label](/wheelchair3/docs/2_Collect_and_label_Documentation.md)
* [Train and test](/wheelchair3/docs/3_Train_and_test_Documentation.md)

---

## Poster and video

To see a more detailed explanation of the location of the sensors and actuators, we provide you with a video and a poster.

To see the video click [here.](https://youtu.be/LGbsHrdyxcA)

![Working Principle](/wheelchair3/docs/images/posterfinal.jpg)

---

## Exhibition pictures

![Working Principle](/wheelchair3/docs/images/20190408_095746.jpg)

![Working Principle](/wheelchair3/docs/images/20190408_095825.jpg)

![Working Principle](/wheelchair3/docs/images/20190408_095846.jpg)

![Working Principle](/wheelchair3/docs/images/20190408_105127.jpg)

![Working Principle](/wheelchair3/docs/images/20190408_105216.jpg)

![Working Principle](/wheelchair3/docs/images/20190408_110552.jpg)

![Working Principle](/wheelchair3/docs/images/20190408_111807.jpg)

_________________________________________________________________________________________________
# Wheelchair Design Platform

Wheelchair Design Platform is a repository that contains some resources to help
designers and developers speak the same language, and work together towards
addressing relevant challenges for wheelchair users. It is a collection of
workshop materials, code examples and also a compilation of resources to foster
a prospering research and design community around wheelchair users.


## Workshops

* [Getting started](/docs/workshops/GettingStarted.md)
* [Workshop 1: Building an Internet-Connected Wheelchair](/docs/workshops/Workshop1.md)
* [Workshop 2: Integrating and Visualising Sensor-Based Data](/docs/workshops/Workshop2.md)
* [Workshop 3: Developing Algorithms and Controlling Actuators](/docs/workshops/Workshop3.md)
* [Workshop 4: Developing and Conducting a Data Collection Campaign](/docs/workshops/Workshop4.md)
* [Workshop 5: Implementing a Machine Learning Pipeline](/docs/workshops/Workshop5.md)
* [Workshop 6: Developing a Product Analytics Dashboard](/docs/workshops/Workshop6.md)

## Resources

* This platform uses two programming languages, Python on computers and C on
micro-controllers. While descriptions and examples of code should help you
get started, you can find some additional resources
[here](/docs/resources/software.md "Python and C resources").

* Documentation of your project is key,
[here are some tips and examples](/docs/resources/documentation.md "Documentation tips and examples").

* [Git manipulation such as Pull Request](/docs/resources/git.md "Git manipulation").



## Contact and Existing projects

* [The hiking wheelchair](https://github.com/cprecioso/wheelchair-design-platform)
* [The EDU wheelchair](https://github.com/ctsai-1/wheelchair-design-platform)
* [Weelchair tracking for basketball players](https://github.com/FabianIDE/wheelchair-design-platform)
* [Disco Wheelchair](https://github.com/MatthijsBrem/wheelchair-design-platform)
* [Wheelchair Madness 2222](https://github.com/pherkan/wheelchair-design-platform/tree/master/wheelchair)
* [Who is sitting?](https://github.com/Rosanfoppen/wheelchair-design-platform/tree/master/wheelchair)
* [Magic Wheelchair](https://github.com/Yuciena/wheelchair-design-platform)
* [Yoga Wheelchair](https://github.com/artgomad/wheelchair-design-platform)


Feel free to contact us at jacky@datacentricdesign.org. We welcome feedback, pull requests
or links to your project.
