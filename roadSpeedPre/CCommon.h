#ifndef CCOMMON_H_INCLUDED
#define CCOMMON_H_INCLUDED
/*
this file is from  my CSimpleLib
github: https://github.com/fuxiang90/CSimpleLib
*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef int  (*CompareDataFunc)     (void *  a, void *  b);
typedef void *  (*MallocDataFunc)      (int len);

typedef void (*SwapDataFunc)  (void * ,void * ,int );

#endif // CCOMMON_H_INCLUDED
