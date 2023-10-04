// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_CTYPE_H_INCLUDED
#define __CCWRAP_CTYPE_H_INCLUDED

#include <boost2std_common.hpp>

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <ctype.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
  #include __CCWRAP_NATIVE_UC_HEADER_PATH(ctype.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(ctype.h)
#else
  #include <../include/ctype.h>
#endif

#endif
