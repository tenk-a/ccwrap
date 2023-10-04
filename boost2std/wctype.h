// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_WCTYPE_H_INCLUDED
#define __CCWRAP_WCTYPE_H_INCLUDED

#include <boost2std_common.hpp>

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <wctype.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
  #include __CCWRAP_NATIVE_UC_HEADER_PATH(wctype.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(wctype.h)
#else
  #include <../include/wctype.h>
#endif

#endif
