#!/usr/bin/env python

import networkx as nx



class ShortPath(object):

    def __init__(self,road_file):
        self._road_file = road_file 
        self._mg = nx.MultiGraph()
    def run(self):
        
        for line in open(self._road_file):
            line_list = line.strip().split(' ')
            if len(line_list) != 5 : continue 
            
            self._mg.add_weighted_edges_from([ (int(line_list[1]) ,int(line_list[2]) , float(line_list[3]) )] )
            

        print nx.shortest_path(self._mg , 381 ,321)    
   

if __name__ == "__main__":

    test = ShortPath('../data/map_txt/nav-roadid.txt')


    test.run()
