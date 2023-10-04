/**
 *  @file ccwrap_header.h
 *  @brief  for MS Visual C/C++
 *  @author Masashi Kitamura(tenka@6809.net)
 *  @license Boost Software Lisence Version 1.0
 */
#pragma once

/*  vs      vc      _MSC_VER
    6.0     6.0     1200
    2002    7.0     1300
    2003    7.1     1310
    2005    8.0     1400
    2008    9.0     1500
    2010    10.0    1600
    2012    11.0    1700
    2013    12.0    1800
    2015    14.0    1900
    2017    14.1    1910
    2019    14.2    1920
    2022    14.3    1930
*/

// ----------------------------------------------------------------------------
#if !defined(_MSC_VER)
#error for visual c/c++ header
#endif

#if !defined(__CCWRAP__)
#define __CCWRAP__  1
#endif

#define __CCWRAP_COMPILER                   "MS Visual C/C++ (" __CCWRAP_M_STR(_MSC_VER) ")"

#define __CCWRAP_CONFIG_DIR                 ../vc2015_or_later
#define __CCWRAP_CONFIG_PATH(x)             <__CCWRAP_CONFIG_DIR/##x>

#ifndef __CCWRAP_NATIVE_C_HEADER_DIR
  #if _MSC_VER >= 1910
    #define __CCWRAP_NATIVE_C_HEADER_DIR    ../include
    #define __CCWRAP_NATIVE_UC_HEADER_DIR   ../ucrt
  #else //if _MSC_VER >= 1900
    #define __CCWRAP_NATIVE_C_HEADER_DIR    ../../VC/include
    #define __CCWRAP_NATIVE_UC_HEADER_DIR   ../ucrt
  #endif
#endif

#ifndef __CCWRAP_NATIVE_C_HEADER_PATH
  #define __CCWRAP_NATIVE_C_HEADER_PATH(x)  <__CCWRAP_NATIVE_C_HEADER_DIR/##x>
#endif
#ifndef __CCWRAP_NATIVE_UC_HEADER_PATH
  #define __CCWRAP_NATIVE_UC_HEADER_PATH(x) <__CCWRAP_NATIVE_UC_HEADER_DIR/##x>
#endif
#ifndef __CCWRAP_NATIVE_STL_HEADER_PATH
  #define __CCWRAP_NATIVE_STL_HEADER_PATH(x) <__CCWRAP_NATIVE_C_HEADER_DIR/##x>
#endif

// ----------------------------------------------------------------------------
// c & c++

#define __CCWRAP_WCHAR_BIT          16
#define __CCWRAP_LONG_BIT           32
#define __CCWRAP_LDOUBLE_BIT        64

#define __CCWRAP_HAS_PRAGMA_ONCE

#ifndef __CCWRAP_M_CAT
  #define __CCWRAP_M_CAT(a,b)       __CCWRAP_M_CAT_S2(a,b)
  #define __CCWRAP_M_CAT_S2(a,b)    __CCWRAP_M_CAT_S3(a##b)
  #define __CCWRAP_M_CAT_S3(x)      x
#endif

#ifdef _CHAR_UNSIGNED
 #define __CCWRAP_IS_CHAR_UNSIGNED  1
#else
 #define __CCWRAP_IS_CHAR_UNSIGNED  0
#endif

#define __ccwrap_cdecl               __cdecl
#define __ccwrap_stdcall             __stdcall
#define __ccwrap_fastcall            __fastcall
#define __ccwrap_force_inline        __forceinline
#define __ccwrap_noinline            __declspec(noinline)
#define __ccwrap_selectany           __declspec(selectany)
#define __ccwrap_dllimport           __declspec(dllimport)
#define __ccwrap_dllexport           __declspec(dllexport)


// ----------------------------------------------------------------------------
// c++

#if defined(__cplusplus)

#if __cplusplus >= 201101 || _MSVC_LANG >= 201101 || _MSC_VER >= 1900
 #define __CCWRAP_CXX11       1
#else
 #define __CCWRAP_LESS_CXX11  1
#endif
#if __cplusplus >= 201402 || _MSVC_LANG >= 201402
 #define __CCWRAP_CXX14       1
#else
 #define __CCWRAP_LESS_CXX14  1
#endif
#if __cplusplus >= 201703 || _MSVC_LANG >= 201703
 #define __CCWRAP_CXX17       1
#else
 #define __CCWRAP_LESS_CXX17  1
#endif
#if __cplusplus >= 202000 || _MSVC_LANG >= 202000
 #define __CCWRAP_CXX20       1
#else
 #define __CCWRAP_LESS_CXX20  1
#endif

#if defined(__cpp_char8_t) // _MSC_VER >= 1922 || _MSVC_LANG >= 202000
 #define __CCWRAP_HAS_CHAR8_T         1
#else
 #define __CCWRAP_HAS_CHAR8_T         0
 typedef unsigned char   char8_t;
#endif

#define __ccwrap_noreturn            [[noreturn]]
#define __ccwrap_deprecated          [[deprecated]]
#define __ccwrap_carries_dependency  [[carries_dependency]]

#if _MSC_VER < 1910
#define __ccwrap_fallthrough
#else
#define __ccwrap_fallthrough         [[fallthrough]]
#endif

#if !defined(__CCWRAP_CXX17) //_MSC_VER < 1913 || !defined(_HAS_CXX17)
#define __ccwrap_maybe_unused
#define __ccwrap_nodiscard
#else
#define __ccwrap_maybe_unused        [[maybe_unused]]
#define __ccwrap_nodiscard           [[nodiscard]]
#endif


// ------------------------------
// c++ std header
// __CCWRAP_NO_.....      0 =>"boost2std" has   1 => Not available at all

// c++17 header
#if _MSC_VER < 1910 // vc11 or earlier
// #define __CCWRAP_NO_HEADER_CXX17_ITERATOR       0
#endif

#if !defined(__CCWRAP_CXX17)
 #define __CCWRAP_NO_HEADER_STRING_VIEW          0
 #define __CCWRAP_NO_HEADER_FILESYSTEM           0
 #define __CCWRAP_NO_CXX17_ALGORITHM             0
 #define __CCWRAP_NO_HEADER_ANY                  0
 #define __CCWRAP_NO_HEADER_VARIANT              0
 #define __CCWRAP_NO_HEADER_OPTIONAL             0
#endif

// c++20 header


#else   // for c    -----------------------------------------------------------
  #define __STDC_NO_VLA__
  #define __STDC_NO_COMPLEX__
  //#if !defined restrict   //BAD: vc's header used __declspec(restrict)
    //#define restrict              __restrict  //__declspec(restrict)
  //#endif
  #ifndef _Thread_local
    #define _Thread_local           __declspec(thread)
  #endif
  #ifndef _Static_assert
    #define _Static_assert(c,...)   typedef char __CCWRAP_M_CAT(__static_assert_failed_L,__LINE__)[(c) ? 1 : -1]
  #endif
  #if !defined(_Alignof)
    #define _Alignof(t)             __alignof(t)
  #endif
  #ifndef _Alignas
    #define _Alignas(a)             __declspec(align(a))
  #endif
  #ifndef _Noreturn
    #define _Noreturn               __declspec(noreturn)
  #endif
  #if !defined(__func__)
   #define __func__                 __FUNCTION__
  #endif
#endif

#include "../_ccwrap/ccwrap_header_sub.hh"
