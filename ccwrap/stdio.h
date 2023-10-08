// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_STDIO_H_INCLUDED
#define __CCWRAP_STDIO_H_INCLUDED

#include <ccwrap_common.h>

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <stdio.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
  #include __CCWRAP_NATIVE_UC_HEADER_PATH(stdio.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(stdio.h)
#else
  #include <../include/stdio.h>
#endif

#include <_ccwrap_detail/stdio.hh>

#endif	// __CCWRAP_STDIO_H_INCLUDED
