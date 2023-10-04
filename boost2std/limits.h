// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_LIMITS_H_INCLUDED
#define __CCWRAP_LIMITS_H_INCLUDED

#include <boost2std_common.hpp>

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <limits.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
 #include __CCWRAP_NATIVE_UC_HEADER_PATH(limits.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(limits.h)
#else
  #include <../include/limits.h>
#endif

#if !defined(LLONG_MIN)
#define LLONG_MIN   	    (-9223372036854775807LL-1LL)
#define LLONG_MAX   	      9223372036854775807LL
#define ULLONG_MAX  	      0xFFFFFFFFFFFFFFFFULL
#endif

#endif
