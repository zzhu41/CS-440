import numpy as np
import math
import random
import matplotlib.pyplot as plt
import time
import matplotlib as mpl

# initialize
# weight = np.random.rand(10,784)
# weight = np.zeros((10,784))
start = time.time()

f_ti = open('trainingimages', 'r');
f_tl = open('traininglabels', 'r');
label_arr = []
image_arr = []
count = 0
index = 0
temp = []
alpha = 1
n = 2
for line in f_tl:
    label_arr.append(int(line[0:-1]));

for line in f_ti:
    if(count < 28):
        temp.append(line[0:-1])
        count += 1;
    else:
        label = label_arr[index]
        new_temp = np.zeros(729, dtype=int)
        for i in range(28-(n-1)):
            for j in range(28-(n-1)):
                for x in range(n):
                    for y in range(n):
                        #wlb += temp[i+x][j+y]
                        if temp[i+x][j+y] == '#' :
                            new_temp[i*(28-(n-1))+j] += 2
                        if temp[i+x][j+y] == '+' :
                            new_temp[i*(28-(n-1))+j] += 1
        image_arr.append(new_temp)
        count = 0
        temp = []
        index += 1
        temp.append(line[0:-1])
        count += 1;

f_testi = open('testimages', 'r');
f_testl = open('testlabels', 'r');

label_arr_valid = []
test_arr = []
count = 0
index = 0
temp = []

for line in f_testl:
    label_arr_valid.append(int(line[0:-1]));

for line in f_testi:
    if(count < 28):
        temp.append(line[0:-1])
        count += 1;
    else:#get an image
        label = label_arr_valid[index]
        new_temp = np.zeros(784, dtype=int)
        for i in range(28):
            for  j in range(28):
                if temp[i][j] == '#':
                    new_temp[i*28+j] = 2
                if temp[i][j] == '+':
                    new_temp[i*28+j] = 1
        test_arr.append(new_temp)
        ##
        count = 0
        index += 1
        temp = []
        temp.append(line[0:-1])
        count += 1;

training_curve_x = np.arange(1,101)
training_curve_y = []
confusion_matrix = np.zeros((10,10))
total_count_each_class = np.zeros(10)

for k in range(100,101):
    weight = np.random.rand(10,729)
    correct = 0
    total = 0
    for i in range(k):
        training_correct = 0
        training_total = 0
        for j in range(len(image_arr)):
            image = image_arr[j]
            label = label_arr[j]
            # predict
            c = np.argmax(np.dot(weight,image))
            # print(c)
            if label != c:
                weight[label] += alpha*image
                weight[c] -= alpha*image
            else:
                training_correct += 1
            training_total += 1
        training_curve_y.append(training_correct/training_total)

    for i in range(999):
        label = label_arr_valid[i]
        test_image = test_arr[i]
        test_ma = np.zeros(729)
        j = 0
        for i in range(28*28):
            if (((i+1)%28) != 0) and i <756:
                if test_image[i] == 2 :
                            test_ma[j] += 2
                if test_image[i] == 1 :
                            test_ma[j] += 1
                if test_image[i+1] == 2 :
                            test_ma[j] += 2
                if test_image[i+1] == 1 :
                            test_ma[j] += 1
                if test_image[i+28] == 2 :
                            test_ma[j] += 2
                if test_image[i+28] == 1 :
                            test_ma[j] += 1
                if test_image[i+28+1] == 2 :
                            test_ma[j] += 2
                if test_image[i+28+1] == 1 :
                            test_ma[j] += 1
                j += 1
        print(j)

        print(weight)
        print(test_ma)

        result = np.dot(weight,test_ma)
        print(result)
        classified = np.argmax(result)
        if label == classified:
            correct += 1
        total += 1
        total_count_each_class[label] += 1
        confusion_matrix[label][classified] += 1

for i in range(10):
    confusion_matrix[i] /= total_count_each_class[i]

print(confusion_matrix)
print(correct,total)

end = time.time()
print(end-start)
# plt.figure()
# plt.plot(training_curve_x, training_curve_y)
# plt.xlabel("number of epoch")
# plt.ylabel("accuracy")
# plt.title("training curve")
# plt.show()


# plt.title("weight image for 7")
# plt.imshow(np.reshape(weight[7],(28,28)))
# plt.colorbar(orientation='vertical', cmap=mpl.cm.jet)

# plt.show()