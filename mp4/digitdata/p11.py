import numpy as np
import math

# initialize
weight = np.random.rand((10,28*28))
print (weight)

f_ti = open('digitdata/trainingimages', 'r');
f_tl = open('digitdata/traininglabels', 'r');
label_arr = []
count = 0
index = 0
temp = []

for line in f_tl:
	label_arr.append(int(line[0:-1]));

for line in f_ti:
	if(count < 28):
		temp.append(line[0:-1])
		count += 1;
	else:
		number = label_arr[index]
		# temp
		temp = np.array(temp)
		count = 0
		temp = []
		index += 1
		temp.append(line[0:-1])
		count += 1;