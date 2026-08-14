// ccwrap <limits.h>
#pragma once

#include <ccwrap_common.h>

#include _CCW_NATIVE_C_HEADER_PATH(limits.h)

#if _MSC_VER < 1300
#define LLONG_MIN           (-9223372036854775807LL-1LL)
#define LLONG_MAX             9223372036854775807LL
#define ULLONG_MAX            0xFFFFFFFFFFFFFFFFULL
#endif

#if (_CCW_TARGET_C >= 2023) || (_CCW_TARGET_CXX >= 2023)
 #ifndef BOOL_WIDTH
  #define BOOL_WIDTH         1
  #define CHAR_WIDTH         CHAR_BIT
  #define SCHAR_WIDTH        CHAR_BIT
  #define UCHAR_WIDTH        CHAR_BIT
  #define SHRT_WIDTH         16
  #define USHRT_WIDTH        16
  #define INT_WIDTH          32
  #define UINT_WIDTH         32
  #define LONG_WIDTH         32
  #define ULONG_WIDTH        32
  #define LLONG_WIDTH        64
  #define ULLONG_WIDTH       64
 #endif
#endif
