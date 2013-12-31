#coding=utf-8
#/urs/bin/env python
import os
import csv

def csv_read(file_name):
    reader = csv.reader(open(file_name),delimiter = ",")
    count = 0 
    for id, speed, times in reader:
        count += 1
        if count == 1:
            continue 

        print id 


if __name__ == '__main__':

    
    csv_read("../data/csv/386_0.csv")
