// ccwrap <uchar.h>
#pragma once

#include <ccwrap_common.h>

#include <stddef.h>
#include <stdint.h>

#ifndef _CCW_CHAR8_16_32_DEFINED
 #define _CCW_CHAR8_16_32_DEFINED
 #ifndef __cplusplus
  typedef unsigned char   char8_t;
  typedef uint_least16_t  char16_t;
  typedef uint_least32_t  char32_t;
  #ifndef __CCW_HAS_CHAR8_T
   #define __CCW_HAS_CHAR8_T 1
  #endif
 #endif
#endif

#ifdef __cplusplus
 #ifndef __CCW_HAS_CHAR8_T
  #define __CCW_HAS_CHAR8_T 1
  typedef unsigned char   char8_t;
 #endif
#endif

#include "../../detail/c/uchar_impl.h"
