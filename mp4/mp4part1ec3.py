import numpy as np
import math
import random
from sklearn import svm,metrics
import matplotlib.pyplot as plt
from sklearn.model_selection import GridSearchCV
# initialize







weight = np.random.rand(10,784)

f_ti = open('trainingimages', 'r');
f_tl = open('traininglabels', 'r');
label_arr = []
image_arr = []
count = 0
index = 0
temp = []
alpha = 0.01
# c_l = 0
# c_i = 0
for line in f_tl:
    label_arr.append(int(line[0:-1]));
#    c_l += 1


for line in f_ti:
#    c_i += 1
    if(count < 28):
        temp.append(line[0:-1])
        count += 1;
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
        count += 1;
print(np.shape(image_arr),np.shape(label_arr))

#----grid search
range_gamma= np.outer(np.logspace(-3, 0, 4),np.array([1,5]))
range_gamma = range_gamma.flatten()
C_ = np.outer(np.logspace(-1, 1, 3),np.array([1,5]))
C_ = C_.flatten()

p = {'kernel':['rbf'], 'C':C_, 'gamma': range_gamma}

svm_ = svm.SVC()
grid_temp = GridSearchCV(estimator=svm_,param_grid=p,n_jobs=1, verbose=2)
grid_temp.fit(image_arr, label_arr)
sorted(grid_temp.cv_results_.keys())
#clr= grid_temp.best_estimator_
params = grid_temp.best_params_
scores = grid_temp.cv_results_['grade'].reshape(len(C_),len(range_gamma))
#gridsearch end


clf = svm.SVC(gamma = 0.01,C = 5)
clf.fit(image_arr,label_arr)

f_testi = open('testimages', 'r');
f_testl = open('testlabels', 'r');


test_label_arr = []
test_image_arr = []
count = 0
index = 0
temp = []
total = 0
correct = 0

for line in f_testl:
    test_label_arr.append(int(line[0:-1]));

for line in f_testi:
#    c_i += 1
    if(count < 28):
        temp.append(line[0:-1])
        count += 1;
    else:
        label = label_arr[index]
        new_temp = np.zeros(784, dtype=int)
        for i in range(28):
            for  j in range(28):
                if temp[i][j] != ' ':
                    new_temp[i*28+j] = 1
        test_image_arr.append(new_temp)
        count = 0
        temp = []
        index += 1
        temp.append(line[0:-1])
        count += 1;

correct_arr = clf.predict(test_image_arr)
correct = 0
total_count = 0
for i in range(len(test_label_arr)):
    total_count += 1
    if test_label_arr[i] == correct_arr[i] :
        correct += 1
print(correct)
print(total_count)
print metrics.confusion_matrix(correct_arr,test_label_arr)