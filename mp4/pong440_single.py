#!/usr/bin/env python
#
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 2 of the License, or
#       (at your option) any later version.
#       
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#		It's my first actual game-making attempt. I know code could be much better 
#		with classes or defs but I tried to make it short and understandable with very 
#		little knowledge of python and pygame(I'm one of them). Enjoy.

import numpy as np
import math
from random import randint
import random
import time
#Game --------
import pygame
from pygame.locals import *
from sys import exit
import random

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
    trainCount = 50000 #100K
    print("Train %d times for the pong game." % trainCount )
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
    print("Train ended.")
    return Q

def game_one_run(Q, b_x, b_y, v_x, v_y, p_y):
    #Discrete current state 
    bc = 0
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
            # print("Bounce")
            bc = 1
            b_x = 2 - b_x
            print("Bounce %03f" % b_x)
            U = random.uniform(-0.015, 0.015)
            V = random.uniform(-0.03, 0.03)
            if (-v_x + U > 0.03):
                v_x = 0.029
            elif (-v_x + U < -0.03):
                v_x = -0.029
            v_x = -abs(-v_x + U)
            v_y = v_y + V
        # if failed to catch the ball
        else:
            return (-1, bc)

    return ((b_x, b_y, v_x, v_y, p_y),bc)




#Game ==================================
b_x = 0.5
b_y = 0.5
v_x = 0.03
v_y = 0.01
p_y = 0.4
scaleFactor = 640
grid_size = 12
p_h = 0.2

Q = train()

print("Game start!")

pygame.init()

screen=pygame.display.set_mode((640,640),0,32)#[screen_width,screen_height]
pygame.display.set_caption("The Ultimate Pong")

#Creating 2 bars, a ball and background.
back = pygame.Surface((640,640))
background = back.convert()
background.fill((255,255,255))
bar = pygame.Surface((10,128))#10 by 50 pixels, 25
# bar1 = bar.convert()
# bar1.fill((0,0,255))
bar2 = bar.convert()
bar2.fill((255,0,0))
circ_sur = pygame.Surface((15,15))
circ = pygame.draw.circle(circ_sur,(0,0,255),(7,7),7)
circle = circ_sur.convert()
circle.set_colorkey((0,0,0))

# some definitions
bar2_x = 630.
bar2_y = 215.
circle_x, circle_y = 307.5, 307.5
bar1_move, bar2_move = 0. , 0.
speed_circ = 250., 250., 250.
bar1_score, bar2_score = 0,0
#clock and font objects
clock = pygame.time.Clock()
font = pygame.font.SysFont("Chalkboard",80)

speed_x = 0.03
speed_y = 0.01
# ai_speed = 0

while True:
    for event in pygame.event.get():
        if event.type == QUIT:
            exit()
        # if event.type == KEYDOWN:
        #     if event.key == K_UP:
        #         bar1_move = -ai_speed
        #     elif event.key == K_DOWN:
        #         bar1_move = ai_speed
        # elif event.type == KEYUP:
        #     if event.key == K_UP:
        #         bar1_move = 0.
        #     elif event.key == K_DOWN:
        #         bar1_move = 0.
    
    # score1 = font.render(str(bar1_score), True, (255,255,255))
    score2 = font.render(str(bar2_score), True, (0,0,0))

    screen.blit(background,(0,0))
    frame = pygame.draw.rect(screen,(0,0,0),Rect((1,1),(639,639)),2)
    screen.blit(bar2,(bar2_x,bar2_y))
    screen.blit(circle,(circle_x,circle_y))
    screen.blit(score2,(320.,120.))
    
# movement of circle
    time_passed = clock.tick(30)
    time_sec = time_passed / 1000.0
    circle_x = b_x * (scaleFactor-10)
    circle_y = b_y * scaleFactor
    bar2_y = p_y * scaleFactor

    (res, bc) = game_one_run(Q, b_x, b_y, v_x, v_y, p_y)
    bar2_score += bc
    if (res == -1):
        print ("Game end, bounce: %d" % bar2_score)
        time.sleep(3)
        bar2_x = 630.
        bar2_y = 215.
        circle_x, circle_y = 307.5, 307.5
        bar1_move, bar2_move = 0. , 0.
        speed_circ = 250., 250., 250.
        bar1_score, bar2_score = 0,0
        b_x = 0.5
        b_y = 0.5
        v_x = 0.03
        v_y = 0.01
        p_y = 0.4
        # break

    else:
        (b_x, b_y, v_x, v_y, p_y) = res


#AI of the computer.
    
    # print("update")
    pygame.display.update()
# time.sleep(5)