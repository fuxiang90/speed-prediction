#!/usr/bin/env python 
# coding=utf-8
from config import loc_store_path, times_day  

from loc_store import LocHistoryRoad, LocRealRoad 
from road_network import ShortPath  
from history_road import HistoryRoad 
class LocRoad(object):

    def __init__ (self):
        
        #self.history_road_dict =  dict()    #储存每个road的历史速度
        self._history_road =  dict()    #储存每个road的历史速度
        self._real_road_dict = dict()        #当前道路的实时速度

        self._lr_dict = dict() 

        self._road_network =  None 
