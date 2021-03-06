#ifndef ROAD_H_INCLUDED
#define ROAD_H_INCLUDED

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

using namespace std;

extern int *locid_hash;

//nav 表示导航道路
struct NavRoad_t{

    int start_id;
    int end_id;
    int road_seq;
    int locid ;//属于loc 地图中哪个seq
    double length;
};
typedef struct NavRoad_t NavRoad;

struct NavRoadKeyNode_t
{
    int node_seq;

    int num_out;                //节点的出度数
	int num_in;
    int road_out[6] ; //记录每条路的id
    int road_in [6] ;  //记录每条路的id
};
typedef struct NavRoadKeyNode_t NavRoadKeyNode;

struct LocRoad_t
{

    int locid;
    int pre_locid; //他的上一个locid
    int flag;
    //double  history_road[7][TIMES_DAY]; //这条道路 7天历史时刻，每个时间段的平均速度
    //直接开 内存浪费太多 在xp上有点跑不动
    double * * history_road; //但是如果频繁的调用malloc 开销有点大
    //double  weights_arr[7][5]; //每个对应一个权值


    //
    double  **weights_arr;

    int ** road_times_arr;
    std::vector< int > road_seq_vec; // nav road seq


};
typedef struct  LocRoad_t LocRoad;


//初始化 分配内存
void nav_road_init(NavRoad * *nav_road_arr , NavRoadKeyNode *  *nav_key_node_arr);

void nav_road_create(NavRoad * nav_road_arr , NavRoadKeyNode *  nav_key_node_arr);

void nav_road_release(NavRoad * nav_road_arr , NavRoadKeyNode *  nav_key_node_arr);

void nav_get_node_info(NavRoad * nav_road_arr , NavRoadKeyNode *  nav_key_node_arr);

//创建 一个 loc——road——arr 数组
LocRoad * loc_road_arr_create(int n);

void loc_road_arr_release(LocRoad * loc_road_arr ,int n );

//为locroad 里面的 history_road  weights_arr 分配内存   2013年6月8日
int loc_road_arr_node_malloc(LocRoad * loc_road);

int loc_road_arr_node_free(LocRoad * loc_road);

int loc_road_arr_node_free_road_times(LocRoad * loc_road);


//主要是将loc id  和 roadseq 对应起来 ，在相应的数据结构中赋值
void get_locid_seq( LocRoad * loc_road_arr  ,NavRoad  *nav_road_arr );

//void set_nav_road_locid( LocRoad * loc_road_arr ,map<int ,int> & locid_seq_map );


//返回 locid 的上一个 locid
int  nav_bfs(int locid , LocRoad * loc_road_arr  ,NavRoad  *nav_road_arr , NavRoadKeyNode *  nav_key_node_arr);


void check_locid_pre( LocRoad * loc_road_arr  ,NavRoad  *nav_road_arr , NavRoadKeyNode *  nav_key_node_arr);

void get_locid_pre( LocRoad * loc_road_arr  ,NavRoad  *nav_road_arr , NavRoadKeyNode *  nav_key_node_arr);
// just for test
void road_main();

#endif // ROAD_H_INCLUDED
