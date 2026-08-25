// ccwrap <uchar.h>
#pragma once

#include <ccwrap_common.h>

#if defined(__cplusplus)

#if defined(__has_include_next)
 #if __has_include_next(<uchar.h>)
  #include_next <uchar.h>
 #endif
#else
 #include_next <uchar.h>
#endif

#if !defined(__APPLE__) || defined(_UCHAR_H) || defined(_UCHAR_H_) || \
    defined(_INC_UCHAR) || defined(__STDC_VERSION_UCHAR_H__)
 #define __CCW_UCHAR_NATIVE_H 1
#endif

#if !defined(__cpp_char8_t)
 #ifndef __CCW_HAS_CHAR8_T
  #define __CCW_HAS_CHAR8_T   1
  typedef _ccw_char8          char8_t;
 #endif
#endif

#ifndef __STDC_VERSION_UCHAR_H__
 #ifdef __CCW_UCHAR_NATIVE_H
  #define __CCW_UCHAR_IMPL_C8_ONLY 1
 #endif
 #include <../../detail/c/uchar_impl.h>
#endif

#else

#include <stddef.h>      /* size_t */
//#include <stdint.h>    /* uint_least16_t / uint_least32_t */

#ifndef _CCW_16_32_DEFINED
 #define _CCW_16_32_DEFINED
 typedef _ccw_char16        char16_t;
 typedef _ccw_char32        char32_t;
#endif
#ifndef __CCW_HAS_CHAR8_T
 #define __CCW_HAS_CHAR8_T  1
 typedef _ccw_char8         char8_t;
#endif

#include "../../detail/c/uchar_impl.h"

#endif // __cplusplus

#ifndef __STDC_VERSION_UCHAR_H__
 #define __STDC_VERSION_UCHAR_H__ 202311L
#endif
