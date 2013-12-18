#!/usr/bin/env python 
# coding=utf-8
from config import loc_store_path, times_day  

from loc_store import LocHistoryRoad, LocRealRoad 
from road_network import ShortPath  
class LocRoad(object):

    def __init__ (self):
        
        self.history_road_dict =  dict()    #储存每个road的历史速度
        self.real_road_dict = dict()        #当前道路的实时速度

        self.lr_dict = dict() 

        self.road_network =  None 
