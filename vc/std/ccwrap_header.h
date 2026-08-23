/**
 *  @file ccwrap_header.h
 *  @brief  for MS Visual C/C++
 *  @author Masashi Kitamura(tenka@6809.net)
 *  @license Boost Software Lisence Version 1.0
 */
#pragma once



// ---------------------------------------------------------------------------
#if !defined(_MSC_VER)
#error for visual c/c++ header
#endif

#define _CCW_COMPILER                    "MS Visual C/C++ (" _CCW_M_STR(_MSC_VER) ")"

#define _CCW_CONFIG_DIR                  ../vc

#define _CCW_CONFIG_PATH(x)              <_CCW_CONFIG_DIR/x>

#ifndef _CCW_NATIVE_C_HEADER_DIR
  #if _MSC_VER >= 1900
    #define _CCW_NATIVE_C_HEADER_DIR    ../include
    #define _CCW_NATIVE_UC_HEADER_DIR   ../ucrt
    #define _CCW_NATIVE_STL_HEADER_DIR  ../include
  #elif _MSC_VER >= 1400
    #define _CCW_NATIVE_C_HEADER_DIR    ../../VC/include
    #define _CCW_NATIVE_UC_HEADER_DIR   ../../VC/include
  #elif _MSC_VER >= 1300
    #define _CCW_NATIVE_C_HEADER_DIR    ../../Vc7/include
    #define _CCW_NATIVE_UC_HEADER_DIR   ../../Vc7/include
  #elif _MSC_VER >= 1200
    #define _CCW_NATIVE_C_HEADER_DIR    ../../VC6/include
    #define _CCW_NATIVE_UC_HEADER_DIR   ../../VC6/include
  #endif
#endif

#ifndef _CCW_NATIVE_C_HEADER_PATH
  #define _CCW_NATIVE_C_HEADER_PATH(x)      <_CCW_NATIVE_C_HEADER_DIR/x>
#endif
#ifndef _CCW_NATIVE_UC_HEADER_PATH
  #define _CCW_NATIVE_UC_HEADER_PATH(x)     <_CCW_NATIVE_UC_HEADER_DIR/x>
#endif
#ifndef _CCW_NATIVE_STL_HEADER_DIR
  #define _CCW_NATIVE_STL_HEADER_DIR        _CCW_NATIVE_C_HEADER_DIR
#endif
#ifndef _CCW_NATIVE_STL_HEADER_PATH
  #define _CCW_NATIVE_STL_HEADER_PATH(x)    <_CCW_NATIVE_STL_HEADER_DIR/x>
#endif


#if defined(__cplusplus)
 #ifndef _CCW_TARGET_CXX
  //#if _MSVC_LANG >= 201103L
   //#define _CCW_TARGET_CXX    (_MSVC_LANG/100)
  //#else
   //#define _CCW_TARGET_CXX    2014    // vc14.0
  //#endif
  #define _CCW_TARGET_CXX       2026
 #endif
 #undef _CCW_TARGET_C
#else
 #ifndef _CCW_TARGET_C
  //#if __STDC_VERSION__ >= 201100L
   //#define _CCW_TARGET_C      (__STDC_VERSION__/100)
  //#else
   //#define _CCW_TARGET_C      2011
  //#endif
  #define _CCW_TARGET_C         2023
 #endif
 #undef _CCW_TARGET_CXX
#endif

// ---------------------------------------------------------------------------
// c & c++

#if defined(__cplusplus)
 #if _MSVC_LANG && __cplusplus < _MSVC_LANG
  #define _ccw_cplusplus     _MSVC_LANG
 #elif _MSC_VER >= 1900 && __cplusplus < 201402L
  #define _ccw_cplusplus     201402L
 //#elif _MSC_VER >= 1800
 // Since VC12 has significant shortcomings regarding C++11 support, treat it as C++03 plus extensions.
 //#define _ccw_cplusplus    201103L
 #else
  #define _ccw_cplusplus     __cplusplus
 #endif
#endif

#ifndef _CCW_M_CAT
 #define _CCW_M_CAT(a,b)        _CCW_M_CAT_S2(a,b)
 #define _CCW_M_CAT_S2(a,b)     _CCW_M_CAT_S3(a##b)
 #define _CCW_M_CAT_S3(x)       x
