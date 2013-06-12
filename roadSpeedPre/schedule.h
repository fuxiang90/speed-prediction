
/*
得到loc road 所依赖的上一条路，得到一个安全并节省内存的访问loc.txt的顺序

主要算法 拓扑排序


locid --> pre_locid  ==> pre_locid 指向 locid 的边 ，即 pre_locid要先执行
*/

#ifndef SCHEDULE_H_INCLUDED
#define SCHEDULE_H_INCLUDED

#include "config.h"
#include "road.h"
#include <queue>

#define MAX_DEGREE 6
//static int MAX_LOC_ROAD = 50000;
struct ScheduleGraph_t{

    int edge_arr[50000][MAX_DEGREE];
    int indegree[50000];
    int degree[50000];
};

typedef struct ScheduleGraph_t ScheduleGraph;

ScheduleGraph * graph_init();

void graph_release(ScheduleGraph * graph) ;


void set_graph_info(ScheduleGraph * graph ,LocRoad * loc_road_arr);
void graph_topo_sort(int * * schedule_arr  ,int *n , ScheduleGraph * graph  );



void schedule_main();
//schedule




#endif // SCHEDULE_H_INCLUDED
