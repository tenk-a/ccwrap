// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_STRING_H_INCLUDED
#define __CCWRAP_STRING_H_INCLUDED

#include <ccwrap_common.h>

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <string.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
  #include __CCWRAP_NATIVE_UC_HEADER_PATH(string.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(string.h)
#else
  #include <../include/string.h>
#endif

#include <_ccwrap_detail/string.hh>


#endif	// __CCWRAP_STRING_H_INCLUDED
