# YOGI.SENSE

*A connected wheelchair to enable wheelchair users to exercise through a yoga guided session*.

## Current situation

Sitting in a wheelchair during long periods might increase problems of posture, blood circulation, and stiffness in the users’ body. There are so many reasons why yoga can be considered as a therapy for this kind of users, not to mention that it brings together mind and body, increasing well being in general. With this concept, posture is improved through different exercises which can also be considered as healing to joints and muscles avoiding stiffness and increasing blood circulation for wheelchair users. Furthermore, it must be noted that this practice link exercises with respiration, which will not just increase flexibility but also lung capacity.

## Concept description

This connected wheelchair guides the user through different yoga postures using nine FSR distributed along the seat and backrest; they sense the position of the user, this data is collected making possible a recognition of the different postures the user performs. This way the user will be provided by feedback depending on the prediction made by machine learning.

---

## What code is running where and for what purpose

### Code running in each device

* [Arduino Mega: Get FSR input and sent through the Serial port](/wheelchair3/docs/1_Mega_Code_Documentation.md)
* [Raspberry Pi: Predict yoga posture and send it through Bluetooth](/wheelchair3/docs/4_Pi_Code_Documentation.md)
* [Adafruit Feather: Read posture through Bluetooth and actuate](/wheelchair3/docs/5_Feather_Code_Documentation.md)

### Machine learning code

* [Collect and label](/wheelchair3/docs/2_Collect_and_label_Documentation.md)
* [Train and test](/wheelchair3/docs/3_Train_and_test_Documentation.md)


## Sensors

The following sensors will be used to verify if each posture made by the user.

* _**Force Sensing Resistor (FSR):**_ This sensor will be used to detect the different yoga postures of the user. The recommended setup is 5 FSR in the back rest and 6 FSR distributed in the bottom cushion.

* _**Time of flight distance sensor:**_ The sensor will determine the distance between the foot holders and the fit, detecting which foot is the user lifting. It can also be placed in the backseat to determine whether the user is bending the back forward or not.

* _**LDR Light sensor:**_ The LDR will be placed in the backseat to detect the users’ hand when performing postures such as the “twist” or “side stretch”.

## Actuators

To inform the user if they are performing the postures correctly, the following actuators will guide them through each exercise.

* _**LCD display 16x2:**_ A screen will be placed at the armrest to let the user know the name of the posture that needs to be performed. It will also help to keep track of the process by indicating the progress.

* _**LED:**_ The LEDs will be placed at the back part of the seat, next to the LDR light sensor acting as visual signals for the user to know where to look and place his or her hand.

* _**DC vibration motor:**_ A vibration motor placed on the seat and backseat will provide sensorial feedback to indicate where the pressure should be made with each posture.

![Cat pose](/docs/workshops/images/yoga/wheelchair-yoga-cat-pose.jpg)

![Cow pose](/docs/workshops/images/yoga/wheelchair-yoga-cow-pose.jpg)

![Eagle pose](/docs/workshops/images/yoga/wheelchair-yoga-eagle-pose.jpg)

![bend pose](/docs/workshops/images/yoga/wheelchair-yoga-forward-bend-pose.jpg)

![stretch pose](/docs/workshops/images/yoga/wheelchair-yoga-hip-stretch-pose.jpg)

![leg pose](/docs/workshops/images/yoga/wheelchair-yoga-leg-stretch-pose.jpg)

![side pose](/docs/workshops/images/yoga/wheelchair-yoga-side-stretch-pose.jpg)

![twist pose](/docs/workshops/images/yoga/wheelchair-yoga-twist-pose.jpg)

Images:
Smart Chair. (2015). Yoga for Wheelchair Users. Retrieved from https://kdsmartchair.com/blogs/news/58783043-yoga-for-wheelchair-users-8-poses



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
