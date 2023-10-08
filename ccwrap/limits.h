// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_LIMITS_H_INCLUDED
#define __CCWRAP_LIMITS_H_INCLUDED

#include <ccwrap_common.h>

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <limits.h>
//#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
//  #include __CCWRAP_NATIVE_UC_HEADER_PATH(limits.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(limits.h)
#else
  #include <../include/limits.h>
#endif

#include <_ccwrap_detail/limits.hh>

#endif
