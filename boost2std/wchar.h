// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_WCHAR_H_INCLUDED
#define __CCWRAP_WCHAR_H_INCLUDED

#include <boost2std_common.hpp>

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <wchar.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
  #include __CCWRAP_NATIVE_UC_HEADER_PATH(wchar.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(wchar.h)
#else
  #include <../include/wchar.h>
#endif

#endif
