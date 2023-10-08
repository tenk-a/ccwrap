// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_MATH_H_INCLUDED
#define __CCWRAP_MATH_H_INCLUDED

#include <ccwrap_common.h>


#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <math.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
 #include __CCWRAP_NATIVE_UC_HEADER_PATH(math.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(math.h)
#else
  #include <../include/math.h>
#endif

#include <_ccwrap_detail/math.hh>

#endif
