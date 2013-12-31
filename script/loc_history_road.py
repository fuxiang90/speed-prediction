#coding=utf-8
#/urs/bin/env python

import csv
import matplotlib.pyplot as plt 
import datetime
import time
def get_weekday(year,mou,day):
    if year == 0 or mou == 0 or day == 0 :
        return -1 
    t = datetime.date(year,mou,day)
    return (t.weekday() + 1)%7

class LocHistoryRoad(object):

    def __init__(self,csv_path):
        
        self._road_id = 0
        self._history_road_speed_dict = dict() # [0.0 for i in range(24*60/4)]
        self._history_road_speed_count = dict() 
        
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
            
            weekday = get_weekday(year[0], year[1], year[2])

            h = hours[0]
            m = hours[1]

            if weekday not in self._history_road_speed_dict :
                self._history_road_speed_dict[weekday] = [0.0 for i in range(24*60/4) ]
                self._history_road_speed_count[weekday] = [0 for i in range(24*60/4) ]
            
            self._history_road_speed_dict[weekday][(h*60 + m)/4] += float(speed)
            self._history_road_speed_count[weekday][(h*60 + m)/4] += 1
        for weekday in range(7):
            for i in range(60*24/4):
                self._history_road_speed_dict[weekday][i] /= self._history_road_speed_count[weekday][i]
    def parse_time(self,time_str):
        time_list = time_str.strip().split(' ')
        if len(time_list) != 2 :
            return (0,0,0),(0,0)
        year_tuple = tuple( int(i) for i in time_list[0].split('-'))
        date_list = time_list[1].split(':')
        return year_tuple,( int(date_list[0]),int(date_list[1]) ) 


    def show(self):

        d = [4*i for i in range(24*60/4)]
        for i in range(7):
            v = self._history_road_speed_dict[i]
            plt.plot(d, v )
            #plt.show() 
            plt.savefig(str(i)+".png")
    def read(self,h,m):
        t = h * 60 + m ;
        t /= 4;
        return self._history_road_speed[t]
    def get_speed(self,year_tuple, h ,m):

        weekday = get_weekday(year_tuple[0], year_tuple[1], year_tuple[2])
        t = h*60 + m ;
        t /= 4

        return self._history_road_speed_dict[weekday][t]

    def set(self,h,m,speed):
        t = h * 60 + m ;
        t /= 4;
        
        self._history_road_speed[t] = speed


if __name__ == '__main__':
    
    
    """
    print get_weekday(2013,12,29)

    """
    test = LocHistoryRoad('../data/csv/376_0.csv')
    #test.show() 
