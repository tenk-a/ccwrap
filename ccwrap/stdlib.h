// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_STDLIB_H_INCLUDED
#define __CCWRAP_STDLIB_H_INCLUDED

#include <ccwrap_common.h>

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <stdlib.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
  #include __CCWRAP_NATIVE_UC_HEADER_PATH(stdlib.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(stdlib.h)
#else
  #include <../include/stdlib.h>
#endif

#include <_ccwrap_detail/stdlib.hh>

#endif	// __CCWRAP_STDLIB_H_INCLUDED
