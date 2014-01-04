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

            self._history_road_object[id] = LocHistoryRoad(file_name) 

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
        road_len = len(self._network._g.edges() )
        
        #这里的roadid 以0开始 ，路网中的是1 开始
        for road_id in range(road_len) :
            
            now_speed = self._real_road_object[road_id+1].get_speed(year_tuple,h,m)
            first_pre_speed , pre_speed = self.get_predict_road_speed(year_tuple,h,m,road_id+1)
            speed_list = [now_speed,first_pre_speed, pre_speed]
            self._network.update_weight(road_id,speed_list) 

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
    def get_predict_road_speed(self,year,h,m,road_id,time_pre = 28):
        

        
        pre_road_id = self._pre_road.get(road_id, road_id)
        real_speed = self.get_real_road_speed(year,h,m)
        now_road_id_speed = real_speed[road_id]
        now_pre_road_id_speed = real_speed[pre_road_id]
        
        
        

        for i in range(time_pre/4 - 1):
            #history_speed = self.get_history_road_speed(year,h,m)
            history_road_id_speed = self._history_road_object[road_id].get_speed(year,h,m)
            history_pre_road_id_speed = self._history_road_object[pre_road_id].get_speed(year,h,m)
            
            speeds = [1, now_road_id_speed,now_pre_road_id_speed, history_road_id_speed,history_pre_road_id_speed]

            
            now_road_id_speed = self._lr_mode[road_id].predict(1,speeds)
            if i == 0: 
                first_pre_speed = now_road_id_speed
            m += 4
            if m >= 60:
                h += 1 
                m %= 60 
        return first_pre_speed,now_road_id_speed 

    
    def get_lr_mode(self):
        for id in self._lr_mode :
            self._lr_mode[id].train() 

            
    def run(self):
        
        """
        1. 从某个时间开始
        2. 每次递增4min 
        3. 模拟车辆的位置，是否通过了这条道路
        4. 不断计算车辆到最终点的路径
        """
        self.get_lr_mode()

        start_year = 2012
        start_mou = 11
        start_day = 26
        
        start_h = 17
        start_m = 00
        
        min_step = 00

        start_pos = 1
        end_pos = 11 
        
        now_pass_len = 0

        year_tuple = (start_year,start_mou,start_day)

        print "run is begin "
        print self._network._g.edges() 
        for i in range(100):
            path = self._network.short_path(start_pos,end_pos)
            
            if len(path) == 2 and now_pass_len > self._network.get_road_id_length(start_pos,end_pos): break
            
            next_node = path[1] 
            
            pos = 1 
            while pos < len(path) and pos +1 < len(path) and  now_pass_len > self._network.get_road_id_length(start_pos,next_node ):
                now_pass_len -= self._network.get_road_id_length(start_pos,next_node )  
                start_pos = path[pos]
                next_node = path[pos+1] 
                pos += 1 

            road_id = self._network.get_road_id(start_pos,next_node) + 1
            now_speed = self._real_road_object[road_id].get_speed( year_tuple, start_h,start_m)

            
            self.update_weight(year_tuple,start_h,start_m) 
            now_pass_len += now_speed * 4.0/60 * 1000  ;
            start_m += 4 
            if start_m >= 60 :
                start_m %= 60 
                start_m += 1 
            print "===============================",i 
            print "现在是%d:%d ,车辆在道路road<%d,%d> ,道路的速度是%f "%(start_h,start_m,start_pos,next_node,now_speed)  
            print "现在到终点的路径是" ,path[pos:]   
            
            

if __name__ == '__main__':
    t = Manager('./conf/main.yaml')
    

    """
    t.get_lr_mode() 
    print t.get_predict_road_speed((2012,12,26),17,0,2)
    """ 

    t.run()
