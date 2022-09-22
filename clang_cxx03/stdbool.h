/**
 *  @file   stdbool.h
 *  @brief  Boolean type and values
 */
#ifndef __CCWRAP_STDBOOL_H_INCLUDED
#define __CCWRAP_STDBOOL_H_INCLUDED

#include <ccwrap_header.h>

#if 0

#include_next <stdbool.h>

#else

#if !defined(bool) // || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
 #define bool                   _Bool
 #define true                   1
 #define false                  0
 #define __bool_is_defined      1
#else
 #error not impliment stdbool.h
#endif

#endif

#endif
