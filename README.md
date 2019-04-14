# YOGI.SENSE

*A connected wheelchair to enable wheelchair users to exercise through a yoga guided session*.

### Current situation

Sitting in a wheelchair during long periods might increase problems of posture, blood circulation, and stiffness in the users’ body. There are so many reasons why yoga can be considered as a therapy for this kind of users, not to mention that it brings together mind and body, increasing well being in general. With this concept, posture is improved through different exercises which can also be considered as healing to joints and muscles avoiding stiffness and increasing blood circulation for wheelchair users. Furthermore, it must be noted that this practice link exercises with respiration, which will not just increase flexibility but also lung capacity.

### Concept description

This connected wheelchair guides the user through different yoga postures using nine FSR distributed along the seat and backrest; they sense the position of the user, this data is collected making possible a recognition of the different postures the user performs. This way the user will be provided by feedback depending on the prediction made by machine learning.

### Flow of interaction of a yoga session with Yogi.Sense

The user presses the button whenever she or he wants to start a yoga session. The voice of the instructor welcomes the user and explains the first part of the session, which is a respiration meditation guided by different vibration motors distributed along the seat and backrest. The speaker plays the voice of the instructor explaining the first posture and the corresponding image of the position lights up in the simulated screen composed by illustrations of the different poses and LEDs. Succeeding, the FSR sensors start sensing the data depending on how the user is sitting. If the posture is correct, the session continues with the next posture. When the user finishes doing the last pose, the speaker closes the session.

![Working Principle](/docs/workshops/images/finalR/WorkingPrinciple.png)

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
4. Raspberry Pi transfers the data values to the DCD Hub (using a WiFi connection), which allows us to visualise and track the different postures in Grafana.

![Working Principle](/docs/workshops/images/finalR/PosturesAndGrafana.png)

5. At the same time, the Raspberry Pi processes the data with a machine learning algorithm to predict the current position.
6. The algorithm’s outcome is sent through Bluetooth to Adafruit Feather.
7. The Adafruit Feather transmits a signal to the three actuators:
   - Speaker to guide the user with instructions on how to do the postures.
   - LED ring to show the current posture.
   - Three vibration motors placed in both the seat and backrest to set the breathing rhythm during the whole session.

![Working Principle](/docs/workshops/images/finalR/SystemArch.png)

## What code is running where and for what purpose

### Code running in each device

* [Arduino Mega: Get FSR input and sent through the Serial port](/wheelchair3/docs/1_Mega_Code_Documentation.md)
* [Raspberry Pi: Predict yoga posture and send it through Bluetooth](/wheelchair3/docs/4_Pi_Code_Documentation.md)
* [Adafruit Feather: Read posture through Bluetooth and actuate](/wheelchair3/docs/5_Feather_Code_Documentation.md)

### Machine learning code

* [Collect and label](/wheelchair3/docs/2_Collect_and_label_Documentation.md)
* [Train and test](/wheelchair3/docs/3_Train_and_test_Documentation.md)



_________________________________________________________________________________________________
## Wheelchair Design Platform

Wheelchair Design Platform is a repository that contains some resources to help
designers and developers speak the same language, and work together towards
addressing relevant challenges for wheelchair users. It is a collection of
workshop materials, code examples and also a compilation of resources to foster
a prospering research and design community around wheelchair users.


![IoT1 Exhibition](/docs/workshops/images/iot1_exhibition.jpg)

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

## Main Components

__**Disclaimer:**__ the design of this platform focuses on flexibility and
technology exploration rather than optimisation.

The main design includes a Raspberry Pi 3 and an Arduino Mega 2560 on the wheelchair frame.

The Arduino Mega is the micro-controller of the platform. Fixed on the main frame of the wheelchair,
it can collect data from sensors (e.g. force sensors, accelerometers), and trigger actions from actuators
(e.g. LEDs, vibration motors).

More on the Arduino Mega can be found [here](/docs/resources/arduino.md "Arduino resources").

Raspberry Pi is a small computer. It is also fixed to the main frame of the wheelchair,
where it can:
* interact with the Arduino Mega via USB to receive data and transmit commands;
* interact with the Internet to transmit commands and receive data;
* store data locally in files;
* run (machine learning) algorithms.

More on the Raspberry Pi can be found [here](/docs/resources/raspberrypi.md "Raspberry Pi resources").

These components fit together as shown on the following diagram. A large powerbank
powers the Raspberry Pi. The Arduino Mega communicates and receives power from the
Raspberry Pi via USB. A Feather (Arduino-like development board) on the wheel connects to
the Raspberry Pi via Bluetooth to sense and actuate from the wheel.

![Main Wheelchair components](/docs/workshops/images/wheechair-components.png)

## List of suggested components:

On the frame:

* 1 Raspberry Pi 3B;
* 1 SD card (Some come directly with NOOBS installed);
* 1 Arduino Mega;
* 1 Large power bank;
* 1 large breadboard;
* 1 USB cable A/micro (Powerbank to Raspberry Pi);
* 1 USB cable A/B (Raspberry Pi to Arduino Mega).

On the wheel:

* 1 Feather (Bluetooth enabled);
* 1 small power bank;
* 1 small breadboard;
* 1 USB cable A/B (power bank to Arduino Uno).


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