#endif

#ifndef _CCW_M_STR
 #define _CCW_M_STR(s)          _CCW_M_STR_S2(s)
 #define _CCW_M_STR_S2(s)       #s
#endif

#ifndef _CCW_NO_VA_ARGS
 #if _MSC_VER < 1400
  #define _CCW_NO_VA_ARGS       1
 #else
  #define _CCW_NO_VA_ARGS       0
 #endif
#endif

#define _CCW_HAS_PRAGMA_ONCE

#ifdef _CHAR_UNSIGNED
 #define _CCW_IS_CHAR_UNSIGNED  1
#else
 #define _CCW_IS_CHAR_UNSIGNED  0
#endif

#define _CCW_MESSAGE(x)         __pragma(message(x))

#if defined(__cplusplus)
  typedef bool                  _ccw_bool;
#elif __STDC_VERSION__ >= 199901L
  typedef _Bool	                _ccw_bool;
#else
  typedef unsigned char         _ccw_bool;
#endif

#if defined(__cpp_char8_t) // _MSC_VER >= 1922 || _MSVC_LANG >= 202000
  typedef char8_t               _ccw_char8;
#else
  typedef unsigned char         _ccw_char8;
#endif
#if defined(__cplusplus)
typedef wchar_t                 _ccw_wchar;
#else
typedef unsigned __int16        _ccw_wchar;
#endif

typedef signed __int8           _ccw_int8;
typedef unsigned __int8         _ccw_uint8;
typedef __int16                 _ccw_int16;
typedef unsigned __int16        _ccw_uint16;
typedef __int32                 _ccw_int32;
typedef unsigned __int32        _ccw_uint32;
typedef __int64                 _ccw_int64;
typedef unsigned __int64        _ccw_uint64;

#if _MSC_VER < 1600 // vc9 or earlier
 typedef __int64                _ccw_llong;
 typedef unsigned __int64       _ccw_ullong;
#else   //_MSC_VER >= 1600 // vc10 or later
 typedef long long              _ccw_llong;
 typedef unsigned long long     _ccw_ullong;
#endif

#if _ccw_cplusplus >= 201103L
typedef char16_t                _ccw_char16;
typedef char32_t                _ccw_char32;
#else
typedef _ccw_uint16             _ccw_char16;
typedef _ccw_uint32             _ccw_char32;
#endif

#define _ccw_bool               _ccw_bool
#define _ccw_char8              _ccw_char8
#define _ccw_char16             _ccw_char16
#define _ccw_char32             _ccw_char32
#define _ccw_wchar              _ccw_wchar
#define _ccw_int8               _ccw_int8
#define _ccw_uint8              _ccw_uint8
#define _ccw_int16              _ccw_int16
#define _ccw_uint16             _ccw_uint16
#define _ccw_int32              _ccw_int32
#define _ccw_uint32             _ccw_uint32
#define _ccw_int64              _ccw_int64
#define _ccw_uint64             _ccw_uint64
#define _ccw_llong              _ccw_llong
#define _ccw_ullong             _ccw_ullong
#define _ccw_llong              _ccw_llong
#define _ccw_ullong             _ccw_ullong

#if _MSC_VER < 1600
  #if !_CCW_NO_VA_ARGS
    #if !defined(_Pragma)
      #define _Pragma(...)      __pragma(__VA_ARGS__)
    #endif
  #else
    #if !defined(_Pragma)
      #define _Pragma(x)        __pragma(x)
    #endif
  #endif
#endif

#if _MSC_VER < 1900
  #define _XKEYCHECK_H
  #ifndef _CRT_SECURE_NO_WARNINGS
    #define _CRT_SECURE_NO_WARNINGS 1
  #endif
#endif

#define _ccw_forceinline        __forceinline
#define _ccw_noinline           __declspec(noinline)
//#define _ccw_selectany        __declspec(selectany)
#define _ccw_weak               __declspec(selectany)
#define _ccw_dllimport          __declspec(dllimport)
#define _ccw_dllexport          __declspec(dllexport)
#define _ccw_fastcall           __fastcall
#define _ccw_cdecl              __cdecl
#define _ccw_stdcall            __stdcall
#define _ccw_native_decl        __fastcall


