#ifndef __CCWRAP_ASSERT_H_INCLUDED
#define __CCWRAP_ASSERT_H_INCLUDED

#include <ccwrap_header.h>

#include_next <assert.h>

#ifndef __cplusplus
 #if __STDC_VERTION < 201100
   #define static_assert(c,...) _Static_assert(c,__VA_ARGS__)
 #endif
#endif

#endif
