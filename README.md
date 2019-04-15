# YOGI.SENSE

*A connected wheelchair to enable wheelchair users to exercise through a yoga guided session*.


* [Exhibition pictures](/wheelchair3/docs/images/Exhibition_pictures.md)
* [Sensors and actuators](/wheelchair3/docs/System_Architecture.md)
* [Flow of interaction](/wheelchair3/docs/Flow_of_Interaction.md)


### Current situation

Sitting in a wheelchair during long periods might increase problems of posture, blood circulation, and stiffness in the users’ body. There are so many reasons why yoga can be considered as a therapy for this kind of users, not to mention that it brings together mind and body, increasing well being in general. With this concept, posture is improved through different exercises which can also be considered as healing to joints and muscles avoiding stiffness and increasing blood circulation for wheelchair users. Furthermore, it must be noted that this practice link exercises with respiration, which will not just increase flexibility but also lung capacity.

### Concept description

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

---

## Poster and video

To see a more detailed explanation of the location of the sensors and actuators, we provide you with a video and a poster.

To see the video click [here.](https://youtu.be/LGbsHrdyxcA)

![Working Principle](/wheelchair3/docs/images/posterfinal.jpg)


_________________________________________________________________________________________________
# Developing Connected Products and Services for the internet of Things

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
