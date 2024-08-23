/**
 *  @file   stdint_impl.hh
 *  @brief  stdint(typedef int??_t) for vc(<=15.??)
 *  @date   2000-2017,2023
 *  @author Masashi Kitamura (tenka@6809.net)
 *  @license Boost Software License Version 1.0
 */
#ifndef __CCWRAP_STDINT_IMPL_HH_DEFINED
#define __CCWRAP_STDINT_IMPL_HH_DEFINED

#if __CCWRAP_C1990_OR_CXX1998  && !__CCWRAP_HAS_STDINT

#include <ccwrap_common.h>

typedef signed char         int8_t;
typedef unsigned char       uint8_t;
typedef short               int16_t;
typedef unsigned short      uint16_t;
typedef int                 int32_t;
typedef unsigned int        uint32_t;
typedef __ccwrap_int64	    int64_t;
typedef __ccwrap_uint64	    uint64_t;

#if defined(__CCWRAP_CPU64)
 typedef int64_t            intptr_t;
 typedef uint64_t           uintptr_t;
#else
 typedef int32_t            intptr_t;
 typedef uint32_t           uintptr_t;
#endif

#if !defined(__CCWRAP_NO_INT64)
 typedef int64_t            intmax_t;
 typedef uint64_t           uintmax_t;
#else
 typedef int32_t            intmax_t;
 typedef uint32_t           uintmax_t;
#endif

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
typedef int16_t             int_fast16_t;
typedef uint16_t            uint_fast16_t;
typedef int32_t             int_fast32_t;
typedef uint32_t            uint_fast32_t;
typedef int64_t             int_fast64_t;
typedef uint64_t            uint_fast64_t;


#include "stdint_macro_impl.hh"


#endif  // !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS)


#endif	// __CCWRAP_STDINT_IMPL_HH_DEFINED
