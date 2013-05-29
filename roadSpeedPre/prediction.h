#ifndef PREDICTION_H_INCLUDED
#define PREDICTION_H_INCLUDED


// 这个文件主要是提供预测这个功能 ，全部的接口函数在这里完成

/*
1 之前的loc可以得到 所有loc id 之前的locid
2 这里把所有有csv 导成txt 的数据处理得到 一个道路 的历史信息
3 把历史信息整理成  386 2 25.300000 25.300000 25.685714 27.861429 30.272857  txt
在 数据结构 double  history_road[7][TIMES_DAY];  中对伊哦哪个

4 可以生成对应的txt or ，将这样的数据传给 梯度下降 得到 一个参数权重的数组

5 生成查询接口
*/

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <map>
#include <list>
#include <vector>
#include "config.h"

#include "function.h"
#include "input.h"
#include "road.h"
#include "gradient_descent.h"

using namespace std;


/*
生成 一个 roadinfo的数组
*/
RoadInfo * predict_init();


void get_road_info(RoadInfo * road_info_arr);

void get_road_info_filename(RoadInfo * head , char *filename );


//生成所有的history_road 的数据
void get_road_info_history(RoadInfo  *road_info_arr);

void get_road_info_history_node(RoadInfo  *road_info ,RoadInfo * pre_road_info, LocRoad * loc_road_arr ) ;

void road_info_process(RoadInfo  *road_info_arr );

static void get_info_str(char * in_str ,RoadInfo * road_info);
// test

//把历史数据 拷贝到 loc 中,
void copy_history_roadinfo_locroado(RoadInfo  *road_info_arr , LocRoad * loc_road_arr );
void prediction_train_loc_road(LocRoad * loc_road_arr  );
void predict_main();

#endif // PREDICTION_H_INCLUDED
