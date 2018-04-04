import math
import numpy as np


trainingData = open('facedatatrain', 'r');
label_file = open('facedatatrainlabels', 'r');

training_label = [] #array consist of training label
count = 0
index = 0
data_dict = {}
temp = []

for line in label_file:
	training_label.append(int(line[0:-1]));

numberOfTrainingData = len(training_label)

for line in trainingData:
	if(count < 70):
		temp.append(line[0:-1])
		count += 1;
	else:
		isFace = training_label[index]
		if isFace in data_dict:
			data_dict[isFace].append(temp)
		else:
			data_dict[isFace] = [temp];
		count = 0
		temp = []
		index += 1
		temp.append(line[0:-1])
		count += 1;

## calculate possibility

# priors
class_possibility = []
for i in range(2):
	class_possibility.append(float(len(data_dict[i])) / numberOfTrainingData)
m = 70
n = 60
#m*n size block
trained = []

for i in range(2):#For each number class
	num_trained = {}#Dict for current class, (x, y, pixelValue):Occurance Time
	for x_idx in range(70):
		for y_idx in range(60):
			for ele in ['#', ' ']:
				num_trained[(x_idx, y_idx, ele)] = 1;
	for j in range(len(data_dict[i])):
		num_dict = data_dict[i][j]
		for x_idx in range(len(num_dict)-(m-1)):
			for y_idx in range(len(num_dict[x_idx])-(n-1)):
				#obj = (x_idx, y_idx, data_dict[i][j][x_idx][y_idx])
				compareString = ''
				for x in range(m):
					for y in range(n):
						#wenwen += data_dict[i][j][x_idx+x][y_idx+y]
						if data_dict[i][j][x_idx+x][y_idx+y] == '#' :
							compareString += '#'
						if data_dict[i][j][x_idx+x][y_idx+y] == ' ' :
							compareString += ' '
				obj = (x_idx, y_idx, compareString)
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
f_testi = open('facedatatest', 'r');
f_testl = open('facedatatestlabels', 'r');

label_arr_valid = []
total_result  = []

temp = []
testimage_file = []
count = 0
index = 0
t_count = 0.0;
correct_number = 0
wrong_number = 0
for line in f_testl:
	label_arr_valid.append(int(line[0:-1]));

for line in f_testi:
	if(count < 70):
		temp.append(line[0:-1])
		count += 1;
	else:#get an image
		t_count += 1;
		testimage_file.append(temp)
		result = []
		for k in range(2):
			curr_result = np.log2(class_possibility[k]);
			for i in range(70-(m-1)):
				for j in range(60-(n-1)):
					temp_dict = trained[k]
					stringtemp = ''
					for x in range(m):
						for y in range(n):
							#wlb += temp[i+x][j+y]
							if temp[i+x][j+y] == '#' :
								stringtemp += '#'
							if temp[i+x][j+y] == ' ' :
								stringtemp += ' '
					if (i,j,stringtemp) in temp_dict:
						t = temp_dict[(i,j,stringtemp)]
					else:
						t = 1/float(len(data_dict[k]))
					curr_result = curr_result + np.log2(t)
			result.append(curr_result);
		result_num = np.argmax(result);
		true_num = label_arr_valid[index];
		if result_num == true_num:
			correct_number +=1
			print("---------number:",t_count)
			print("correct")
			if(result_num == 1):
				print("face")
			else:
				print("not face")
			print(">>>>>>>>>")
		else:
			wrong_number +=1
			print("wrong")
		# total_result.append(np.argmax(result))
		count = 0
		index += 1
		temp = []
		temp.append(line[0:-1])
		count += 1;

print(correct_number)
print(wrong_number)
print(correct_number/t_count)
