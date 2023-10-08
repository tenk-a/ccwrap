// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_STDINT_H_INCLUDED
#define __CCWRAP_STDINT_H_INCLUDED

#include <ccwrap_common.h>

#if __CCWRAP_C1990_OR_CXX1998 == 0 || __CCWRAP_HAS_STDINT

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <stdint.h>
//#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
//  #include __CCWRAP_NATIVE_UC_HEADER_PATH(stdint.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(stdint.h)
#else
  #include <../include/stdint.h>
#endif

#else
#include "../ccwrap/ccwrap_misc/stdint_impl.hh"
#endif

#include <_ccwrap_detail/stdint.hh>

#endif  // __CCWRAP_STDINT_H_INCLUDED
