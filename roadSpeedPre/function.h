#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED


#include <stdio.h>
#include "config.h"


/*
一些功能函数 和时间的定义
*/

struct date_t{

    int year;
    int mouth;
    int day;

    int cmp( struct date_t * p);
    //date_t():year(0),mouth(0),day(0){}
};

const int MAX_STRLEN = 100;
const unsigned int DATA_LEN = 28;





/*
以2049年10月1日（100周年国庆）为例，用蔡勒（Zeller）公式进行计算，过程如下：
蔡勒（Zeller）公式：w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1
=49+[49/4]+[20/4]-2×20+[26× (10+1)/10]+1-1
=49+[12.25]+5-40+[28.6]
=49+12+5-40+28
=54 (除以7余5)

*/
/*http://www.dreamincode.net/forums/topic/44532-zellers-algorithm/*/

/*

*/
int ZellerFun(int yearnumber ,int monthnumber ,int daynumber);

/*39_0 将这个样的字符 转化为 数字 390 */
int get_locid_str(char * str);

void get_filename(int locid ,char str[]);

int get_dir_filenames( char  * * filename_arr ,int max_file_num);

void testFun();
#endif // FUNCTION_H_INCLUDED
