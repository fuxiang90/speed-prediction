#!/usr/bin/env python 
from config import loc_store_path, times_day  
class LocRoad(object):

    def __init__(self,id, locid ,pre_locid,times_day):
        self.id = id
        self.locid = locid
        self.pre_locid = pre_locid
        
        self.history_road_speed = self.history_road(times_day)
        self.weight = [0.0 for i in range(5*7)]
    def history_road(self, n):
        l = [0.0 for i in range(7*n) ]
        
        return l 
    def get_history_speed(self,)
class RealRoadSpeed(object):
    """
      当前时间段loc 道路的实时速度
    """

    def __init__(self):
        self.year = 2013
        self.mouth = 5
        self.day = 12
        self.weekday = 4
        self.h = 12
        self.m = 12

        self.loc_speed = {}
        
    
    def get_now_speed(self,seq):
        pass
    def predict_speed(self,time_step):
        
        now_time = self.h* 60 + self.m
        pos = now_time/4
        
        speed = [0.0 for i in range(5) ]
        speed[0] = 1
        speed[1] = self.get_now_speed(roadid)

        pass 


    

def get_loc_from_file(file_name):
    fin = open(file_name)
    lines = []
    for line in fin:
        lines.append(line)
    fin.close()
    
    line_num = len(lines)
    pos = 0 
    print line_num 
    loc_dict = {}
    while pos < line_num :
        
        line = lines[pos]
        line_list = line.strip().split(' ')
        id = int(line_list[0]);locid = int(line_list[1]) ;pre_locid = int(line_list[2])
        
        loc = LocRoad(id,locid,pre_locid,360)
        
        pos += 1
        for i in range(pos,pos+7):
            l = lines[i].strip().split(" ")
            j = 0
            for speed in l :
                loc.history_road_speed[(i -pos) * times_day  + j ] = float(speed)
                j += 1
        pos += 7 

        for i in range(pos ,pos + 7):
            l = lines[i].strip().split(" ")
            j = 0 
            for weight in l :
                loc.weight[(i - pos) * 5  + j ] = float(weight) 
                j += 1
        pos += 7 
        
        loc_dict[locid] = loc 
    
    for each in loc_dict:
        print loc_dict[each].locid
    return loc_dict


if __name__ == "__main__":

    get_loc_from_file(loc_store_path)
    
