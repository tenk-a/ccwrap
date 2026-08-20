// ccwrap <uchar.h>
#pragma once

#include <ccwrap_common.h>

#if _MSC_VER >= 1900
 #include _CCW_NATIVE_UC_HEADER_PATH(uchar.h)

 #if !defined(__cpp_char8_t)
  #ifndef __CCW_HAS_CHAR8_T
   #define __CCW_HAS_CHAR8_T    1
   typedef _ccw_char8           char8_t;
  #endif
 #endif

 #ifndef __STDC_VERSION_UCHAR_H__
  #define __CCW_UCHAR_IMPL_C8_ONLY 1
  #include <../../detail/c/uchar_impl.h>
 #endif
#else
 #include <stddef.h>

 #ifndef _CCW_16_32_DEFINED
  #define _CCW_16_32_DEFINED
  typedef _ccw_char16        char16_t;
  typedef _ccw_char32        char32_t;
 #endif
 #ifndef __CCW_HAS_CHAR8_T
  #define __CCW_HAS_CHAR8_T  1
  typedef _ccw_char8         char8_t;
 #endif

 #include <../../detail/c/uchar_impl.h>
#endif

#ifndef __STDC_VERSION_UCHAR_H__
 #define __STDC_VERSION_UCHAR_H__ 202311L
#endif
