import numpy as np
import math


def trainAudioModel(tSet, tLabel):
	# print("Training for : %s" % tLabel);
	#Some variables here

	k = 12;
	line_count = 0;
	spaceCount = 0;

	#Read from path
	tSetPath = 'yesno/'+tSet
	# print(tSetPath)
	f_tSet = open(tSetPath, 'r')

	#create 2D array for Prob.
	pMatrix = np.ones((25,10))*k#25*10 array
	# pCountMatrix = np.ones((25,10))#With lap smooth
	pCount = 0;

	#Open file, Read by lines. 
	for line in f_tSet:
	#If line is empty then mark is finished and if line is not empty, mark as reading and add the value to pMatrix & pCountMatrix
		#while reading
		if (line_count < 25):
			# print("Parsing lines: %s with line_count %d" % (line, line_count))
			for i in range(len(line)-1):
				pMatrix[line_count][i] += 1.0 if (line[i]=='%') else 0.0 # '%' -> 1, ' ' -> 0
			line_count += 1
		elif (line_count >= 25 and spaceCount < 2):
			# print("Empty lines: %s" % line)
			spaceCount += 1
		elif(line_count >= 25 and spaceCount == 2):
			# print("Ending reading")
			pCount += 1
			spaceCount = 0
			line_count = 0

	#After adding all the data, calculate the mean
	# print(pMatrix)

	pMatrix = pMatrix/(pCount+k)

	print("Finish Training with %d training data" % pCount)
	# print(pMatrix)
	return pMatrix
	
def classify(trainedMatrix, testSetName, contents):
	#Construct result array 
	result_y = []
	result_n = []
	pMatrix = np.zeros((25,10))
	line_count = 0
	spaceCount = 0

	#First read from testSet 
	testPath = 'yesno/'+testSetName
	# print("Reading from %s" % testSetName)
	testSet = open(testPath, 'r')

	#Open file, Read by lines. 
	for line in testSet:
	#If line is empty then mark is finished and if line is not empty, mark as reading and add the value to pMatrix & pCountMatrix
		#while reading
		if (line_count < 25):
			# print("Continue... Parsing lines: %s" % line)
			for i in range(len(line)-1):
				pMatrix[line_count][i] = 1.0 if (line[i]=='%') else 0.0 # '%' -> 1, ' ' -> 0
			line_count += 1
		#first empty line
		elif (line_count >= 25 and spaceCount < 2):
			spaceCount += 1
		elif (line_count >= 25 and spaceCount == 2):
			line_count = 0
			spaceCount = 0
			#check yes prob
			yesProb = math.log2(140/(140+131))
			noProb = math.log2(131/(140+131))
			# print(pMatrix)
			for i in range(len(pMatrix)):
				for j in range(len(pMatrix[i])):
					if (pMatrix[i][j] == 1): # %
						yesProb += math.log2(trainedMatrix[0][i][j])
						noProb += math.log2(trainedMatrix[1][i][j])
					else:
						# print(trainedMatrix[0][i][j])
						yesProb += math.log2(1-trainedMatrix[0][i][j] if (1-trainedMatrix[0][i][j] != 0) else 1/(140+131))
						noProb += math.log2(1-trainedMatrix[1][i][j] if (1-trainedMatrix[1][i][j] != 0) else 1/(140+131))

			result_y.append(yesProb)
			result_n.append(noProb)


			# print("Prob_y is %d, prob_n is %d" % (yesProb, noProb))

	# print(result_y)
	# print(result_n)
	y_count = 0
	n_count = 0
	for ele in range(len(result_y)):
		if (result_y[ele] > result_n[ele]):
			y_count += 1
		else:
			n_count += 1

	print("Yes: %d, No: %d" % (y_count, n_count))
	if contents == 'yes':
		return [y_count, (y_count + n_count)]
	else:
		return [n_count, (y_count + n_count)]


yesMatrix = trainAudioModel('yes_train.txt', 'Yes')
noMatrix = trainAudioModel('no_train.txt', 'No')

[a,b] = classify([yesMatrix, noMatrix], 'yes_test.txt', 'yes')
[c,d] = classify([yesMatrix, noMatrix], 'no_test.txt', 'no')
print((a+c) / (b+d))

confusion = np.zeros((2,2))
confusion[0,0] = 49/50
confusion[0,1] = 1/50
confusion[1,0] = 3/50
confusion[1,1] = 47/50

print (confusion)