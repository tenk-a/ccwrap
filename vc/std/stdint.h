/**
 *  @file   stdint.h
 *  @brief  stdint(typedef int??_t) for vc(<=14.??)
 *  @date   2000-2017,2023
 *  @author Masashi Kitamura (tenka@6809.net)
 *  @license Boost Software License Version 1.0
 */
#pragma once

#include <ccwrap_common.h>

#if _MSC_VER >= 1600        // vc10 or later

#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#include _CCW_NATIVE_C_HEADER_PATH(stdint.h)

#else   // _MSC_VER < 1600  // vc9 and earlier

typedef __int8              int8_t;
typedef unsigned __int8     uint8_t;
typedef __int16             int16_t;
typedef unsigned __int16    uint16_t;
typedef __int32             int32_t;
typedef unsigned __int32    uint32_t;
typedef __int64             int64_t;
typedef unsigned __int64    uint64_t;

#if _CCW_PTR_BIT == 64
 typedef int64_t            intptr_t;
 typedef uint64_t           uintptr_t;
#elif _CCW_PTR_BIT == 32
 typedef int32_t            intptr_t;
 typedef uint32_t           uintptr_t;
#endif

typedef int64_t             intmax_t;
typedef uint64_t            uintmax_t;

typedef int8_t              int_least8_t;
typedef uint8_t             uint_least8_t;
typedef int16_t             int_least16_t;
typedef uint16_t            uint_least16_t;
typedef int32_t             int_least32_t;
typedef uint32_t            uint_least32_t;
typedef int64_t             int_least64_t;
typedef uint64_t            uint_least64_t;

typedef int8_t              int_fast8_t;
typedef uint8_t             uint_fast8_t;
typedef int32_t             int_fast16_t;
typedef uint32_t            uint_fast16_t;
typedef int32_t             int_fast32_t;
typedef uint32_t            uint_fast32_t;
typedef int64_t             int_fast64_t;
typedef uint64_t            uint_fast64_t;

//#if defined(__STDC_LIMIT_MACROS) || defined(__STDC_CONSTANT_MACROS)

#define INT8_MIN            (-127-1)
#define INT8_MAX              127
#define UINT8_MAX             255

#define INT16_MIN           (-32767-1)
#define INT16_MAX             32767
#define UINT16_MAX            65535

#define INT32_MIN           (-2147483647-1)
#define INT32_MAX             2147483647
#define UINT32_MAX            0xFFFFFFFF

#if !defined(UINT64_MAX)
#define INT64_MIN           (-9223372036854775807LL-1LL)
#define INT64_MAX             9223372036854775807LL
#define UINT64_MAX             0xFFFFFFFFFFFFFFFFULL
#endif

#if defined(_ccw_int64)
 #define INTMAX_MIN         INT64_MIN
 #define INTMAX_MAX         INT64_MAX
 #define UINTMAX_MAX        UINT64_MAX
#else
 #define INTMAX_MIN         INT32_MIN
 #define INTMAX_MAX         INT32_MAX
 #define UINTMAX_MAX        UINT32_MAX
#endif

#if _CCW_CPU_BIT == 64
 #define INTPTR_MIN         INT64_MIN
 #define INTPTR_MAX         INT64_MAX
 #define UINTPTR_MAX        UINT64_MAX
 #define PTRDIFF_MIN        INT64_MIN
 #define PTRDIFF_MAX        INT64_MAX
#else
 #define INTPTR_MIN         INT32_MIN
 #define INTPTR_MAX         INT32_MAX
 #define UINTPTR_MAX        UINT32_MAX
 #define PTRDIFF_MIN        INT32_MIN
 #define PTRDIFF_MAX        INT32_MAX
#endif

#define INT_LEAST8_MIN      INT8_MIN
#define INT_LEAST8_MAX      INT8_MAX
#define UINT_LEAST8_MAX     UINT8_MAX

#define INT_LEAST16_MIN     INT16_MIN
#define INT_LEAST16_MAX     INT16_MAX
#define UINT_LEAST16_MAX    UINT16_MAX

#define INT_LEAST32_MIN     INT32_MIN
#define INT_LEAST32_MAX     INT32_MAX
#define UINT_LEAST32_MAX    UINT32_MAX

#define INT_LEAST64_MIN     INT64_MIN
#define INT_LEAST64_MAX     INT64_MAX
#define UINT_LEAST64_MAX    UINT64_MAX


