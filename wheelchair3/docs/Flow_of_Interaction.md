# Working Principle

### Flow of interaction of a yoga session with Yogi.Sense

The user presses the button whenever she or he wants to start a yoga session. The voice of the instructor welcomes the user and explains the first part of the session, which is a respiration meditation guided by different vibration motors distributed along the seat and backrest. The speaker plays the voice of the instructor explaining the first posture and the corresponding image of the position lights up in the simulated screen composed by illustrations of the different poses and LEDs. Succeeding, the FSR sensors start sensing the data depending on how the user is sitting. If the posture is correct, the session continues with the next posture. When the user finishes doing the last pose, the speaker closes the session.


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

---

![Working Principle](/wheelchair3/docs/images/WorkingPrinciple.png)
