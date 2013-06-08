#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED


#include "function.h"
#include "CSlist.h"

#include "road.h"

#include <string>
#include <assert.h>


using namespace std;
//#include "fGradient_descent.h"
//将从数据库中提取的道路数据 转换成我所要的格式


//多长时间 为一个间隔
//TIMES_DAY 那么一天可以分割多少个间隔
const int STEP = 4;
const int TIMES_DAY = 24*60 / STEP;

////一条道路 一天每个时间的速度
//typedef struct road_day_t  RoadDay;
//struct road_day_t {
//    double speed_arr[TIMES_DAY];
//
//};







typedef struct road_t  Road;
struct road_t{
    double speed_arr[TIMES_DAY];
    int times[TIMES_DAY];
    int weekday;//记录这个信息是星期几
    //struct date_t mtime;
};


typedef struct road_info_t  RoadInfo;
struct road_info_t{
    //struct road_t * road_arr;
    CSlist * road_list;//记录这条路 每一天的速度
    struct date_t mtime;//string mtime;//起始时间
    std::string road_str;
    //int len ; //记录了多少条路
    int road_id;

    struct road_info_t * pre;//他上一条道路
    struct road_info_t * next;//他下一条道路

    //double  history_road[7][TIMES_DAY]; //这条道路 7天历史时刻，每个时间段的平均速度
    //int road_times_arr[7][TIMES_DAY];  //记录这条道路 每个时刻 有多少次数据

    int flag ; //表示有history的数据

};
/* api */

RoadInfo * RoadInfoCreat();
/* 从一个文件中读取 一段连续的路的属性*/
RoadInfo * RoadInfoInput(RoadInfo * head );

void RoadInfoRealease(RoadInfo * head);

//释放 head 中list 的内存
void RoadInfoClistRealease(RoadInfo * head);

void RoadInfoShow(RoadInfo * head,char *out_name);




void getInfoStr(char * in_str ,RoadInfo * head,struct date_t * now_time, struct  LocRoad_t * loc_road );
void getInfoStr(char * in_str ,RoadInfo * head,struct date_t * mtime   );

/*test*/

void testInput();

#endif // INPUT_H_INCLUDED
