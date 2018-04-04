import numpy as np
import math
import os

# 0 stands for no, 1 stands for yes

k = 1;
data_dict = {} # 0 for no, 1 for yes
yes_count = 0
no_count = 0
for filename in os.listdir('./txt_yesno/training'):
	data = []
	with open("./txt_yesno/training/" + filename) as f:
		data = f.read().splitlines()
	f.close()
	for i in range(25):
		data[i] = list(data[i])
	class_array = filename[0:15].split('_')
	i = 0
	local_count = 0
	data = np.array(data)
	while(i < 150):
		column = data[0:25,i]
		if np.count_nonzero(column == ' ') >= 2:
			this_class = int(class_array[local_count])
			this_data = data[0:25, i:(i+10)]
			if this_class == 0:
				no_count += 1
			else:
				yes_count += 1
			if this_class in data_dict:
				data_dict[this_class].append(this_data)
			else:
				data_dict[this_class] = [this_data]
			local_count += 1
			i += 10
			if(local_count == 8):
				break
		else:
			i += 1

## calculate possibility
# priors
class_possibility = [] # 0 for no, 1 for yes
class_possibility.append(no_count/(no_count+yes_count))
class_possibility.append(yes_count/(no_count+yes_count))

# conditional possibility
trained = []
for i in range(2):#For no and yes
	num_trained = {}#Dict for current class, (x, y, pixelValue):Occurance Time
	#Init the num_trained
	for x_idx in range(25):
		for y_idx in range(10):
			for ele in [' ', '%']:
				num_trained[(x_idx, y_idx, ele)] = k; # laplace
	for j in range(len(data_dict[i])):
		num_dict = data_dict[i][j]
		for x_idx in range(25):
			for y_idx in range(10):
				obj = (x_idx, y_idx, data_dict[i][j][x_idx][y_idx])
				num_trained[obj] += 1;
	for key in num_trained.keys():
		num_trained[key] = (num_trained[key]) / float(len(data_dict[i]));
	trained.append(num_trained);

# print(trained)
#Validate
correct_count_yes = 0
total_count_yes = 0

for filename in os.listdir('./txt_yesno/yes_test'):
	with open("./txt_yesno/yes_test/" + filename) as g:
		temp = g.read().splitlines()
	g.close()
	for i in range(25):
		temp[i] = list(temp[i])
	result = []
	for k in range(2):
		curr_result = math.log2(class_possibility[k]);
		for i in range(25):
			for j in range(10):
				temp_dict = trained[k]
				curr_result = curr_result + math.log2(temp_dict[(i,j,temp[i][j])])
		result.append(curr_result);
	result_num = np.argmax(result);
	true_num = 1
	total_count_yes += 1
	if (result_num == true_num):
		correct_count_yes += 1


correct_count_no = 0
total_count_no = 0

for filename in os.listdir('./txt_yesno/no_test'):
	with open("./txt_yesno/no_test/" + filename) as f:
		temp = f.read().splitlines()
	f.close()
	for i in range(25):
		temp[i] = list(temp[i])
	temp = np.array(temp)
	result = []
	for k in range(2):
		curr_result = math.log2(class_possibility[k]);
		for i in range(25):
			for j in range(10):
				temp_dict = trained[k]
				curr_result = curr_result + math.log2(temp_dict[(i,j,temp[i][j])])
		result.append(curr_result);
	result_num = np.argmax(result);
	true_num = 0
	total_count_no += 1
	if (result_num == true_num):
		correct_count_no += 1


print(correct_count_yes)
print(total_count_yes)
print("Result: ")
print(correct_count_yes/total_count_yes)
print()
print(correct_count_no)
print(total_count_no)
print("Result: ")
print(correct_count_no/total_count_no)