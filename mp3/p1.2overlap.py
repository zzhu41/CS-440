import math
import numpy as np


trainingData = open('trainingimages', 'r');
label_file = open('traininglabels', 'r');
label_array = [] #array consist of training label
count = 0
index = 0
data_dict = {}
temp = []

for line in label_file:
	label_array.append(int(line[0:-1]));

total_training_data = len(label_array)

for line in trainingData:
	if(count < 28):
		temp.append(line[0:-1])
		count += 1;
	else:
		number = label_array[index]
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
m = 4
n = 2
#m*n size block
trained = []

for i in range(10):#For each number class
	num_trained = {}#Dict for current class, (x, y, pixelValue):Occurance Time
	#Init the num_trained
	# for x_idx in range(28):
	# 	for y_idx in range(28):
	# 		for ele in ['#', '+', ' ']:
	# 			num_trained[(x_idx, y_idx, ele)] = 1;
	for j in range(len(data_dict[i])):
		num_dict = data_dict[i][j]
		for x_idx in range(len(num_dict)-(m-1)):
			for y_idx in range(len(num_dict[x_idx])-(n-1)):
				#obj = (x_idx, y_idx, data_dict[i][j][x_idx][y_idx])
				wenwen = ''
				c = 0
				for x in range(m):
					for y in range(n):
						#wenwen += data_dict[i][j][x_idx+x][y_idx+y]
						if data_dict[i][j][x_idx+x][y_idx+y] == '#' :
							wenwen += '#'
						if data_dict[i][j][x_idx+x][y_idx+y] == '+' :
							wenwen += '#'
						if data_dict[i][j][x_idx+x][y_idx+y] == ' ' :
							wenwen += ' '
				obj = (x_idx, y_idx, wenwen)
				if obj in num_trained:
					num_trained[obj] += 1;
				else:
					num_trained[obj] = 1;
	for key in num_trained.keys():
		print(key)
		num_trained[key] = (num_trained[key]) / float(len(data_dict[i]));
	trained.append(num_trained);

# print(trained[0])

trainingData.close();
label_file.close();

#Validate
f_testi = open('testimages', 'r');
f_testl = open('testlabels', 'r');

label_arr_valid = []
total_result  = []

temp = []
testimage_file = []
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
		t_count += 1;
		testimage_file.append(temp)
		result = []
		for k in range(10):
			curr_result = np.log2(class_possibility[k]);
			for i in range(28-(m-1)):
				for j in range(28-(n-1)):
					temp_dict = trained[k]
					wlb = ''
					for x in range(m):
						for y in range(n):
							#wlb += temp[i+x][j+y]
							if temp[i+x][j+y] == '#' :
								wlb += '#'
							if temp[i+x][j+y] == '+' :
								wlb += '#'
							if temp[i+x][j+y] == ' ' :
								wlb += ' '
					if (i,j,wlb) in temp_dict:
						t = temp_dict[(i,j,wlb)]
					else:
						t = 1/float(len(data_dict[k]))
					curr_result = curr_result + np.log2(t)
			result.append(curr_result);
		result_num = np.argmax(result);
		true_num = label_arr_valid[index];
		# print(result[result_num])
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

		# total_result.append(np.argmax(result))
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