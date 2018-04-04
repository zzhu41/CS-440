import numpy as np
import math
# from sklearn.lda import LDA
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis as LDA

k = 10
tSet = ''
line_count = 0;
spaceCount = 0;
pMatrix = np.ones((25,10))*k
train_list = []
test_list = []
train_labels = []

#Read from path
f_trainYSet = open('yesno/yes_train.txt', 'r')
f_trainNSet = open('yesno/no_train.txt', 'r')
f_testYSet = open('yesno/yes_test.txt', 'r')
f_testNSet = open('yesno/no_test.txt', 'r')
train_sets = [f_trainYSet,f_trainNSet,f_testYSet,f_testNSet]
test_result = []

#Open file, Read by lines. 
for x in range(4):
	for line in train_sets[x]:
	#If line is empty then mark is finished and if line is not empty, mark as reading and add the value to pMatrix & pCountMatrix
		#while reading
		if (line_count < 25):
			# print("Parsing lines: %s with line_count %d" % (line, line_count))
			for i in range(len(line)-1):
				pMatrix[line_count][i] = 1.0 if (line[i]=='%') else 0.0 # '%' -> 1, ' ' -> 0
			line_count += 1
		elif (line_count >= 25 and spaceCount < 2):

			# print("Empty lines: %s" % line)
			spaceCount += 1
		elif(line_count >= 25 and spaceCount == 2):
			print (x)
			if (x == 0):
				train_labels.append(1)#yes
			if (x == 1):
				train_labels.append(0)#no
			spaceCount = 0
			line_count = 0
			if (x <= 1):
				# print("printing1")
				# print (pMatrix.flatten())
				train_list.append(pMatrix.flatten())
			else:
				# print("printing2")
				# print (pMatrix.flatten())
				test_list.append(pMatrix.flatten())
			
	# break
# print(train_labels)
# print(len(train_list))
# print(len(train_labels))
X = np.array(train_list)
y = np.array(train_labels)
clf = LDA()
clf.fit(X, y)
LDA(n_components=None, priors=None, shrinkage=None, solver='svd', store_covariance=False, tol=0.0001)
test_result = clf.predict(test_list)
print(clf.predict(test_list))

total_count = 0
tc1 = 0
tc2 = 0

for idx in range(len(test_result)):
	total_count += 1.0
	if (idx < len(test_result)/2):
		if (test_result[idx] == 1):
			tc1 += 1.0
	else:
		if (test_result[idx] == 0):
			tc2 += 1.0


print("Correctness: %d Percent" % (tc1/total_count*100*2))
print("Correctness: %d Percent" % (tc2/total_count*100*2))
print("Correctness: %d Percent" % ((tc1+tc2)/total_count*100))