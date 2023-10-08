// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_STDDEF_H_INCLUDED
#define __CCWRAP_STDDEF_H_INCLUDED

#include <ccwrap_common.h>

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <stddef.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
  #include __CCWRAP_NATIVE_UC_HEADER_PATH(stddef.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(stddef.h)
#else
  #include <../include/stddef.h>
#endif

#endif
