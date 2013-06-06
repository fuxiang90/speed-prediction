#ifndef LOC_STORE_H_INCLUDED
#define LOC_STORE_H_INCLUDED

#include "road.h"

/*
简单设计一个格式 ，保存 loc 的数据结构的内容，并且能从这个恢复

*/




void store_loc_in_file( LocRoad * loc_road_arr  );

void get_loc_from_file( LocRoad * loc_road_arr);
#endif // LOC_STORE_H_INCLUDED
