import numpy as np
import numpy.linalg as la
import matplotlib.pyplot as plt
import time

def calculate_distance(d1, d2):
    return la.norm(d1-d2, 2)

start = time.time()
f_ti = open('digitdata/trainingimages', 'r');
f_tl = open('digitdata/traininglabels', 'r');
label_arr = [] # training label
image_arr = [] # training image
count = 0
index = 0
temp = []
alpha = 1

for line in f_tl:
    label_arr.append(int(line[0:-1]));

for line in f_ti:
    if(count < 28):
        temp.append(line[0:-1])
        count += 1
    else:
        label = label_arr[index]
        new_temp = np.zeros(784, dtype=int)
        for i in range(28):
            for  j in range(28):
                if temp[i][j] != ' ':
                    new_temp[i*28+j] = 1
        image_arr.append(new_temp)
        count = 0
        temp = []
        index += 1
        temp.append(line[0:-1])
        count += 1

f_testi = open('digitdata/testimages', 'r');
f_testl = open('digitdata/testlabels', 'r');

label_arr_valid = [] # test label
test_arr = [] # test image
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
                if temp[i][j] != ' ':
                    new_temp[i*28+j] = 1
        test_arr.append(new_temp)
        ##
        count = 0
        index += 1
        temp = []
        temp.append(line[0:-1])
        count += 1

plot_x = np.arange(1,10)
plot_y = []

for k in range(3,4):
    print(k)
    correct = 0
    total = 999
    confusion_matrix = np.zeros((10,10))
    class_count = np.zeros(10)

    for i in range(999):
        test_image = test_arr[i]
        test_label = label_arr_valid[i]
        result = []

        # go through training data
        for j in range(len(label_arr)):
            training_image = image_arr[j]
            result.append(calculate_distance(test_image, training_image))

        # get k nearest
        result = np.array(result)
        k_nearest_index = result.argsort()[:k]

        # count occurance
        count_dict = {}
        for elem in k_nearest_index:
            if label_arr[elem] in count_dict:
                count_dict[label_arr[elem]] += 1
            else:
                count_dict[label_arr[elem]] = 1
        max_key = -1
        max_count = -1
        for key in count_dict.keys():
            if count_dict[key] > max_count:
                max_count = count_dict[key]
                max_key = key

        # valid
        if max_key == test_label:
            correct += 1

        class_count[test_label] += 1
        confusion_matrix[test_label][max_key] += 1

    for i in range(10):
        confusion_matrix[i] /= class_count[i]

    plot_y.append(correct/total)
    print(confusion_matrix)
    print(correct/total)
end = time.time()
print(end-start)
# plt.figure()
# plt.plot(plot_x,plot_y,"o")
# plt.show()

