_**This Code Uses Machine Learning to generate a model that will be used to compare with the FSR readings in real-time, allowing the Raspberry Pi to infer the current posture of the wheelchair user.**_

---

It performs the following tasks:

## 1. Connect with the Data Centric Design Hub and finds its properties by name
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
* 20% to the test data
* 20% to cross-validate data
* 60% to the train data

## 3. Train a k-Nearest Neighbour (kNN) algorithm
In k-NN classification, the output is a class membership, a class from the label array in our case. Each sample is classified by a plurality vote of its neighbors, with the object being assigned to the class most common among its nearest neighbors.
```
neigh = KNeighborsClassifier(n_neighbors=1)
neigh.fit(train_data, train_label)
```
## 4. Use the test data to evaluate the algorithm
We will use the 'neigh' algorithm to predict the class (label) of the cv_data dataset (cross-validation data array).
```
predicted = neigh.predict(cv_data)
cvLabel = numpy.array(cv_label)
result = accuracy_score(cvLabel, predicted)
print("cv accuracy: {}".format(result))
```
If the accuracy of the cross-validation data is higher than 0.8, then it repeat the process with the test data, and print the evaluation of the algorithm in a Confusion Matrix
```
if result > 0.8:
    print("Validation passed. Displaying testing performance")
    predicted = neigh.predict(test_data)
    testLabel = numpy.array(test_label)
    result = accuracy_score(testLabel, predicted)
    print("test accuracy: {}".format(result))
    print(classes)

    generate_confusion_matrix(testLabel)
    print(classification_report(testLabel, predicted, target_names=classes))
```
## 5. Save the model in a file
To conclude, the model of the k-NN classification ('neigh') is saved in the file 'pickle', which will be retrieved by the code [3_Predict_and_Actuate.py](/wheelchair3/docs/4_Pi_Code_Documentation.md) to predict the current posture from the 9 FSR readings.
```
with io.open(MODEL_FILE_NAME, "wb") as file:
    pickle.dump(neigh, file, protocol=2)
```
