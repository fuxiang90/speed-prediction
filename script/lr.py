# coding=utf-8
#! /usr/bin/python
 
import matplotlib.pyplot as plt
import numpy as np
import mlpy
import sys
import math


def get_input(filename):
    fin = open(filename)
    
    x = []
    y = []
    for each in fin:
        each = each[:each.find('\n')]
        l = each.split(' ')
        each_x = []
        each_x.append(1)
        each_x .append(float(l[0]))
        each_x .append(float(l[1]))
        each_x .append(float(l[2]))
        each_x .append(float(l[3]))
        x.append(each_x)
        y.append(float(l[4]))
    return x ,y
 
def mlpy_linear_regressiono(x ,y):
 
    beta ,rank = mlpy.ols_base(x, y,0.05)   
    
    
    return beta
 
def split(filename):
    
    f_train = open('train','w')
    f_test = open('test','w')
    
    fin = open(filename)
    
    pos = 0
    for each in fin:
        if pos % 8 ==0:
            f_test.write(each)
        else:
            f_train.write(each)
        pos = pos + 1
    
    fin.close()
    f_train.close()
    f_test.close()
 
 
def ridge_main():
    x,y = get_input('train')
    ridge = mlpy.Ridge()
    ridge.learn(x, y)
    test_x ,test_y = get_input('test')
    right_num = 0
    mae = 0.0
    for pos  in range(0,len(test_x)):
        yy = ridge.pred(test_x[pos])
        #print yy ,test_y[pos]
        if abs(yy - test_y[pos]) < 5.0 :
            right_num +=1

        mae += math.sqrt( (yy-test_y[pos]) ** 2 )
    print right_num*1.0 / len(test_x), mae/len(test_x)
 
def lars_base_main():
    x,y = get_input('train')
    lars = mlpy.LARS()
    lars.learn(x, y)
    test_x ,test_y = get_input('test')
    right_num = 0
    for pos  in range(0,len(test_x)):
        yy = lars.pred(test_x[pos])
        #print yy ,test_y[pos]
        if abs(yy - test_y[pos]) < 5.0 :
            right_num +=1
    print right_num*1.0 / len(test_x)  
      
def main():
    
    x,y = get_input('train')
    beta =  mlpy_linear_regressiono(x,y)
    test_x ,test_y = get_input('test')
    right_num = 0
    mae = 0.0
    for pos in range(0,len(test_x)):
        pre_y = test_x[pos][0] * beta[0] + test_x[pos][1] * beta[1] + test_x[pos][2] * beta[2] + test_x[pos][3] * beta[3]
        
        #print pre_y , y[pos]
        
        if abs(pre_y - y[pos]) < 5.0:
            right_num += 1
        
        mae +=  math.fabs(pre_y-y[pos] )
    
    print right_num*1.0 / len(test_x) ,mae/len(test_x)     

def test_main():
    files = ['2111_data' , '22520_data' ,'3860_data','390_data','620_data']

    for file_name in files:
        name = file_name[:file_name.find('_')]
        print "roadid :" , name
        split(file_name)
        #ridge_main()
        main()
if __name__ =='__main__':
    
    #split('2111_data')
    #ridge_main()
    test_main() 
    #main()
    #lars_base_main()
