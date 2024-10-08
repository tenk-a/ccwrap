/**
 *  @file stdint_macro_impl.hh
 *  @brief stdint.h's macro
 *  @author Masashi Kitamura (tenka@6809.net)
 *  @license Boost Software License Version 1.0
 */
#if !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS)
#pragma once

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

#if !defined(__CCWRAP_NO_INT64)
 #define INTMAX_MIN         INT64_MIN
 #define INTMAX_MAX         INT64_MAX
 #define UINTMAX_MAX        UINT64_MAX
#else
 #define INTMAX_MIN         INT32_MIN
 #define INTMAX_MAX         INT32_MAX
 #define UINTMAX_MAX        UINT32_MAX
#endif

#if defined(__CCWRAP_CPU64)
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

#define INT_FAST16_MIN      INT16_MIN
#define INT_FAST16_MAX      INT16_MAX
#define UINT_FAST16_MAX     UINT16_MAX

#define INT_FAST32_MIN      INT32_MIN
#define INT_FAST32_MAX      INT32_MAX
#define UINT_FAST32_MAX     UINT32_MAX

#define INT_FAST64_MIN      INT64_MIN
#define INT_FAST64_MAX      INT64_MAX
#define UINT_FAST64_MAX     UINT64_MAX

#ifndef SIZE_MAX
#ifdef _FKSTD_CPU64
#define SIZE_MAX            UINT64_MAX
#else
#define SIZE_MAX            UINT32_MAX
#endif
#endif

#define SIG_ATOMIC_MIN      INT32_MIN
#define SIG_ATOMIC_MAX      INT32_MAX


#if __CCWRAP_WCHAR_BIT == 32
 #if !defined(WCHAR_MIN)
  #define WCHAR_MIN         0
 #endif
 #if !defined(WCHAR_MAX)
  #define WCHAR_MAX         UINT32_MAX
 #endif
 #define WINT_MIN           UINT32_MIN
 #define WINT_MAX           UINT32_MAX
#else
 #if !defined(WCHAR_MIN)
  #define WCHAR_MIN         0
 #endif
 #if !defined(WCHAR_MAX)
  #define WCHAR_MAX         65535U
 #endif
 #define WINT_MIN           UINT16_MIN
 #define WINT_MAX           UINT16_MAX
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

#endif
