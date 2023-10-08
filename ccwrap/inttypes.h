// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_INTTYPES_H_INCLUDED
#define __CCWRAP_INTTYPES_H_INCLUDED

#include <ccwrap_common.h>

#if __CCWRAP_C1990_OR_CXX1998 == 0
 #if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <inttypes.h>
 #elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
  #include __CCWRAP_NATIVE_UC_HEADER_PATH(inttypes.h)
 #elif defined(__CCWRAP_NATIVE_STL_HEADER_PATH)
  #include __CCWRAP_NATIVE_STL_HEADER_PATH(inttypes.h)
 #else
  #include <../include/inttypes.h>
 #endif
//#else
// #include <ccwrap_misc/inttypes_impl.hh>
#endif

#include <_ccwrap_detail/inttypes.hh>

#endif
