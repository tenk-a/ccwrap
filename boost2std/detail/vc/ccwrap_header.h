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

  
  vc14.0 or later : C headers
    C:\Program Files (x86)\Windows Kits\??\Include\??.?.?????.?\ucrt
*/

// ----------------------------------------------------------------------------
#if !defined(_MSC_VER)
#error for visual c/c++ header
#endif

#define __CCWRAP__  1

#define __CCWRAP_COMPILER                    "MS Visual C/C++ (" __CCWRAP_M_STR(_MSC_VER) ")"

#define __CCWRAP_CONFIG_DIR                  ../vc2013_or_earlier
#define __CCWRAP_CONFIG_PATH(x)              <__CCWRAP_CONFIG_DIR/##x>

#ifndef __CCWRAP_NATIVE_C_HEADER_DIR
  #if _MSC_VER >= 1900
    #define __CCWRAP_NATIVE_C_HEADER_DIR    ../include
    #define __CCWRAP_NATIVE_UC_HEADER_DIR   ../ucrt
  //#elif _MSC_VER >= 1900
  //  #define __CCWRAP_NATIVE_C_HEADER_DIR    ../../VC/include
  //  #define __CCWRAP_NATIVE_UC_HEADER_DIR   ../ucrt
  #elif _MSC_VER >= 1400
    #define __CCWRAP_NATIVE_C_HEADER_DIR    ../../VC/include
    #define __CCWRAP_NATIVE_UC_HEADER_DIR   ../../VC/include
  #elif _MSC_VER >= 1300
    #define __CCWRAP_NATIVE_C_HEADER_DIR    ../../Vc7/include
    #define __CCWRAP_NATIVE_UC_HEADER_DIR   ../../Vc7/include
  #elif _MSC_VER >= 1200
    #define __CCWRAP_NATIVE_C_HEADER_DIR    ../../VC6/include
    #define __CCWRAP_NATIVE_UC_HEADER_DIR   ../../VC6/include
  #endif
#endif

#ifndef __CCWRAP_NATIVE_C_HEADER_PATH
  #define __CCWRAP_NATIVE_C_HEADER_PATH(x)   <__CCWRAP_NATIVE_C_HEADER_DIR/##x>
#endif
#ifndef __CCWRAP_NATIVE_UC_HEADER_PATH
  #define __CCWRAP_NATIVE_UC_HEADER_PATH(x)   <__CCWRAP_NATIVE_UC_HEADER_DIR/##x>
#endif
#ifndef __CCWRAP_NATIVE_STL_HEADER_PATH
  #define __CCWRAP_NATIVE_STL_HEADER_PATH(x)   <__CCWRAP_NATIVE_C_HEADER_DIR/##x>
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

#if _MSC_VER < 1600 // vc9 or earlier
 typedef __int64                    __ccwrap_llong;
 typedef unsigned __int64           __ccwrap_ullong;
#else   //_MSC_VER >= 1600 // vc10 or later
 typedef long long                  __ccwrap_llong;
 typedef unsigned long long         __ccwrap_ullong;
#endif
#define __ccwrap_llong              __ccwrap_llong
#define __ccwrap_ullong             __ccwrap_ullong

#if _MSC_VER < 1600
  #if _MSC_VER >= 1400
    #if !defined(_Pragma)
      #define _Pragma(...)          __pragma(__VA_ARGS__)
    #endif
  #else
    #if !defined(_Pragma)
      #define _Pragma(x)            __pragma(x)
    #endif
  #endif
#endif

#if _MSC_VER < 1900
  #define _XKEYCHECK_H
#endif

#define __ccwrap_cdecl              __cdecl
#define __ccwrap_stdcall            __stdcall
#define __ccwrap_fastcall           __fastcall
#define __ccwrap_forceinline        __forceinline
#define __ccwrap_noinline           __declspec(noinline)
#define __ccwrap_selectany          __declspec(selectany)
#define __ccwrap_dllimport          __declspec(dllimport)
#define __ccwrap_dllexport          __declspec(dllexport)


// ----------------------------------------------------------------------------
// c++

#if defined(__cplusplus)

#if _MSVC_LANG && _MSVC_LANG > __cplusplus 
#define __CCWRAP_CXX	_MSVC_LANG
#else
#define __CCWRAP_CXX	__cplusplus
#endif

#if __CCWRAP_CXX >= 201101L || _MSC_VER >= 1900
 #define __CCWRAP_CXX11       1
#else
 #define __CCWRAP_LESS_CXX11  1
#endif
#if __CCWRAP_CXX >= 201402L
 #define __CCWRAP_CXX14       1
