# coding=utf-8
#! /usr/bin/python

import yaml 
import os

from road_network import ShortPath
from loc_train import LocTrain
from loc_real import LocRealRoad 
from loc_history_road import LocHistoryRoad
train_root = '/home/fuxiang/speed-prediction/data/train'
csv_root = '/home/fuxiang/speed-prediction/data/csv'
class Manager(object):


    def __init__(self,conf_path):


        self._history_road_object = {}  #储存道路的历史速度的对象
        self._real_road_object = {}     # 道路的实时 
        self._lr_mode = {}              # 道路的预测模型
        self._network = None 

        self._pre_road = {}
        

        self.init(conf_path)
    def init(self,conf_path):
        d = yaml.load(open(conf_path) )
        
        print d 

        self._id_locid_dict = d['roadid']

        # 加载道路tuopu 
        
        network_file_path = d['network_file']
        self._network  = ShortPath(network_file_path)
        
        # 加载pre 
        #

        pre_file_path = d ['loc_pre_file']
        self.set_pre_road(pre_file_path)
        
        # 初始化 线性回归模型
        #

        for id in self._id_locid_dict:
            id = int(id)
            locid = self._id_locid_dict[id]

            file_name = os.path.join( train_root ,str(locid)+'_data')
            self._lr_mode[id] = LocTrain(file_name,"ridge")
        
        #加载实时
        for id in self._id_locid_dict:
            id = int(id)
            locid = self._id_locid_dict[id]

            file_name = os.path.join(csv_root , str(locid)+'.csv')
            self._real_road_object[id] = LocRealRoad(file_name)

            self._history_road_object = LocHistoryRoad(file_name) 

    def set_pre_road(self, loc_pre_file_path):
        f = open(loc_pre_file_path) 
        for line in f :
            line_list = line.strip().split(' ')
            
            id1 = int(line_list[0]) 
            id2 = int(line_list[1])

            self._pre_road[id1] = id2
    
    def update_weight(self,year_tuple, h, m ):
        """
            取得每条道路的实时速度，和预测的速度 ，预测28min后的 
        """

    def get_real_road_speed(self,year_tuple,h,m):
        real_road_speed_dict = {}
        for id in self._real_road_object:
            speed = self._real_road_object[id].get_speed(year_tuple,h,m)
            real_road_speed_dict [id] = speed

        return real_road_speed_dict
    
    def get_history_road_speed(self,year, h, m):
        
        history_road_speed_dict = {}
        for id in self._history_road_object :
            speed = self._history_road_object[id].get_speed(year_tuple,h,m)
            history_road_speed_dict [id] = speed

        return history_road_speed_dict
    def get_predict_road_speed(self,year,h,m):

        
            
    def run():
        
        """
        1. 从某个时间开始
        2. 每次递增4min 
        3. 模拟车辆的位置，是否通过了这条道路
        4. 不断计算车辆到最终点的路径
        """
        
        for id in self._lr_mode :
            self._lr_mode[id].train() 

        start_year = 2013
        start_mou = 6
        start_day = 12
        
        start_h = 12
        start_min = 0
        min_step = 4

        start_pos = 1
        end_pos = 11 

        for i in range(10):


if __name__ == '__main__':
    t = Manager('./conf/main.yaml')
