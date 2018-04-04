import numpy as np
import math
from random import randint
import random
import time

def discretize(b_x, b_y, v_x, v_y, p_y):
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
	bx_dis = math.floor(b_x*grid_size)
	if bx_dis == grid_size:
		bx_dis = grid_size-1
	by_dis = math.floor(b_y*grid_size)
	if by_dis == grid_size:
		by_dis = grid_size-1
	#Set py_dis, the position of paddle in the grid
	py_dis = math.floor(grid_size * p_y / (1 - p_h))
	if (py_dis >= grid_size):
		py_dis = grid_size-1
	# print(bx_dis, by_dis, vx_dis, vy_dis, py_dis)
	new_state = (bx_dis, by_dis, vx_dis, vy_dis, py_dis)
	return new_state


def train():
	#Initalization
	gamma = 0.8 #Discount 
	decay = 0.9 #Decay constant
	p_h = 0.2
	grid_size = 12

	Q = {}
	# R = {}
	N = {}
	 
	#Init the R and Q
	# actionCount = 0
	for py_dis in range(grid_size):#paddle y
		for vx_dis in [-1,1]:
			for vy_dis in [-1,0,1]:
				for bx_dis in range(grid_size):
					for by_dis in range(grid_size):
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
		#initialize position
		b_x = 0.5
		b_y = 0.5
		v_x = 0.03
		v_y = 0.01
		p_y = 0.5 - p_h/2 

		cur_state = discretize(b_x, b_y, v_x, v_y, p_y)
		
		# while not fall
		while (b_x <= 1):
			action = -1
			reward = 0
			score = random.random()
			# epsilon greedy to choose action
			if score < epsilon:
				action = random.randint(0,2)
			else:
				action = np.argmax(Q[cur_state])
			if action == 0:
				p_y = max(0.0, p_y - 0.04)
			elif action == 2:
				p_y = min(0.8, p_y + 0.04)

			N[cur_state][action] += 1
			# Update bx by vx vy to next state
			b_x += v_x
			b_y += v_y
			if (b_y < 0):
				b_y = -b_y
				v_y = -v_y
			if (b_y > 1):
				b_y = 2 - b_y
				v_y = -v_y
			if (b_x < 0):
				b_x = -b_x
				v_x = -v_x
			if b_x > 1:
				# if catch the ball
				if b_y >= p_y and b_y <= p_y + 0.2:
					reward = 1
					b_x = 2 - b_x
					U = random.uniform(-0.015, 0.015)
					V = random.uniform(-0.03, 0.03)
					if (-v_x + U > 0.03):
						v_x = 0.029
					elif (-v_x + U < -0.03):
						v_x = -0.029
					v_x = -v_x + U 
					v_y = v_y + V

			next_state = discretize(b_x,b_y,v_x,v_y,p_y)
			
			if b_x > 1:
				reward = -1
				next_state = -1
			
			# print(next_state)

			LR = decay/(decay + N[cur_state][action])
			if next_state != -1:
				Q[cur_state][action] += LR*(reward + gamma*max(Q[next_state]) - Q[cur_state][action])
			else:
				Q[cur_state][action] += LR*(reward + gamma*Q[-1] - Q[cur_state][action])
			cur_state = next_state
		
		trainLoop += 1
		epsilon -= 1.0/trainCount
	return Q

def game_one_run(Q, b_x, b_y, v_x, v_y, p_y):
	#Discrete current state 
	dis_state = discretize(b_x, b_y, v_x, v_y, p_y)
	# print(dis_state)
	#From Q find argmax action
	action = np.argmax(Q[dis_state])
	# print(action)
	#Update state & p_y according to selected action
	# Update bx by vx vy to next state
	b_x += v_x
	b_y += v_y

	if(action == 0):
		p_y = max(0.0, p_y - 0.04)
	elif action == 2:
		p_y = min(0.8, p_y + 0.04)
	if (b_y < 0):
		b_y = -b_y
		v_y = -v_y
	if (b_y > 1):
		b_y = 2 - b_y
		v_y = -v_y
	if (b_x < 0):
		b_x = -b_x
		v_x = -v_x
	if b_x > 1:
		# if catch the ball
		if b_y >= p_y and b_y <= p_y + 0.2:
			b_x = 2 - b_x
			U = random.uniform(-0.015, 0.015)
			V = random.uniform(-0.03, 0.03)
			if (-v_x + U > 0.03):
				v_x = 0.029
			elif (-v_x + U < -0.03):
				v_x = -0.029
			v_x = -v_x + U 
			v_y = v_y + V
		# if failed to catch the ball
		else:
			return -1

	return (b_x, b_y, v_x, v_y, p_y)