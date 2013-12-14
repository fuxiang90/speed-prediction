# coding=utf-8
#! /usr/bin/python

from pybrain.tools.shortcuts import buildNetwork
from pybrain.datasets import SupervisedDataSet
from pybrain.supervised.trainers import BackpropTrainer

fnn = buildNetwork(4,3,2,1,bias = True)

ds = SupervisedDataSet(4,1)

fin = open("train")
train = []
train_label = []
for line in fin :
    line_list = line.strip().split(' ')
    if len(line_list) != 5 : 
        continue 
    a = [line_list[x] for x in range(4)]
    b = [line_list[4]]
    ds.addSample(a,b)

trainer = BackpropTrainer(fnn, ds)
trainer.train()

fin = open("test")
test = SupervisedDataSet(4,1)
for line in fin :
    line_list = line.strip().split(' ')
    if len(line_list) != 5 : 
        continue 
    a = [line_list[x] for x in range(4)]
    b = [ 0 ]
    #print trainer.activate(a)
    test.addSample(a,b)

out = fnn.activateOnDataset(test)
for each in out :
    print each 



