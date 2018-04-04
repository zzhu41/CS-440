import numpy as np
import math

f_ti = open('digitdata/trainingimages', 'r');
f_tl = open('digitdata/traininglabels', 'r');
label_arr = []
count = 0
index = 0
data_dict = {}
temp = []

for line in f_tl:
	label_arr.append(int(line[0:-1]));

total_training_data = len(label_arr)

for line in f_ti:
	if(count < 28):
		temp.append(line[0:-1])
		count += 1;
	else:
		number = label_arr[index]
		if number in data_dict:
			data_dict[number].append(temp)
		else:
			data_dict[number] = [temp];
		count = 0
		temp = []
		index += 1
		temp.append(line[0:-1])
		count += 1;


## calculate possibility

# priors
class_possibility = []
for i in range(10):
	class_possibility.append(float(len(data_dict[i])) / total_training_data)

# print(class_possibility)
# conditional possibility
trained = []
for i in range(10):#For each number class
	num_trained = {}#Dict for current class, (x, y, pixelValue):Occurance Time
	#Init the num_trained
	for x_idx in range(28):
		for y_idx in range(28):
			for ele in ['#', '+', ' ']:
				num_trained[(x_idx, y_idx, ele)] = 1;
	for j in range(len(data_dict[i])):
		num_dict = data_dict[i][j]
		for x_idx in range(len(num_dict)):
			for y_idx in range(len(num_dict[x_idx])):
				obj = (x_idx, y_idx, data_dict[i][j][x_idx][y_idx])
				if obj in num_trained:
					num_trained[obj] += 1;
				else:#should not go here
					print("Something goes wrong dude")
					num_trained[obj] = 1;
	for key in num_trained.keys():
		num_trained[key] = (num_trained[key]) / float(len(data_dict[i]));
	trained.append(num_trained);

# print(trained[0])

f_ti.close();
f_tl.close();

#Validate
f_testi = open('digitdata/testimages', 'r');
f_testl = open('digitdata/testlabels', 'r');

label_arr_valid = []
total_result  = []

temp = []
count = 0
index = 0
valid_data = {}
correct_count = np.zeros(10);
total_count = np.zeros(10);
confusion_matrix = np.zeros((10,10))
biggest_posterior = np.array([-np.inf]*10)
index_biggest = np.zeros(10)
smallest_posterior = np.array([np.inf]*10)
index_smallest = np.zeros(10)
t_count = 0.0;
c_count = 0.0;

for line in f_testl:
	label_arr_valid.append(int(line[0:-1]));

for line in f_testi:
	if(count < 28):
		temp.append(line[0:-1])
		count += 1;
	else:#get an image
		result = []
		for k in range(10):
			curr_result = math.log1p(class_possibility[k]);
			for i in range(28):
				for j in range(28):
					temp_dict = trained[k]
					curr_result = curr_result + math.log1p(temp_dict[(i,j,temp[i][j])])
			result.append(curr_result);
		result_num = np.argmax(result);
		true_num = label_arr_valid[index];
		# total_result.append(np.argmax(result))
		if biggest_posterior[true_num] < result[result_num]:
			biggest_posterior[true_num] = result[result_num]
			index_biggest[true_num] = index;
		if smallest_posterior[true_num] > result[result_num]:
			smallest_posterior[true_num] = result[result_num]
			index_smallest[true_num] = index;

		total_count[true_num] += 1;
		if (result_num == label_arr_valid[index]):
			c_count += 1;
			correct_count[true_num] += 1;
		confusion_matrix[true_num,result_num] += 1
		##
		count = 0
		index += 1
		temp = []
		temp.append(line[0:-1])
		count += 1;

# print(correct_count)
# print(total_count)
print("Result: ")
print(correct_count/total_count)
for i in range(10):
	confusion_matrix[i] /= total_count[i]

# print(confusion_matrix)
# print(index_biggest)
# print(index_smallest)
# for i in range(10):
# 	print(testimage_file[int(index_biggest[i])])
# 	print(testimage_file[int(index_smallest[i])])
print(c_count/t_count);


