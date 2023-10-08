// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_WCHAR_H_INCLUDED
#define __CCWRAP_WCHAR_H_INCLUDED

#include <ccwrap_common.h>

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <wchar.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
  #include __CCWRAP_NATIVE_UC_HEADER_PATH(wchar.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(wchar.h)
#else
  #include <../include/wchar.h>
#endif

#include <_ccwrap_detail/wchar.hh>

#endif	// __CCWRAP_WCHAR_H_INCLUDED
