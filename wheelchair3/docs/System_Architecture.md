# System Architecture

## Sensors

* _**Button:**_
The start button will be placed in the armrest. The user can press it to initiate the yoga session.

* _**Force Sensing Resistor (FSR):**_
These sensors will be used to detect the different yoga postures of the user. The recommended setup is 3 FSR in the backrest and 6 FSR distributed in the bottom cushion.

## Actuators

* _**NeoPixel LED Ring - 16:**_
It will be attached in the left side of the left armrest and will show the current posture as well as the progress achieved.

* _**Speaker:**_
The instructions reproduced by the speaker will guide the user through the different yoga postures and will give feedback in case the position is incorrect.

* _**DC Vibration motors:**_
Two vibration motors will be placed on the backseat and 1 in the seat to provide with guidance for breathing during the whole session.

## Microcontrollers

* _**Arduino MEGA:**_
It’s located in the wheelchair mainframe and it is connected to: Raspberry Pi (USB B cable), LED (no cables, used to verify connection), Breadboard with a button (wires), Breadboard with FSR sensors (wires).

* _**Adafruit Feather Bluefruit 32u4:**_
It is located in a breadboard on the wheelchair mainframe, and it is connected to: DC vibration motors (wires), NeoPixel ring (wires), Powerbank (MICRO USB cable).

## Microprocessor

* _**Raspberry Pi:**_
It’s attached to the wheelchair mainframe, and it is connected to: Arduino MEGA (USB B cable), Speaker (USB cable), Powerbank (MICRO USB cable).

![Working Principle](/wheelchair3/docs/images/SystemArch.png)
![Working Principle](/wheelchair3/docs/images/arduinoPi.jpg)
![Working Principle](/wheelchair3/docs/images/featherC.jpg)