// ---------------------------------------------------------------------------
// c++

#if defined(__cplusplus)

#if _MSC_VER >= 1914 && _MSVC_LANG && _MSVC_LANG != __cplusplus
 #pragma message("ccwrap: set /Zc:__cplusplus so __cplusplus follows /std:c++NN")
 //#warning from 19.29 (VS2019 16.10).
#endif

//#if _MSC_VER >= 1914 && ((__cplusplus / 100) < _CCW_TARGET_CXX)
// #pragma message("ccwrap: raise /std:c++NN so __cplusplus reaches _CCW_TARGET_CXX")
//#endif

#if _MSC_VER < 1400     // vc7.1 and earlier
 #if !defined(override)
  #define override
 #endif
 //#define WCHAR_MAX                ((wchar_t)-1)
#endif

#define _CCW_16_32_DEFINED
#if _MSC_VER < 1900     // vc12 and earlier: char16_t/char32_t are not native
 typedef _ccw_char16                char16_t;
 typedef _ccw_char32                char32_t;
#endif

#if _MSC_VER < 1600     // vc9 and earlier
 #if !defined(nullptr)
  #define nullptr                   (0)
 #endif
 #if !defined(static_assert)
  namespace __ccw { template<int x> struct static_assert_check{}; }
  template <bool x> struct __static_assert_FAILED_;
  template <> struct __static_assert_FAILED_<true> { enum { value = 1 }; };
  #if !_CCW_NO_VA_ARGS    // vc8, vc9
    #define static_assert(c, ...)   typedef __ccw::static_assert_check<sizeof(__static_assert_FAILED_<(c) != 0>)> _CCW_M_CAT(__ccw_static_assert_L_, __LINE__)
  #else   // vc7.1 and earlier
    #define static_assert(c, m)     typedef __ccw::static_assert_check<sizeof(__static_assert_FAILED_<(c) != 0>)> _CCW_M_CAT(__ccw_static_assert_L_, __LINE__)
  #endif
 #endif
#endif

#if _MSC_VER < 1700   // vc10 and earlier
 #define _CCW_NO_CHAR1632_T         0
 #if !defined(final) && _MSC_VER >= 1400 // vc8-10
  #define final                     sealed
 #else
  #define final
 #endif
#endif

#if _MSC_VER < 1900 // vc12 and earlier
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

#if _MSC_VER < 1900 // __cplusplus >= 201103L
 #define _ccw_noreturn              __declspec(noreturn)
 #define _ccw_deprecated            __declspec(deprecated)
 #define _ccw_carries_dependency
#else
 #define _ccw_noreturn              [[noreturn]]
 #define _ccw_deprecated            [[deprecated]]
 #define _ccw_carries_dependency    [[carries_dependency]]
#endif

#if _MSC_VER < 1910     // __cplusplus < 201703L
 #define _ccw_fallthrough
#else
 #define _ccw_fallthrough           [[fallthrough]]
#endif

#if _MSC_VER < 1913     // __cplusplus < 201703L
 #define _ccw_maybe_unused
#else
 #define _ccw_maybe_unused          [[maybe_unused]]
#endif

#if _MSC_VER < 1913     // __cplusplus < 202202L
 #define _ccw_nodiscard
#else
 #define _ccw_nodiscard             [[nodiscard]]
#endif

#if _MSC_VER >= 1700 && _MSC_VER < 1800
 #ifndef _VARIADIC_MAX
  #define _VARIADIC_MAX 10  // vc11:5
 #endif
#endif

// variadic templates: MSVC from vc12 (VS2013). 1 = not available.
#ifndef _CCW_NO_VARIADIC_TEMPLATES
 #if defined(__cpp_variadic_templates)
  #define _CCW_NO_VARIADIC_TEMPLATES 0
 #elif _MSC_VER >= 1800
  #define _CCW_NO_VARIADIC_TEMPLATES 0
 #elif !defined(_MSC_VER) && (__cplusplus >= 201103L)
  #define _CCW_NO_VARIADIC_TEMPLATES 0
 #else
  #define _CCW_NO_VARIADIC_TEMPLATES 1
 #endif
