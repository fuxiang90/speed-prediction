# coding=utf-8
#! /usr/bin/python
 
import matplotlib.pyplot as plt
import numpy as np
import mlpy
import sys
import math

from config import loc_store_path, times_day  


class LocTrain(object):
    
    """
    
    """
    def __init__(self, file_name ,  flag):
        
        if flag == "ridge":
            self.lrs = [ mlpy.Ridge()  for i in range(7) ]
        else :
            self.lrs = [ mlpy.OLS()  for i in range(7) ]
        
        self.file_name = file_name 
        
        self.x_weekday = dict()
        self.y_weekday = dict()
        #self.x ,self.y = 
        
    def train(self):
        self.get_input(self.file_name)
        
        for weekday in range(7):
            self.lrs[weekday].learn(self.x_weekday[weekday], self.y_weekday[weekday]) 
        
    def predict(self,weekday,speeds):
        
        pre_speed = self.lrs[weekday].pred(speeds)
        return pre_speed 
    def test(self):
        pass 
    
    def get_input(self, filename):
        fin = open(filename)
        
        
        x = []
        y = []
        for each in fin:
            each = each[:each.find('\n')]
            l = each.split(' ')
            each_x = []
            each_x.append(1)
            each_x.append(float(l[0]))
            each_x.append(float(l[1]))
            each_x.append(float(l[2]))
            each_x.append(float(l[3]))
            
            weekday = int(l[5])
            if weekday not  in self.x_weekday:
                self.x_weekday[weekday] = []
                self.y_weekday[weekday] = []
            self.x_weekday[weekday].append(each_x)
            self.y_weekday[weekday].append(float(l[4]))
        

def main():

    tmp = LocTrain('../data/train/3860_data',"ridge")
    tmp.train() 
    
    print tmp.predict(1,[1,10,10,20,10])
    pass 

if __name__ == '__main__':
    main() 