#else
 #define __CCWRAP_LESS_CXX14  1
#endif
#if __CCWRAP_CXX >= 201703L
 #define __CCWRAP_CXX17       1
#else
 #define __CCWRAP_LESS_CXX17  1
#endif
#if __CCWRAP_CXX >= 202002L
 #define __CCWRAP_CXX20       1
#else
 #define __CCWRAP_LESS_CXX20  1
#endif

#if _MSC_VER < 1400             // vc7.1 or earlier
 #define WCHAR_MAX             ((wchar_t)-1)
 #define __CCWRAP_NO_CXX11_VARIADIC_MACROS    1
 #if !defined(override)
  #define override
 #endif
#endif

#if _MSC_VER < 1600 // vc9 or earlier
 #define __CCWRAP_NO_CXX11_AUTO      1
 #define __CCWRAP_NO_DECLTYPE        1
 #define __CCWRAP_PFMT_LL            "I64"
 #define __CCWRAP_PFMT_WLL           L"I64"
 typedef unsigned                   char32_t;
 typedef unsigned short             char16_t;
 #if !defined(nullptr)
  #define nullptr                   (0)
 #endif
 #if !defined(static_assert)
  namespace __ccwrap { template<int x> struct static_assert_check{}; }
  template <bool x> struct __static_assert_FAILED_;
  template <> struct __static_assert_FAILED_<true> { enum { value = 1 }; };
  #if _MSC_VER >= 1400    // vc8, vc9
    #define static_assert(c, ...) typedef __ccwrap::static_assert_check<sizeof(__static_assert_FAILED_<(c) != 0>)> __CCWRAP_M_CAT(__ccwrap_static_assert_L_, __LINE__)
  #else   // vc7.1 or earlier
    #define static_assert(c, m)   typedef __ccwrap::static_assert_check<sizeof(__static_assert_FAILED_<(c) != 0>)> __CCWRAP_M_CAT(__ccwrap_static_assert_L_, __LINE__)
  #endif
 #endif
#endif

#if _MSC_VER < 1700   // vc10 or earlier
 #define __CCWRAP_NO_CHAR1632_T      0
 #if !defined(final) && _MSC_VER >= 1400 // vc8-10
  #define final                     sealed
 #else
  #define final
 #endif
#endif

#if _MSC_VER < 1900 // vc12 or earlier
 #if !defined(alignas)
  #define alignas(a)                __declspec(align(a))
 #endif
 #if !defined(alignof)
  #define alignof(a)                __alignof(a)
 #endif
 #if !defined(__func__)
  #define __func__                  __FUNCTION__
 #endif
 #if !defined(thread_local)
  #define thread_local              __declspec(thread)
 #endif
 #if !defined(noexcept)
  #define noexcept                  throw()
 #endif
 #if !defined(constexpr)
  #define constexpr
 #endif
#endif

#if defined(__cpp_char8_t) // _MSC_VER >= 1922 || _MSVC_LANG >= 202000
 #define __CCWRAP_HAS_CHAR8_T         1
#else
 #define __CCWRAP_HAS_CHAR8_T         0
 typedef unsigned char   char8_t;
#endif

#if _MSC_VER < 1900
#define __ccwrap_noreturn            __declspec(noreturn)
#define __ccwrap_deprecated          __declspec(deprecated)
#define __ccwrap_carries_dependency
#else
#define __ccwrap_noreturn            [[noreturn]]
#define __ccwrap_deprecated          [[deprecated]]
#define __ccwrap_carries_dependency  [[carries_dependency]]
#endif

#if _MSC_VER < 1910
#define __ccwrap_fallthrough
#else
#define __ccwrap_fallthrough         [[fallthrough]]
#endif

#if _MSC_VER < 1913
#define __ccwrap_maybe_unused
#define __ccwrap_nodiscard
#else
#define __ccwrap_maybe_unused        [[maybe_unused]]
#define __ccwrap_nodiscard           [[nodiscard]]
#endif


// ------------------------------
// c++ std header
// __CCWRAP_NO_.....      0 =>"boost2std" has   1 => Not available at all

