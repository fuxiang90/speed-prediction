#!/usr/bin/env python 
# coding=utf-8
import csv
from config import loc_store_path, times_day  


class LocRealRoad(object):

    """
        得到 某条道路的实时的速度，还需要得到一个，给预测算法的的一个输入
    """
    def __init__(self,csv_path):
        
        self._road_id = 0
        self._real_road_speed_dict = dict() # [0.0 for i in range(24*60/4)]
        
        self.init(csv_path)

    def init(self,csv_path):
        reader = csv.reader(open(csv_path),delimiter = ",")
        count = 0 
        for id, speed, times in reader:
            count += 1
            if count == 1:
                continue 
            
            self._road_id = id 
            year,hours = self.parse_time(times)
            

            h = hours[0]
            m = hours[1]

            if year not in self._real_road_speed_dict :
                self._real_road_speed_dict[year] = [0.0 for i in range(24*60/4) ]
            
            self._real_road_speed_dict[year][(h*60 + m)/4] = float(speed)
    def parse_time(self,time_str):
        time_list = time_str.strip().split(' ')
        if len(time_list) != 2 :
            return (0,0,0),(0,0)
        year_tuple = tuple( int(i) for i in time_list[0].split('-'))
        date_list = time_list[1].split(':')
        return year_tuple,( int(date_list[0]),int(date_list[1]) ) 


    def get_speed(self,year_tuple,h,m):

        t = h * 60 + m ;
        t /= 4

        return self._real_road_speed_dict[year_tuple][t]



if __name__ == '__main__':

    test = LocRealRoad('../data/csv/376_0.csv')

