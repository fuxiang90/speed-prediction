#!/usr/bin/env python
# coding:utf-8
import networkx as nx
import matplotlib.pyplot as plt


class ShortPath(object):

    def __init__(self,road_file):
        self._road_file = road_file 
        self._g = nx.Graph()
        self._roadnode_seq_dict = dict()
        self._seq_roadnode_dict = dict()
    def init(self):
        
        for i in range(1,12):
            self._g.add_node(i)
        for line in open(self._road_file):
            line_list = line.strip().split(' ')
            
            if len(line_list) != 5 : continue 
            #self._g.add_weighted_edges_from([ (int(line_list[1]) ,int(line_list[2]) , float(line_list[3]) )] )
            roadnode = (int(line_list[1]), int(line_list[2]))
            seq = int(line_list[0])

            if roadnode not in self._roadnode_seq_dict:
                self._roadnode_seq_dict[roadnode]  =  seq 

            if seq not in self._seq_roadnode_dict :
                self._seq_roadnode_dict[seq] = roadnode
            
            self._g.add_edge(int(line_list[1]), int(line_list[2]), length=float(line_list[3]) )
    def update_length(self):
        """
        更新权重 
        """
    def short_path(self, start,end):
        path = []
        try:
            path = nx.shortest_path(self._g , start ,end,weight='length')    
        except :
            print "have no path"
            path = []
        return path
        
        #paths = nx.get_all_shortest_paths(self._g,start, end) 
        #print paths 
     
    def show(self):
        
        G = self._g
        #pos = nx.circular_layout(G) 
        pos=nx.spring_layout(G) # positions for all nodes 
        
        pos = {1:(100,100),2:(200,100),3:(200,200),4:(300,200),8:(400,200),5:(300,100),9:(400,100),11:(500,100),7:(300,50),10:(400,50),6:(200,50)}
        nx.draw(G,pos) 
        #nx.draw_networkx_nodes(G,pos,node_size=80)
        #nx.draw_networkx_edges(G,pos,edge_color='r') 
        #nx.draw(G,pos=nx.spring_layout(G))
        #plt.axis('off')
        plt.show()

    def test(self):
        for i in range(100):
            line = raw_input()
            l = line.strip().split(' ')
            self.short_path(int(l[0]), int(l[1]) ) 
    
    def test2(self):
        path = self.short_path(1,11)
        
        print path 
        #print "->".join(path) 


        self._g[2][5]['length'] = 12222222222.0

        path = self.short_path(1,11)
        print path 
        
        #print "->".join(path)
        self._g[2][5]['length'] = 3372.0

        self._g[7][9]['length'] = 1222222222.0
        
        path = self.short_path(1,11)
        print path 

if __name__ == "__main__":

    test = ShortPath('../data/map_txt/nav-roadid-demo.txt')
    #test = ShortPath('../data/map_txt/nav-roadid.txt')
    #test = ShortPath('road_test')


    test.init()
    
    #test.show() ;  
    test.test2()
    #69047 69114
    """
    test.short_path(1,11)
    test._g[2][5]['length'] = 12222222222.0
    test.short_path(1,11)
    """
    #test.show()

    #test.test()
