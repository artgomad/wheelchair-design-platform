_**This Code Uses Machine Learning to generate a model that will be used to compare with the FSR readings in real-time, allowing the Raspberry Pi to infer the current posture of the wheelchair user.**_

---

## 1. Connects with the Data Centric Design Hub and finds its properties by name
We only want to use the data that has been properly labelled with the previous code [(Collect and label)](/wheelchair3/docs/2_Collect_and_label_Documentation.md), so we will previously define the data collection timeframe. This is defined as an Epoch timestamp; it is important to use the local time, not the GTM.
```
# Epoch timestamp in milliseconds of the start of the collect_and_label code
START_TS = 1554281940000

# Approximated Epoch timestamp in milliseconds of the end of the data collection process
END_TS = 1554281940000+5000000
```
First we assign the 9-dimension property of the DCD Hub to the variable 'fsr', and read only the values in the relevant timeframe:
```
fsr = my_thing.find_property_by_name("fsrYoga")

fsr.read(START_TS, END_TS)
```
And we do the same with the 'Yoga Wheelchair' property and the variable 'sitting':
```
sitting = my_thing.find_property_by_name("Yoga Wheelchair")
sitting.read(START_TS, END_TS)
```

To be able to corelate the data to its label we need to align both properties:
```
sitting.align_values_to(fsr)
```
Then we can extract the values of each property in two new arrays:
```
data = fsr.values
label = sitting.values
```

## 2. Preparing the data set
The data and label arrays will be decomposed in several subarrays, in order to minimize the biases of a limited data set in machine learning algorithm.
Each sub-set of data will have a different usage: test, cross-validate and train.
```
for index in range(len(data)):
    # remove time
    data[index].pop(0)
    label[index].pop(0)
    if index%5 == 0: # Todos los multiplos de 5 los meto a test_data
        # 20% to test data
        test_data.append(data[index])
        test_label.append(label[index])
    else:
        # 80% leftover data
        leftover_data.append(data[index])
        leftover_label.append(label[index])

for index in range(len(leftover_data)):
    if index%4 == 0:
        # 20% to cross validate
        cv_data.append(leftover_data[index])
        cv_label.append(leftover_label[index])
    else:
        # 60% to train
        train_data.append(leftover_data[index])
        train_label.append(leftover_label[index])
```
