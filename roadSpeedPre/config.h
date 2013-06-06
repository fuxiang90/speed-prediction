#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <vector>



/*
*如要在linux 下编译 请加上这句#define OS_LINUX
*如果在windows 下 请注释掉
*/
#define OS_LINUX 1

#ifdef OS_LINUX
#include <dirent.h>
#include <sys/stat.h>
#endif

#define  MAX_OUT_ROAD 6
#define  MAX_FILE_NAME_LEN 30
#define  MAX_TXT_NAME_LEN 20


extern const int MAX_ROAD;

extern const int MAX_KEY_NODE;

extern const int MAX_LOC_ROAD ;
extern const char * NAV_ROAD_FILE ;
extern const char * LOCID_SEQ_FILE;
extern const char * txt_root_path ;
extern const char * txt_cycle_path;
#endif // CONFIG_H_INCLUDED