// c++11 header
#if _MSC_VER < 1600 // vc9 or earlier
 #if _MSC_VER < 1400
  #define __CCWRAP_NO_HEADER_SYSTEM_ERROR      1
 #else
  #define __CCWRAP_NO_HEADER_SYSTEM_ERROR      0
 #endif
 #define __CCWRAP_NO_HEADER_CSTDINT            0
 #define __CCWRAP_NO_HEADER_CODECVT            1
 #define __CCWRAP_NO_HEADER_TYPEINDEX          0
 #define __CCWRAP_NO_CXX11_ALGORITHM           0
 #define __CCWRAP_NO_CXX11_COMPLEX             0
 #define __CCWRAP_NO_CXX11_FUNCTIONAL          0
 #define __CCWRAP_NO_CXX11_IOMANIP             0
 #define __CCWRAP_NO_CXX11_IOS                 1
 #define __CCWRAP_NO_CXX11_ITERATOR            0
 #define __CCWRAP_NO_CXX11_LIMITS              0
 #define __CCWRAP_NO_CXX11_MEMORY              0
 #define __CCWRAP_NO_CXX11_UTILITY             0
 #define __CCWRAP_NO_CXX11_LOCALE              1
 #define __CCWRAP_NO_CXX11_NUMERIC             0
 #define __CCWRAP_NO_CXX11_STRING              0
#endif
#if _MSC_VER < 1600 || (_MSC_VER == 1600 && defined __CCWRAP_NO_TR1)    // vc9 or earlier or no-TR1
 #define __CCWRAP_NO_HEADER_ARRAY              0
 #define __CCWRAP_NO_HEADER_FORWARD_LIST       0
 #define __CCWRAP_NO_HEADER_RANDOM             0
 #define __CCWRAP_NO_HEADER_REGEX              0
 #define __CCWRAP_NO_HEADER_TUPLE              0
 #define __CCWRAP_NO_HEADER_TYPE_TRAITS        0
 #define __CCWRAP_NO_HEADER_UNORDERED_MAP      0
 #define __CCWRAP_NO_HEADER_UNORDERED_SET      0
#endif
#if _MSC_VER < 1700   // vc10 or earlier
 #if _MSC_VER < 1400
  #define __CCWRAP_NO_HEADER_THREAD              1
  #define __CCWRAP_NO_HEADER_FUTURE              1
  #define __CCWRAP_NO_HEADER_MUTEX               1
 #else
  #define __CCWRAP_NO_HEADER_THREAD              0
  #define __CCWRAP_NO_HEADER_FUTURE              0
  #define BOOST_THREAD_PROVIDES_FUTURE
  #define __CCWRAP_NO_HEADER_MUTEX               0
 #endif
 #define __CCWRAP_NO_HEADER_ATOMIC               0
 #define __CCWRAP_NO_HEADER_CHRONO               0
 #define __CCWRAP_NO_HEADER_CONDITION_VARIABLE   0
 #define __CCWRAP_NO_HEADER_RATIO                0
 #define __CCWRAP_NO_HEADER_SCOPED_ALLOCATOR     0
#endif
#if _MSC_VER < 1800 // vc11 or earlier
 #define __CCWRAP_NO_HEADER_INITIALIZER_LIST     1
#endif

// c++14 header
#if _MSC_VER < 1800 // vc11 or earlier
 #define __CCWRAP_NO_HEADER_CXX14_ITERATOR       0
 #define __CCWRAP_NO_HEADER_CFENV                1
#endif
#if _MSC_VER < 1900   // vc12 or earlier
 #define __CCWRAP_NO_HEADER_CXX14_MEMORY         0
 #if _MSC_VER < 1400
  #define __CCWRAP_NO_HEADER_SHARED_MUTEX        1
 #else
  #define __CCWRAP_NO_HEADER_SHARED_MUTEX        0
 #endif
#endif

// c++17 header
#if _MSC_VER < 1900 // vc12 or earlier
 #define __CCWRAP_NO_HEADER_CXX17_ITERATOR       0
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
   #if _MSC_VER >= 1400
    #define _Static_assert(c,...)   typedef char __CCWRAP_M_CAT(__static_assert_failed_L,__LINE__)[(c) ? 1 : -1]
   #else
    #define _Static_assert(c, m)    typedef char __CCWRAP_M_CAT(__static_assert_failed_L,__LINE__)[(c) ? 1 : -1]
   #endif
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
  #if _MSC_VER < 1900
   #define __CCWRAP_NO_HEADER_STDBOOL
   #if !defined inline
    #define inline                  __inline
   #endif
   #if !defined _Bool
    #define _Bool                   char
   #endif
   #if !defined __CCWRAP_BOOL
    #define __CCWRAP_BOOL           int
   #endif
   #if _MSC_VER < 1400
    #define WCHAR_MAX               ((wchar_t)-1)
   #endif
   //#ifndef __STDC_VERSION__
    //#define __STDC_VERSION__      199409L
   //#endif
  #endif
#endif

#include "../ccwrap_header_sub.hh"