#endif

#if _MSC_VER >= 1900 && _MSC_VER < 1920 && !defined(_ENABLE_ATOMIC_ALIGNMENT_FIX)
 #define _ENABLE_ATOMIC_ALIGNMENT_FIX
#endif

#if !defined(__CCW_HAS_CHAR8_T)
 #if defined(__cpp_char8_t)
  #define __CCW_HAS_CHAR8_T        1
 #elif _CCW_TARGET_CXX >= 2020
  #define __CCW_HAS_CHAR8_T        1
  typedef _ccw_char8               char8_t;
 #endif
#endif

// user-defined literals: MSVC from vc14 (VS2015).
#ifndef _CCW_HAS_UDL
 #if defined(__cpp_user_defined_literals)
  #define _CCW_HAS_UDL 1
 #elif _MSC_VER >= 1900
  #define _CCW_HAS_UDL 1
 #elif !defined(_MSC_VER) && (__cplusplus >= 201103L)
  #define _CCW_HAS_UDL 1
 #else
  #define _CCW_HAS_UDL 0
 #endif
#endif

// alias templates ("template <class T> using X = ...;"): MSVC from vc12 (VS2013).
#ifndef _CCW_HAS_ALIAS_TEMPLATE
 #if defined(__cpp_alias_templates)
  #define _CCW_HAS_ALIAS_TEMPLATE 1
 #elif _MSC_VER >= 1800
  #define _CCW_HAS_ALIAS_TEMPLATE 1
 #elif !defined(_MSC_VER) && (__cplusplus >= 201103L)
  #define _CCW_HAS_ALIAS_TEMPLATE 1
 #else
  #define _CCW_HAS_ALIAS_TEMPLATE 0
 #endif
#endif

// variable templates: MSVC from vc14 (VS2015). 1 = available.
// vc14 rejects the brace-init form (C3376); write "const T<A> v = T<A>();".
#ifndef _CCW_HAS_VAR_TEMPLATE
 #if defined(__cpp_variable_templates)
  #define _CCW_HAS_VAR_TEMPLATE 1
 #elif _MSC_VER >= 1900
  #define _CCW_HAS_VAR_TEMPLATE 1
 #elif !defined(_MSC_VER) && (__cplusplus >= 201402L)
  #define _CCW_HAS_VAR_TEMPLATE 1
 #else
  #define _CCW_HAS_VAR_TEMPLATE 0
 #endif
#endif

// initializer_list: MSVC from vc12 (VS2013). 1 = available.
#ifndef _CCW_HAS_INIT_LIST
 #if defined(__cpp_initializer_lists)
  #define _CCW_HAS_INIT_LIST 1
 #elif _MSC_VER >= 1800
  #define _CCW_HAS_INIT_LIST 1
 #elif !defined(_MSC_VER) && (__cplusplus >= 201103L)
  #define _CCW_HAS_INIT_LIST 1
 #else
  #define _CCW_HAS_INIT_LIST 0
 #endif
#endif

// scoped enums (enum class): MSVC from vc11 (VS2012). 1 = available.
#ifndef _CCW_HAS_ENUM_CLASS
 #if defined(__cpp_scoped_enums) || _MSC_VER >= 1700
  #define _CCW_HAS_ENUM_CLASS 1
 #else
  #define _CCW_HAS_ENUM_CLASS 0
 #endif
#endif


#ifndef _ccw_constexpr11
 #if _ccw_cplusplus >= 201103L && (!defined(_MSC_VER) || _MSC_VER >= 1900)
  #define _ccw_constexpr11          constexpr
 #else
  #define _ccw_constexpr11
 #endif
#endif

#ifndef _ccw_constexpr17
 #if _ccw_cplusplus >= 201703L
  #define _ccw_constexpr17          constexpr
 #else
  #define _ccw_constexpr17
 #endif
#endif

// vc14 (VS2015) reports C++14 but has no relaxed constexpr (N3652): a constexpr
// body may hold only a single return statement, so this stays empty there.
#ifndef _ccw_constexpr14
 #if _ccw_cplusplus >= 201402L && _MSC_VER >= 1910
  #define _ccw_constexpr14          constexpr
 #else
  #define _ccw_constexpr14
 #endif
