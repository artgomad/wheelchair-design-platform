_**This example shows a basic way of collecting data with label. It prompts
the user with the class (e.g 'Proper Sitting', 'Forward Bend') to perform
and it associates the corresponding label to the recorded data.**_

---

It performs the following tasks:

## 1. Connects with the Data Centric Design Hub
Finds or creates two properties by name:

* Yoga Wheelchair: Property of type CLASS that will store the array of classes ('classes') declared at the beginning of the code.
```
prop_label = my_thing.find_or_create_property(LABEL_PROP_NAME, PropertyType.CLASS)

if prop_label.classes is None or len(prop_label.classes) == 0:
    prop_label.create_classes(CLASSES)
```
* fsrYoga: Property of type NINE_DIMENSIONS that will store the values of the 9 FSR sensors
```
prop_data = my_thing.find_or_create_property(DATA_PROP_NAME, PropertyType.NINE_DIMENSIONS)
```

## 2.Stars collecting data for the posture index 0.
```
collect(0)
```
It will collect 2000 samples ('MAX_SAMPLES') of the 9 FSR values read from the Serial port and upload them to the DCD Hub.

The information in the Serial port comes from the Arduino Mega in the form of a string of bytes. To be able to extract the sensor values, it is necessary to decompose the byte-string into an array of floats called 'values'. The last number of the byte-string (after the ,B,) wont be used in this code as it has no relevance for training the algorithm.

## 3. Updates the properties of the DCD Hub with each sample value and an assigned timestamp
First we get the current time in milliseconds and then update values of data and label properties (send them to the DCD Hub) with the same timestamp, so we can easily connect label and raw data later
```
current_ts_ms = int(round(time.time() * 1000))

prop_label.update_values([class_index], current_ts_ms)
prop_data.update_values(values, current_ts_ms)

```
## 3.Starts collecting data for the next posture
Once that the 2000 samples have been uploaded to the hub, it starts the collection of the samples for the next posture in the array classes.
```
collect(class_index + 1)
```
## 4. When all classes are covered, stops the program
```
if class_index >= len(CLASSES):
    print("Data collection done.")
    exit()
```
