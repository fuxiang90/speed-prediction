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

def get_loc_from_file(file_name):
    fin = open(file_name)
    lines = []
    for line in fin:
        lines.append(line)
    fin.close()
    
    line_num = len(lines)
    pos = 0 
    print line_num 
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
    


if __name__ == "__main__":

    get_loc_from_file(loc_store_path)
    