#endif

#ifndef _ccw_constexpr_or_const
 #if _ccw_cplusplus >= 201103L
  #define _ccw_constexpr_or_const   constexpr
 #else
  #define _ccw_constexpr_or_const   const
 #endif
#endif

#ifndef _ccw_inline_const
 #if defined(__cpp_inline_variables) && __cpp_inline_variables >= 201606L
  #define _ccw_inline_const         inline const
 #else
  #define _ccw_inline_const         extern __declspec(selectany) const
 #endif
#endif

#ifndef _ccw_inline_constant
 #if defined(__cpp_inline_variables) && __cpp_inline_variables >= 201606L
  #define _ccw_inline_constant      inline _ccw_constexpr_or_const
 #else
  #define _ccw_inline_constant      static _ccw_constexpr_or_const
 #endif
#endif

#ifndef _ccw_move_or_swap
 #if _ccw_cplusplus >= 201103L
  #define _ccw_move_or_swap(l,r)    ((l) = std::move(r))
 #else
  #define _ccw_move_or_swap(l,r)    swap((l), (r))
 #endif
#endif

#ifndef _ccw_move_or_copy
 #if _ccw_cplusplus >= 201103L
  #define _ccw_move_or_copy(l,r)    ((l) = std::move(r))
 #else
  #define _ccw_move_or_copy(l,r)    ((l) = (r))
 #endif
#endif

#ifndef _ccw_static_assert
 #define _ccw_static_assert(c)      static_assert((c), "`"#c "' is failed.")
#endif

namespace __ccw { namespace detail {} }

#else   // for c    ----------------------------------------------------------

#ifndef __STDC_NO_ATOMICS__
 #define __STDC_NO_ATOMICS__    1
#endif
#ifndef __STDC_NO_COMPLEX__
 #define __STDC_NO_COMPLEX__    1
#endif
#ifndef __STDC_NO_THREADS__
 #define __STDC_NO_THREADS__    1
#endif
#ifndef __STDC_NO_VLA__
 #define __STDC_NO_VLA__        1
#endif

#ifndef _Thread_local
 #define _Thread_local          __declspec(thread)
#endif
#ifndef _Static_assert
 #if !_CCW_NO_VA_ARGS
  #define _Static_assert(c, ...) typedef char _CCW_M_CAT(__static_assert_failed_L,__LINE__)[(c) ? 1 : -1]
 #else
  #define _Static_assert(c, m)  typedef char _CCW_M_CAT(__static_assert_failed_L,__LINE__)[(c) ? 1 : -1]
 #endif
#endif
#if !defined(_Alignof)
 #define _Alignof(t)            __alignof(t)
#endif
#ifndef _Alignas
 #define _Alignas(a)            __declspec(align(a))
#endif

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 201112L
 #ifndef _Noreturn
  #define _Noreturn             __declspec(noreturn)
 #endif
#endif
#if !defined(__func__)
 #define __func__               __FUNCTION__
#endif
#if !defined inline
 #define inline                 __inline
#endif
#if __STDC_VERSION__ < 199901L
 #if !defined _Bool
  #define _Bool                 _ccw_bool
 #endif
#endif

#if _CCW_TARGET_C >= 2023
 #ifndef thread_local
  #define thread_local          _Thread_local
 #endif
 #ifndef static_assert
  #define static_assert         _Static_assert
 #endif
 #ifndef alignof
  #define alignof(t)            _Alignof(t)
 #endif
 #ifndef alignas
  #define alignas(a)            _Alignas(a)
 #endif
 #ifndef bool
  #define bool                  _Bool
 #endif
 #ifndef true
  #define true                  1
 #endif
 #ifndef false
  #define false                 0
 #endif
#endif
//#if !defined restrict   //BAD: vc's header used __declspec(restrict)
 //#define restrict             __restrict  //__declspec(restrict)
//#endif
#endif


// ---------------------------------------------------------------------------
// macro

#if defined _M_X64 || defined _M_AMD64
  #define _CCW_CPU_X64
  #define _CCW_CPU_BIT          64
  #define _CCW_MEM_ACCESS_SAFE_REINTERPRET    1
