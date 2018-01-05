/*
	¹«ÓÃÍ·
*/
#ifndef __COMMON_HEAD_H__
#define __COMMON_HEAD_H__

#include <assert.h>
#include <string>
#include <time.h>
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <map>
#include <sstream>
#include <stack>
#include <queue>

#include "Platform.h"
#include "LogManager/LogManager.h"


typedef unsigned int		UnInt32_t;
typedef unsigned char		UnChar_t;
typedef unsigned long		UnLong_t;
typedef unsigned long int	UnLongInt_t;
typedef long long			LongLong_t;

#define MING_SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define MING_BREAK_IF(cond)			  if(cond) break


#ifdef LOGDEBUG
	#define MINGLOG			LOGDEBUG
#else
	#undef MINGLOG
	#define MINGLOG			printf
#endif

#ifdef LOGERROR
	#define MINGLOGERROR			LOGERROR
#else
	#undef MINGLOGERROR
	#define MINGLOGERROR			printf
#endif


#define MING_SYNTHESIZE_READONLY(varType, varName, funName)\
protected: varType varName; \
public: virtual varType get##funName(void) const { return varName; }

#define MING_SYNTHESIZE(varType, varName, funName)\
protected: varType varName; \
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

#define MINGASSERT(cond, msg) do { \
if (!(cond)) { \
printf(msg); \
assert(cond); \
} \
} while (0)


using namespace std;

#endif //__COMMON_HEAD_H__
