import numpy as np
import math

f_ti = open('data22/training_data.txt', 'r');
f_tl = open('data22/training_labels.txt', 'r');
k = 1;
label_arr = []
count = 0
index = 0
data_dict = {}
temp = []

for line in f_tl:
	label_arr.append(int(line));

total_training_data = len(label_arr)

for line in f_ti:
	if(count < 33):
		if(count < 30):
			temp.append(line[0:-1])
			count += 1
		else:
			count += 1
	else:
		number = label_arr[index]
		if number in data_dict:
			data_dict[number].append(temp)
		else:
			data_dict[number] = [temp];
		count = 1
		temp = []
		index += 1
		temp.append(line[0:-1])


## calculate possibility

# priors
class_possibility = []
for i in range(5):
	class_possibility.append(float(len(data_dict[i+1])) / total_training_data)

# print(class_possibility)

# conditional possibility
trained = []
for i in range(1,6):#For each number class
	num_trained = {}#Dict for current class, (x, y, pixelValue):Occurance Time
	#Init the num_trained
	for x_idx in range(30):
		for y_idx in range(13):
			for ele in [' ', '%']:
				num_trained[(x_idx, y_idx, ele)] = k;
	for j in range(len(data_dict[i])):
		num_dict = data_dict[i][j]
		for x_idx in range(30):
			for y_idx in range(13):
				obj = (x_idx, y_idx, data_dict[i][j][x_idx][y_idx])
				if obj in num_trained:
					num_trained[obj] += 1;
				else:#should not go here
					print("ERROR")
	for key in num_trained.keys():
		# print((num_trained[key]))
		# print(len(data_dict[i])+k)
		# print("###")
		num_trained[key] = (num_trained[key]) / float(len(data_dict[i])+k);
	# print("####")
	# print("END ONE")
	trained.append(num_trained);

f_ti.close();
f_tl.close();


#Validate
f_testi = open('data22/testing_data.txt', 'r');
f_testl = open('data22/testing_labels.txt', 'r');

label_arr_valid = []
total_result  = []

temp = []
count = 0
index = 0
valid_data = {}
correct_count = np.zeros(5);
total_count = np.zeros(5);
confusion_matrix = np.zeros((5,5));
overall_correct = 0
overall_count = 0

for line in f_testl:
	label_arr_valid.append(int(line));


for line in f_testi:
	if(count < 33):
		if(count < 30):
			temp.append(line[0:-1])
			count += 1
		else:
			count += 1
	else:#get an image
		result = []
		for k in range(5):
			curr_result = math.log2(class_possibility[k]);
			for i in range(30):
				for j in range(13):
					temp_dict = trained[k]
					curr_result = curr_result + math.log2(temp_dict[(i,j,temp[i][j])])
			result.append(curr_result);
		result_num = np.argmax(result)+1;
		true_num = label_arr_valid[index];
		total_count[true_num-1] += 1;
		overall_count += 1
		if (result_num == label_arr_valid[index]):
			overall_correct += 1
			correct_count[true_num-1] += 1;
		confusion_matrix[true_num-1, result_num-1] += 1;
		count = 0
		index += 1
		temp = []
		temp.append(line[0:-1])
		count += 1;


for i in range(5):
	confusion_matrix[i,:] /= total_count[i]
# print(correct_count)
# print(total_count)
print("Result: ")
# print(correct_count/total_count)
print(overall_correct/overall_count)
print(confusion_matrix)