#elif defined _M_IX86
  #define _CCW_CPU_X86
  #define _CCW_CPU_BIT          32
  #define _CCW_MEM_ACCESS_SAFE_REINTERPRET    1
#elif defined _M_ARM64 || defined _M_ARM64EC
  #define _CCW_CPU_ARM64
  #define _CCW_CPU_BIT          64
  #define _CCW_MEM_ACCESS_SAFE_REINTERPRET    1
#elif defined _M_ARM || defined _M_ARMT
  #define _CCW_CPU_ARM
  #define _CCW_CPU_BIT          32
#elif defined _M_MPPC
  #define _CCW_PPC
  #define _CCW_CPU_BIT          32
#elif defined _M_SH
  #define _CCW_CPU_SH
  #define _CCW_CPU_BIT          32
#elif defined _M_MRX00
  #define _CCW_CPU_MIPS
  #define _CCW_CPU_BIT          32
#elif defined _M_IA64
  #define _CCW_CPU_IA64
  #define _CCW_CPU_BIT          64
#endif
#ifndef _CCW_CPU_BIT
 #if defined(_WIN64)
  #define _CCW_CPU_BIT          64
 #else
  #define _CCW_CPU_BIT          32
 #endif
#endif

#define _CCW_INT_BIT            32
#define _CCW_WCHAR_BIT          16
#define _CCW_LONG_BIT           32
#define _CCW_LLONG_BIT          64
#define _CCW_DOUBLE_BIT         64
#define _CCW_LDOUBLE_BIT        64
#if _CCW_CPU_BIT == 64
 #define _CCW_PTR_BIT           64
#else
 #define _CCW_PTR_BIT           32
#endif

#if defined _CCW_BIG_ENDIAN
  #define _CCW_ENDIAN           1   // 0:little endian  1:big endian
#else
  #define _CCW_ENDIAN           0   // 0:little endian  1:big endian
#endif

#ifndef  _CCW_LONG_C
 #define _CCW_LONG_C(val)       val##L
 #define _CCW_ULONG_C(val)      val##UL
#endif
#ifndef  _CCW_LLONG_C
 #define _CCW_LLONG_C(val)      val##LL
 #define _CCW_ULLONG_C(val)     val##ULL
#endif

#if _CCW_INT_BIT < 32
 #define _CCW_INT32_C(v)        _CCW_LONG_C(v)
 #define _CCW_UINT32_C(v)       _CCW_ULONG_C(v)
#else
 #define _CCW_INT32_C(v)        v
 #define _CCW_UINT32_C(v)       v
#endif
#if   _CCW_LONG_BIT == 64
 #define _CCW_INT64_C(v)        _CCW_LONG_C(v)
 #define _CCW_UINT64_C(v)       _CCW_ULONG_C(v)
#elif _CCW_LLONG_BIT == 64
 #define _CCW_INT64_C(v)        _CCW_LLONG_C(v)
 #define _CCW_UINT64_C(v)       _CCW_ULLONG_C(v)
#endif

#if _MSC_VER < 1600 // vc9 or earlier
 #define _CCW_PFMT_LL           "I64"
 //#define _CCW_PFMT_WLL        L"I64"
#else
 #define _CCW_PFMT_LL           "ll"
 //#define _CCW_PFMT_WLL        L"ll"
#endif

// ---------------------------------------------------------------------------

#if (_MSC_VER < 1900) || (defined(_ccw_cplusplus) && (_ccw_cplusplus < 201103L))
 #ifndef _CCW_MAX_ALIGN_T_DEFINED
  #define _CCW_MAX_ALIGN_T_DEFINED
  typedef double  max_align_t;
 #endif
 #ifdef __cplusplus
  namespace std { using ::max_align_t; }
 #endif
#if defined(__cplusplus)
 #if defined(_CPPUNWIND)
  #define _CCW_HAS_EXCEPTIONS 1
 #else
  #define _CCW_HAS_EXCEPTIONS 0
 #endif
 #if defined(_CPPRTTI)
  #define _CCW_HAS_RTTI 1
 #else
  #define _CCW_HAS_RTTI 0
 #endif
#endif

#endif
