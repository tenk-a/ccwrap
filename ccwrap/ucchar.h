// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_UCHAR_H_INCLUDED
#define __CCWRAP_UCHAR_H_INCLUDED

#include <ccwrap_common.h>

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <uchar.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
  #include __CCWRAP_NATIVE_UC_HEADER_PATH(uchar.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(uchar.h)
#else
  #include <../include/uchar.h>
#endif

#include <_ccwrap_detail/uchar.hh>

#endif	// __CCWRAP_UCHAR_H_INCLUDED