#define INT_FAST8_MIN       INT8_MIN
#define INT_FAST8_MAX       INT8_MAX
#define UINT_FAST8_MAX      UINT8_MAX

#define INT_FAST16_MIN      INT32_MIN
#define INT_FAST16_MAX      INT32_MAX
#define UINT_FAST16_MAX     UINT32_MAX

#define INT_FAST32_MIN      INT32_MIN
#define INT_FAST32_MAX      INT32_MAX
#define UINT_FAST32_MAX     UINT32_MAX

#define INT_FAST64_MIN      INT64_MIN
#define INT_FAST64_MAX      INT64_MAX
#define UINT_FAST64_MAX     UINT64_MAX

#ifndef SIZE_MAX
#if _CCW_CPU_BIT == 64
#define SIZE_MAX            UINT64_MAX
#else
#define SIZE_MAX            UINT32_MAX
#endif
#endif

#define SIG_ATOMIC_MIN      INT32_MIN
#define SIG_ATOMIC_MAX      INT32_MAX

#if !defined(WCHAR_MIN)
 #define WCHAR_MIN          0
#endif
#if !defined(WCHAR_MAX)
 #define WCHAR_MAX          65535U
#endif
#if !defined(WINT_MIN)
#define WINT_MIN            0
#endif
#if !defined(WINT_MAX)
#define WINT_MAX            UINT16_MAX
#endif

#define INT8_C(val)         (val)
#define UINT8_C(val)        (val)
#define INT16_C(val)        (val)
#define UINT16_C(val)       (val)
#define INT32_C(val)        (INT32_MAX - INT32_MAX + (val))
#define UINT32_C(val)       (UINT32_MAX - UINT32_MAX + (val))
#define INT64_C(val)        (INT64_MAX - INT64_MAX + (val))     // val##LL
#define UINT64_C(val)       (UINT64_MAX - UINT64_MAX + (val))   // val##ULL
#define INTMAX_C(val)       (INTMAX_MAX - INTMAX_MAX + (val))   // val##LL
#define UINTMAX_C(val)      (UINTMAX_MAX - UINTMAX_MAX + (val)) // val##ULL

//#endif  // defined(__STDC_LIMIT_MACROS) || defined(__STDC_CONSTANT_MACROS)

#endif  // _MSC_VER @ 1600

// The *_WIDTH macros are C23 / C++23 and the UCRT has neither. _CCW_TARGET_C is only
// set in C mode, so C++ has to be asked for separately (same fix as vc/limits.h).
#if (_CCW_TARGET_C >= 2023) || (_CCW_TARGET_CXX >= 2023)
 #ifndef __STDC_VERSION_STDINT_H__
  #define __STDC_VERSION_STDINT_H__ 202311L
 #endif
 #ifndef INT8_WIDTH
  #define INT8_WIDTH         8
  #define UINT8_WIDTH        8
  #define INT16_WIDTH        16
  #define UINT16_WIDTH       16
  #define INT32_WIDTH        32
  #define UINT32_WIDTH       32
  #define INT64_WIDTH        64
  #define UINT64_WIDTH       64
  #define INT_LEAST8_WIDTH   8
  #define UINT_LEAST8_WIDTH  8
  #define INT_LEAST16_WIDTH  16
  #define UINT_LEAST16_WIDTH 16
  #define INT_LEAST32_WIDTH  32
  #define UINT_LEAST32_WIDTH 32
  #define INT_LEAST64_WIDTH  64
  #define UINT_LEAST64_WIDTH 64
  #define INT_FAST8_WIDTH    8
  #define UINT_FAST8_WIDTH   8
  #define INT_FAST16_WIDTH   32
  #define UINT_FAST16_WIDTH  32
  #define INT_FAST32_WIDTH   32
  #define UINT_FAST32_WIDTH  32
  #define INT_FAST64_WIDTH   64
  #define UINT_FAST64_WIDTH  64
  #define INTPTR_WIDTH       _CCW_PTR_BIT
  #define UINTPTR_WIDTH      _CCW_PTR_BIT
  #define INTMAX_WIDTH       64
  #define UINTMAX_WIDTH      64
  #define PTRDIFF_WIDTH      _CCW_PTR_BIT
  #define SIZE_WIDTH         _CCW_PTR_BIT
  #define SIG_ATOMIC_WIDTH   32
  #define WCHAR_WIDTH        16
  #define WINT_WIDTH         16
 #endif
#endif
