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
#include "schedule.h"
#include "loc_store.h"

using namespace std;


/*
生成 一个 roadinfo的数组
*/
RoadInfo * predict_roadinfo_create();

void predict_init();

void get_road_info(RoadInfo * road_info_arr);

int get_road_info_filename(RoadInfo * head , char *filename  ,  LocRoad * loc_road_arr ,int loc_pos);


//生成所有的history_road 的数据
void get_road_info_history(RoadInfo  *road_info_arr);

void get_road_info_history_node(RoadInfo  *road_info ,RoadInfo * pre_road_info, LocRoad * loc_road_arr ) ;

void road_info_process_one(RoadInfo * head ,LocRoad * loc_road);

//得到历史数据
void loc_road_info_process_one( LocRoad * loc_road);

void road_info_process(RoadInfo  *road_info_arr );

static void get_info_str(char * in_str ,double * speed ,int * weekday, int * h  ,int * m);
// test

//把历史数据 拷贝到 loc 中,
void copy_history_roadinfo_locroad(RoadInfo  *road_info_arr , LocRoad * loc_road_arr );

int prediction_train_loc_road(double * *speed_arr  ,
LocRoad * loc_road_arr ,int weekday ,int h ,int m ,double speed ,int locid ,int next_time );

//查询下一个时刻的
double  prediction_train_loc_road_next(LocRoad * loc_road_arr ,
    int weekday ,int h ,int m ,double speed ,int locid ,int pre_time );

//查询指定时间之后 的历史数据
int  history_train_loc_road( double * speed_arr ,
LocRoad * loc_road_arr ,int weekday ,int h ,int m ,int locid ,int next_time );

//test 检查 梯度下降的准确性
void check_train_loc_road(LocRoad * loc_road_arr );


//根据调度信息 分别得到各个道路的训练模型
void schedule_train_loc_road(int * schedule_arr  ,int n, LocRoad * loc_road_arr ,RoadInfo  *road_info_arr );

//训练单个道路 并把权值放在loc_road_arr 里面
void train_loc_road(int locid ,int pre_locid , RoadInfo  *road_info_arr,LocRoad * loc_road_arr);

/*
从文件中读loc信息
*/
void predict_main();


// 使用调度算法的预测
void predict_schedule_main();

void predict_test(LocRoad * loc_road_arr);

#endif // PREDICTION_H_INCLUDED
