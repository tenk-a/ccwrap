// Boost Software Lisence Version 1.0
#ifndef __CCWRAP_ASSERT_H_INCLUDED
#define __CCWRAP_ASSERT_H_INCLUDED

#include <ccwrap_common.h>

#if defined(__CCWRAP_HAS_INCLUDE_NEXT)
  #include_next <assert.h>
#elif defined(__CCWRAP_NATIVE_UC_HEADER_PATH)
  #include __CCWRAP_NATIVE_UC_HEADER_PATH(assert.h)
#elif defined(__CCWRAP_NATIVE_C_HEADER_PATH)
  #include __CCWRAP_NATIVE_C_HEADER_PATH(assert.h)
#else
  #include <../include/assert.h>
#endif

#if !defined(__cplusplus) && !defined(static_assert)
 #if !defined(__CCWRAP_NO_VA_ARGS)
  #define static_assert(c,...) _Static_assert(c,__VA_ARGS__)
 #else
  #define static_assert(c,m)   _Static_assert(c,m)
 #endif
#endif

#include <_ccwrap_detail/assert.hh>

#endif	// __CCWRAP_ASSERT_H_INCLUDED
