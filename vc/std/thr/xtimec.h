/**
 *  @file   thr/xtimec.h
 *  @brief  Shim over MSVC's internal <thr/xtimec.h> (vc11/12/13) that stops
 *          ccwrap's C11 `TIME_UTC` macro from corrupting the STL's `TIME_UTC`
 *          enumerator, while keeping `TIME_UTC` usable as the value 1.
 *  @license Boost Software License Version 1.0
 */
#pragma once

#include <ccwrap_common.h>

#if _MSC_VER < 1900
 #ifdef TIME_UTC
 #  undef TIME_UTC
 #endif
 #define _CCW_IN_XTIMEC 1
#endif

#include _CCW_NATIVE_STL_HEADER_PATH(thr/xtimec.h)

#if _MSC_VER < 1900
 #undef _CCW_IN_XTIMEC
 #define TIME_UTC TIME_UTC
#endif
