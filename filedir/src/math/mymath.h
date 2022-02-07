#ifndef _MYMATH_H_
#define _MYMATH_H_

#include <stdio.h>


#define mymax(x,y)  ((x)>(y)?(x):(y))
#ifdef __cplusplus
extern "C"{
#endif
int GetMax(int a,int b);
#ifdef __cplusplus
}
#endif

#endif
