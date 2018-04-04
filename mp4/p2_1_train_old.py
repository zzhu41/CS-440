import numpy as np
import math
from random import randint
import random

def discretize(cur_state):
	# print(cur_state)
	(b_x, b_y, v_x, v_y, p_y) = cur_state
	# Set vx_dis
	vx_dis = 1 if (v_x>0) else -1
	#Set vy_dis
	if (v_y > 0.015):
		vy_dis = 1
	elif (v_y < -0.015):
		vy_dis = -1
	else:
		vy_dis = 0
	#Set bx_dis & by_dis
	bx_dis = math.floor(b_x*12)
	if bx_dis == 12:
		bx_dis = 11
	by_dis = math.floor(b_y*12)
	if by_dis == 12:
		by_dis = 11
	#Set py_dis, the position of paddle in the grid
	py_dis = math.floor(12 * p_y / (1 - p_h))
	if (py_dis >= 12):
		py_dis = 11
	# print(bx_dis, by_dis, vx_dis, vy_dis, py_dis)
	new_state = (bx_dis, by_dis, vx_dis, vy_dis, py_dis)
	return new_state

def checkConBounce(cur_state, bounceCounter):
	(b_x, b_y, v_x, v_y, p_y) = cur_state
	if (v_x > 0 and (b_y >= p_y and b_y <= p_y+0.2) and b_x >= 1):
		b_x = 2 - b_x
		U = random.uniform(-0.015, 0.015)
		V = random.uniform(-0.03, 0.03)
		# print("U: %03f, V: %03f" % (U, V))
		if (-v_x + U > 0.03):
			v_x = 0.029
		elif (-v_x + U < -0.03):
			v_x = -0.029
		v_x = -v_x + U 
		v_y = v_y + V
		bounceCounter = 1
	if (v_x > 0 and (not (b_y >= p_y and b_y <= p_y+0.2)) and b_x >= 1):
		bounceCounter = -1
	if (b_y < 0):
		b_y = -b_y
		v_y = -v_y
	if (b_y > 1):
		b_y = 2 - b_y
		v_y = -v_y
	if (b_x < 0):
		b_x = -b_x
		v_x = -v_x
	ret = ((b_x, b_y, v_x, v_y, p_y), bounceCounter)
	return ret

def updateConState(cur_state, action, bounceCounter):
	(b_x, b_y, v_x, v_y, p_y) = cur_state
	if (action == 0):
		p_y -= 0.04
	if (action == 2):
		p_y += 0.04
	cur_state = (b_x+v_x, b_y+v_y, v_x, v_y, p_y)
	# print("Updating:")
	# print(cur_state)
	ret = checkConBounce(cur_state, bounceCounter)
	return ret
	
def checkBounce(bx, by, vx, vy, py):
	#Check left wall, ball_x = -ball_x and velocity_x = -velocity_x
	if (bx < 0):
		bx = -bx
		vx = -vx
	#Check top wall, ball_y = -ball_y and velocity_y = -velocity_y
	if (by < 0):  
		by = -by
		vy = -vy
	#Check bottom wall
	if (by > 11):
		by = 11
		vy = -vy
	#Check bouncing
	# if (bx == 12 and by == py):
	# 	bx = 11
	# 	vx = -vx
	return (bx, by, vx, vy, py)

def updateState(cur_state, action):
	(bx, by, vx, vy, py) = cur_state
	if (bx == 11 and by == py and vx == 1):
		new_state = (bx, by+vy, -vx, vy, py)
		new_state = checkBounce(bx, by+vy, -vx, vy, py)
		return (new_state, True)
	bx += vx
	by += vy
	if (action == 0):#Up
		py += -1
	elif (action == 2):#Down
		py += 1
	new_state = checkBounce(bx, by, vx, vy, py)
	return (new_state, False)

def findAction_train(cur_state, epsilon):
	py = cur_state[4]
	if (py == 0):
		Q_sa_list = np.zeros(2)
		for ran_action in range(1, 3):
		#Using possible action, consider to go to the next state
			Q_sa = Q[cur_state][ran_action]
			Q_sa_list[ran_action-1] = Q_sa
		prob = random.random()
		if(prob < 1-epsilon):
			return np.argmax(Q_sa_list)+1
		else:
			return np.random.randint(0,1)+1
	elif (py == 11):
		Q_sa_list = np.zeros(2)
		for ran_action in range(0, 2):
		#Using possible action, consider to go to the next state
			Q_sa = Q[cur_state][ran_action]
			Q_sa_list[ran_action] = Q_sa
		prob = random.random()
		if(prob < 1-epsilon):
			return np.argmax(Q_sa_list)
		else:
			return np.random.randint(0,1)
	else:
		Q_sa_list = np.zeros(3)
		for ran_action in range(0, 3):
			#Using possible action, consider to go to the next state
			Q_sa = Q[cur_state][ran_action]
			Q_sa_list[ran_action] = Q_sa
		prob = random.random()
		if(prob < 1-epsilon):
			return np.argmax(Q_sa_list)
		else:
			return np.random.randint(0,2)

