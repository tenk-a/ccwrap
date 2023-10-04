// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_ASSERT_H_INCLUDED
#define __CCWRAP_ASSERT_H_INCLUDED

#include <boost2std_common.hpp>

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <assert.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
  #include __CCWRAP_NATIVE_UC_HEADER_PATH(assert.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(assert.h)
#else
  #include <../include/assert.h>
#endif

#if !defined(__cplusplus)
 #if (__STDC_VERSION__ < 199900L
  #if !defined(static_assert)
   #if _MSC_VER < 1900
    #if _MSC_VER >= 1400
     #define static_assert(c,...) _Static_assert(c,__VA_ARGS__)
    #else
     #define static_assert(c,m)   _Static_assert(c,m)
    #endif
   #endif
  #endif
 #endif
#endif

#endif	// __CCWRAP_ASSERT_H_INCLUDED
