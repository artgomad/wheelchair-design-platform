# Raspberry Pi code: Predict yoga posture and actuate

**Code Name:** 3_predict_and_Actuate.

_**This Code uses the an algorithm previously trained to classify the readings of 9 FSR sensor into 7 posture labels. The FSR readings come from the Serial port, and are updated to the DCD Hub in order to be able to visualize its values in Grafana. Besides, the code uses the predicted posture to play different audio files and sends this posture via Bluetooth using a GATT characteristic.**_

Before running the final code, the algorithm needs to be trained and tested: this required running the following two codes consecutively:

* [1. Collect and label](/wheelchair3/docs/2_Collect_and_label_Documentation.md)
* [2. Train and test](/wheelchair3/docs/3_Train_and_test_Documentation.md)

---

It performs the following tasks:

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
