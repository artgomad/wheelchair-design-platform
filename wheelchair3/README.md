## The yoga wheelchair

## Project description

Sitting in a wheelchair during long periods of time might increase problems of posture, blood circulation and stiffness in the users’ body. There are so many reasons why yoga can be considered as a therapy for this kind of users, not to mention that it brings together mind and body, increasing well being in general. Posture is improved through different exercises which can also be considered as healing to joints and muscles avoiding stiffness and increasing blood circulation. Furthermore, it must be noted that this practice links exercises with respiration, which will not just increase flexibility but also lung capacity.

# What code is running where and for what purpose

## Arduino Mega:

**Code:** FSR_reading_AllInOne.ino

This code reads the values of 9 FSR sensors and prints them in the Serial port. It also transforms the readings of a button into a switch variable, switching from true to false whenever the button is pressed.

The 9 FSR values and the switch value are printed in the Serial port in a single line. The 9 values are separated by comas among each other, and a ',B,' separates them from the switch value, which comes last in the line.

## **Raspberry Pi:**

---

Before running the final code, the algorithm was trained and tested: this required running the following two codes consecutively:

- 1_collect_and_label
- 2_train_and_test

The process of training and testing the algorithm will be explained in detail later

**Code:** 3_predict_and_Actuate.py

This code is a bit more complex, so its functionality will be decomposed for better understanding:
Some examples of yoga postures for wheelchair users:

### 1. Connects with the Data Centric Design Hub

Finds or creates two properties by name:

- Yoga Wheelchair: Property of type CLASS that will store the array of classes ('classes') declared at the beginning of the code.
- fsrYoga: Property of type NINE_DIMENSIONS that will store the values of the 9 FSR sensors

### 2. Reads the characteristics defined by the feather

And prints their UUID in the console

### 3. Reads the Serial Port uninterruptedly extracting the value of the 9 FSRs and the button in separate variables.

To know which port to read, its name has been defined in the environment file 'env.'

The information in the Serial port comes from the Arduino Mega in the form of a string of bytes. To be able to extract the sensor values, it is necessary to decompose the byte-string into an array of floats called 'values'. The last number of the byte-string (after the ,B,) will also be stored in a separate variable called 'button_value'.

### 4. Updates the values of the 'fsrYoga' property in the DCD Hub

The array of 9 floats 'values' is used to update the values of  'fsrYoga' property in the Hub, making possible to visualize the readings of the FSR sensors almost real time in Graffana. This was key for us because it allowed us to demonstrate during the exhibition how the algorithm makes his prediction depending on the FSR readings.

### 5. Starts Yoga Session

If the 'button_value' read from the Serial port becomes one for the first time the code will play the introductory audios and send via Bluetooth the value [1], which the Feather will interpret as the second posture of the array 'classes': 'Proper sitting'.

### 6. Uses the algorithm to predict the current posture of the user based on the readings of the 9 FSRs

The algorithm model has been previously saved in a file called 'model.pickle', and loaded at the beginning of the code. This model contains a lot of 9-dimension data points classified by postures. We will use the model to compare it with the current readings of the FSRs, obtaining a variable 'result' that contains the current position index predicted by the algorithm.

### 7. Updates the value of the 'Yoga Wheelchair' property in the DCD Hub

The property of type CLASS, apparently requires an array of a single integer as value.

prop_label.update_values([int(x)])

### 8. Check if the current posture equals the expected one without changing during 100 loop repetitions

The expected position will start in '1' and will keep increasing one by one every time the loop is executed. The reason for this loop is to send to the Feather the order of executing the next posture, instead of sending the current position continuously.

After 6 repetitions the expected posture and the current one won't be able to match as the 'currentPos' values range between 0 and 6 and the 'expectedPos' will have a value of 7, so the loop will stop executing. No more audio will be played or data sent by Bluetooth.

In case the user changes its posture, within the 100 loop repetitions, the counter will restart, delaying the transition to the next position.

### 9. Send the required position via Bluetooth to the Feather

The current position will only be sent to the Feather if it equals the expected one and keeps the same during 100 loop repetitions. In this way the Feather will only be informed when the indicated yoga posture has been performed without interruption for a while, considering then the posture completed, moving forward to the next one.

Actually, the posture sent won't be the current one but the expected one for the next exercise (list defined in the array 'classes') after the current one has been completed successfully (kept immutable during 100 loop repetitions).

To send it via Bluetooth, the next position is converted into a byte and written in the GATT_CHARACTERISTIC_POSTURE, which its UUID was previously declared in this code and defined by the Feather as a 'write' property (PROPERTIES=0x08).

### 10. Play audio, guiding the user to start with the next posture

Just after sending the next posture to the Feather, the Raspberry Pi will play the audio file corresponding to the next position in the list. While playing, the Pi only reproduces the audio and stops reading the Serial port, so the FSR values can't be visualized in Grafana at the same time. This could be solved in the future using threading.

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

## Setting up the GATT BLE Connexion
```
if ( !ble.begin(VERBOSE_MODE) ) {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }

  if ( FACTORYRESET_ENABLE ) {
    // Perform a factory reset to make sure everything is in a known state
    if ( ! ble.factoryReset() ) {
      error(F("Couldn't factory reset"));
    }
  }

  if ( !ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) ) {
    error( F("Callback requires at least 0.7.0") );
  }
```

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

## Research of possible sensors

__**Direction and speed:**__

* Rotatory Encoder: Pretty simple, but we need to connected in the same rotation axis than the wheel; with one part fixed to the wood while the other rotates with the wheel.

* Optical encoding (IR reflective sensor): The assembly of the hardware and the programming might be a bit more complex, but does the same thing as the rotatory encoder.

__**Pressure, weight and applied force detection:**__

* Force Sensing Resistor (FSR): Can be used to detect different postures of the user. The recommended setup is 5 FSR in the back rest and 6 FSR distributed in the bottom cushion.

* Load Cell (Strain Gauge): A bit more complex I think, its generally used to detect changes in weight, for example in a bathroom scale. It could be used to detect the weight progress of the wheelchair user

__**Wheelchair's orientation:**__

* Gyroscope: Can track wheel chairs rotation in three axis. This can detect where the user is pointing (maybe also looking by combining with the position detection system), detect changes on the slope, or detect if the chair overturns.

__**Acceleration:**__

* Accelerometer: Bump detection (Strong upwards inclination followed by downwards inclination)
                 Ramp detection (Strong upwards inclination over a medium distance)
                 Road condition detection (Frequency and intensity of the bumps)
                 Assisted drive detection?

__**Heart Rate (HR):**__

* Fitness tracking devices like fitBit or Apple Watch: They are only accurate for wheelchair users in low intensity tasks (below 100 bpm) [(reference)](https://www.resna.org/sites/default/files/conference/2016/outcomes/tsang.html). Diferent sensors allow to do this kind of tracking, like EKG sensors (Apple Watch) or PPG sensors.

As HR is difficult to track for Wheelchair users, even with a wristband, it is also very difficult to track the Energy Expenditure (EE), which calculation highly depends on the HR tracking.

__**Electrocardiogram (ECG):**__

* ECG sensor: Is able to detect Arrhythmias (AFib episodes) and therefore reduce the risk of strokes in people with decreased mobility. There are some fitness trackers with this capabilities implemented, like Withings Move ECG or Apple Watch.

__**Time:**__

* RTC module: Real time clock

__**Location:**__

* GPS module
* Connect to the GPS of the phone via Bluetooth
____
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