def findAction(cur_state):
	py = cur_state[4]
	if (py == 0):
		Q_sa_list = np.zeros(2)
		for ran_action in range(1, 3):
		#Using possible action, consider to go to the next state
			Q_sa = Q[cur_state][ran_action]
			Q_sa_list[ran_action-1] = Q_sa
		return np.argmax(Q_sa_list)+1
	elif (py == 11):
		Q_sa_list = np.zeros(2)
		for ran_action in range(0, 2):
		#Using possible action, consider to go to the next state
			Q_sa = Q[cur_state][ran_action]
			Q_sa_list[ran_action] = Q_sa
		return np.argmax(Q_sa_list)
	else:
		Q_sa_list = np.zeros(3)
		for ran_action in range(0, 3):
			#Using possible action, consider to go to the next state
			Q_sa = Q[cur_state][ran_action]
			Q_sa_list[ran_action] = Q_sa
		return np.argmax(Q_sa_list)


#Initalization
alpha = 1.0 #Step Size/learning rate
gamma = 0.8 #Discount 
decay = 100.0 #Decay constant
p_h = 0.2

Q = {}
# R = {}
N = {}
 
#Init the R and Q
# actionCount = 0
for py_dis in range(12):#paddle y
	for vx_dis in [-1,1]:
		for vy_dis in [-1,0,1]:
			for bx_dis in range(12):
				for by_dis in range(12):
					t = (bx_dis, by_dis, vx_dis, vy_dis, py_dis)#Create tuple
					Q[t] = [0, 0, 0]
					N[t] = [0, 0, 0]
Q[(-1)] = 0
N[(-1)] = 0

#Begin training
trainCount = 100000 #100K
trainLoop = 0
Q_up = 0
epsilon = 1 # initial epsilon
while (trainLoop < trainCount):
	print(trainLoop)
	#Select random initial state
	ran_bx = 0.5
	ran_by = 0.5
	ran_vx = 0.03
	ran_vy = 0.01
	ran_py = 0.5 - p_h/2 

	con_state = (ran_bx, ran_by, ran_vx, ran_vy, ran_py)
	cur_state = discretize(con_state)
	# print(cur_state)
	# print("Random state: (%d, %d, %d, %d, %d)" % (ran_bx, ran_by, ran_vx, ran_vy, ran_py))
	
	goal = False
	while (True):
		if (cur_state == -1):
			# print("#############################")
			nxt_state = -1
			cur_Qsa = Q[-1]
			nxt_Qsa = Q[-1]
			N[-1] += 1
			R_cur = -1
			goal = True
			#Set up learning rate
			LR = alpha * (decay/(decay + N[-1]))
			#Update Q[s,a]
			Q[-1] += LR*(R_cur + gamma*nxt_Qsa - cur_Qsa)
			break
		# print(":"),
		cur_action = findAction_train(cur_state,epsilon)
		# print("Choose action: %d" % cur_action)
		cur_Qsa = Q[cur_state][cur_action]
		N[cur_state][cur_action] += 1
		#Update next state
		nxt_state = updateConState(con_state, cur_action, 0)
		if (nxt_state[1] == 1):
			print("............................")
			R_cur = 1
		else:
			R_cur = 0
		nxt_state = nxt_state[0]
		# print(nxt_state)
		# print("Next state: (%d, %d, %d, %d, %d)" % nxt_state)
		#Check if outside the bound
		if (nxt_state[0] > 1):
			# print("True")
			nxt_state_dis = (-1)
			nxt_state = (-1)
			nxt_Qsa = Q[-1]
		else:	
			nxt_state_dis = discretize(nxt_state)
			nxt_action = findAction_train(nxt_state_dis,epsilon)
			# print("Next action: %d" % nxt_action)
			nxt_Qsa = Q[nxt_state_dis][nxt_action]
		#Set up learning rate
		LR = alpha * (decay/(decay + N[cur_state][cur_action]))
		#Update Q[s,a]
		Q[cur_state][cur_action] += LR*(R_cur + gamma*nxt_Qsa - cur_Qsa)
		if (goal):
			break
		else:
			con_state = nxt_state
			cur_state = nxt_state_dis


	# if (Q[cur_state][cur_action] != 0):
	# 	Q_up += 1
		# print("Update Q as %02f" % Q[cur_state][cur_action])

	trainLoop += 1
	epsilon -= 1.0/trainCount
# print(Q)
# print(Q_up)


total = 0
for i in range(10):
	b_x = 0.5 #Ball X, Y location
	b_y = 0.5
	v_x = 0.03 #Ball X speed
	v_y = 0.01 #Ball Y speed
	p_y = 0.5 - p_h/2 #Paddle Y location, in range(0, 1-(p_h))
	bc = 0 #Bouncing counter
	#Testing
	print("Testing...")
	#Start with init state
	cur_state = (b_x, b_y, v_x, v_y, p_y)
	#Do while until goes out of the board
	while(cur_state[0] < 1):
		print(cur_state)
		#Discrete current state 
		dis_state = discretize(cur_state)
		print(dis_state)	
		#From Q find argmax action
		action = findAction(dis_state)
		print(Q[dis_state])
		print(action)
		#Update state & p_y according to selected action
		ret_val = updateConState(cur_state, action, bc)
		cur_state = ret_val[0]
		if(ret_val[1] == 1):
			bc += 1

	print ("result: %d" % bc)
	total += bc
total /= 10.0
print("Average Bounce: %03f" % total)
