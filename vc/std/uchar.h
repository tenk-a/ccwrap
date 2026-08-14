// ccwrap <uchar.h>
#pragma once

#include <ccwrap_common.h>

#if _MSC_VER >= 1900
 #include _CCW_NATIVE_UC_HEADER_PATH(uchar.h)
#else
 #include <stddef.h>
 #include <stdint.h>
 #include <wchar.h>

 #ifndef __cplusplus
  typedef unsigned char char8_t;
  typedef uint_least16_t char16_t;
  typedef uint_least32_t char32_t;
 #else
  #if !defined(__cpp_char8_t)
   typedef unsigned char char8_t;
  #endif
 #endif

 #include <../../detail/c/uchar_impl.h>
#endif

#ifndef __STDC_VERSION_UCHAR_H__
 #define __STDC_VERSION_UCHAR_H__ 202311L
#endif

#if _MSC_VER >= 1900 && !defined(__cpp_char8_t)
 typedef unsigned char char8_t;
#endif
