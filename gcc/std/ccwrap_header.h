/**
 *  @file ccwrap_header.h
 *  @brief  for GCC g++ / Clang clang++ (mainly msys2 ucrt64, WSL Ubuntu; -std=c++03)
 *  @author Masashi Kitamura(tenka@6809.net)
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_CCWRAP_HEADER_H
#define _CCW_CCWRAP_HEADER_H
#pragma once

// ---------------------------------------------------------------------------
#if !defined(__GNUC__) && !defined(__clang__)
#error for GCC/Clang header
#endif

#ifdef _MSC_VER
#undef _MSC_VER
#endif

#ifndef __STDC_LIMIT_MACROS
 #define __STDC_LIMIT_MACROS    1
#endif
#ifndef __STDC_CONSTANT_MACROS
 #define __STDC_CONSTANT_MACROS 1
#endif
#ifndef __STDC_FORMAT_MACROS
 #define __STDC_FORMAT_MACROS   1
#endif

#if defined(__cplusplus) && (defined(__MINGW32__) || defined(__MINGW64__))
 #ifndef _CONST_RETURN
  #define _CONST_RETURN         const
  #define _CCW_MINGW_CONST_RETURN 1
  #ifndef __CORRECT_ISO_CPP_WCHAR_H_PROTO
   #define __CORRECT_ISO_CPP_WCHAR_H_PROTO 1
  #endif
 #endif
#endif

#if defined(__clang__)
 #define _CCW_COMPILER      "LLVM Clang (" _CCW_M_STR(__clang_major__) "." _CCW_M_STR(__clang_minor__) ")"
#else
 #define _CCW_COMPILER      "GNU g++ (" _CCW_M_STR(__GNUC__) "." _CCW_M_STR(__GNUC_MINOR__) ")"
#endif

#define _CCW_CONFIG_DIR                 ../gcc
#define _CCW_CONFIG_PATH(x)             <_CCW_CONFIG_DIR/x>


#if defined(__cplusplus)
 #ifndef _CCW_TARGET_CXX
  #define _CCW_TARGET_CXX       2026
 #endif
 #undef _CCW_TARGET_C
#else
 #ifndef _CCW_TARGET_C
  #define _CCW_TARGET_C         2023
 #endif
 #undef _CCW_TARGET_CXX
#endif

// ---------------------------------------------------------------------------

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
 #define _CCW_NO_VA_ARGS       0
#endif

#define _CCW_HAS_PRAGMA_ONCE

#if defined(__CHAR_UNSIGNED__)
 #define _CCW_IS_CHAR_UNSIGNED  1
#else
 #define _CCW_IS_CHAR_UNSIGNED  0
#endif

#define _CCW_MESSAGE(x)         _Pragma(_CCW_M_STR(message(x)))

#if defined(__cplusplus)
  typedef bool                  _ccw_bool;
#elif __STDC_VERSION__ >= 199901L
  typedef _Bool	                _ccw_bool;
#else
  typedef unsigned char         _ccw_bool;
#endif

#if defined(__cpp_char8_t)
 typedef char8_t                _ccw_char8;
#else
 typedef unsigned char          _ccw_char8;
#endif

#if __cplusplus
typedef wchar_t                 _ccw_wchar;
#else
typedef __WCHAR_TYPE__          _ccw_wchar;   /* gcc/clang builtin: the platform wchar_t */
#endif

typedef signed char             _ccw_int8;
typedef unsigned char           _ccw_uint8;
typedef short                   _ccw_int16;
typedef unsigned short          _ccw_uint16;
typedef int                     _ccw_int32;
typedef unsigned int            _ccw_uint32;
typedef long long               _ccw_int64;
typedef unsigned long long      _ccw_uint64;
typedef long long               _ccw_llong;
typedef unsigned long long      _ccw_ullong;

#if __cplusplus >= 201103L
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

#define _ccw_forceinline        inline __attribute__((always_inline))
#define _ccw_noinline           __attribute__((noinline))
#define _ccw_weak               __attribute__((weak))
#define _ccw_dllimport          __attribute__((dllimport))
#define _ccw_dllexport          __attribute__((dllexport))
#define _ccw_fastcall
#define _ccw_cdecl
#define _ccw_stdcall
#define _ccw_native_decl

// ---------------------------------------------------------------------------
// c

#if !defined(__cplusplus)

#if __STDC_VERSION__ < 199901L
#define inline                  __inline
#define _Bool                   _ccw_bool
#endif
#endif

// ---------------------------------------------------------------------------
// c++

#if defined(__cplusplus)

#define _ccw_cplusplus  __cplusplus

#if __cplusplus < 201103L      // C++03: supply C++11 keyword/type shims
 #if !defined(nullptr)
  #define nullptr                   __null
 #endif

 #if !defined(static_assert)
  namespace __ccw { template<int x> struct static_assert_check {}; }
  template <bool x> struct __static_assert_FAILED_;
  template <> struct __static_assert_FAILED_<true> { enum { value = 1 }; };
  #define static_assert(c, ...)     typedef __ccw::static_assert_check<sizeof(__static_assert_FAILED_<(c) != 0>)> _CCW_M_CAT(__ccw_static_assert_L_, __LINE__)
 #endif

 // char16_t/char32_t are C++11; at -std=c++03 g++ does not provide them.
 #if !defined(__cpp_unicode_characters) && !defined(_CHAR16T)
  typedef unsigned short            char16_t;
  typedef unsigned int              char32_t;
  #define _CHAR16T
 #endif

 #if !defined(override)
  #define override
 #endif
 #if !defined(final)
  #define final
 #endif
 #if !defined(constexpr)
  #define constexpr
 #endif
 #if !defined(noexcept)
  #define noexcept                  throw()
 #endif
 #if !defined(alignas)
  #define alignas(a)                __attribute__((aligned(a)))
 #endif
 #if !defined(alignof)
  #define alignof(a)                __alignof__(a)
 #endif
 #if !defined(thread_local)
  #define thread_local              __thread
 #endif
 #if !defined(__func__)
  #define __func__                  __FUNCTION__
 #endif

#endif  // __cplusplus < 201103L

#if __cplusplus < 201103L
 #define _ccw_noreturn             __attribute__((noreturn))
 #define _ccw_deprecated           __attribute__((deprecated))
 #define _ccw_carries_dependency
#else
 #define _ccw_noreturn             [[noreturn]]
 #define _ccw_deprecated           [[deprecated]]
 #define _ccw_carries_dependency   [[carries_dependency]]
#endif

#if __cplusplus < 201703L
 #define _ccw_fallthrough
 #define _ccw_maybe_unused
 #define _ccw_nodiscard
#else
 #define _ccw_fallthrough          [[fallthrough]]
 #define _ccw_maybe_unused         [[maybe_unused]]
 #define _ccw_nodiscard            [[nodiscard]]
#endif

#if _CCW_TARGET_CXX >= 2020 && !defined(__cpp_char8_t)
 #ifndef __CCW_HAS_CHAR8_T
  #define __CCW_HAS_CHAR8_T        1
  typedef _ccw_char8               char8_t;
 #endif
#endif

#ifndef _CCW_NO_VARIADIC_TEMPLATES
 #if defined(__cpp_variadic_templates) || (__cplusplus >= 201103L)
  #define _CCW_NO_VARIADIC_TEMPLATES 0
 #else
  #define _CCW_NO_VARIADIC_TEMPLATES 1
 #endif
#endif

#ifndef _CCW_HAS_UDL
 #if defined(__cpp_user_defined_literals) || (__cplusplus >= 201103L)
  #define _CCW_HAS_UDL          1
 #else
  #define _CCW_HAS_UDL          0
 #endif
#endif

#ifndef _CCW_HAS_ALIAS_TEMPLATE
 #if defined(__cpp_alias_templates) || (__cplusplus >= 201103L)
  #define _CCW_HAS_ALIAS_TEMPLATE   1
 #else
  #define _CCW_HAS_ALIAS_TEMPLATE   0
 #endif
#endif

#ifndef _CCW_HAS_VAR_TEMPLATE
 #if defined(__cpp_variable_templates) || (__cplusplus >= 201402L)
  #define _CCW_HAS_VAR_TEMPLATE     1
 #else
  #define _CCW_HAS_VAR_TEMPLATE     0
 #endif
#endif

#ifndef _CCW_HAS_INIT_LIST
 #if defined(__cpp_initializer_lists) || (__cplusplus >= 201103L)
  #define _CCW_HAS_INIT_LIST        1
 #else
  #define _CCW_HAS_INIT_LIST        0
 #endif
#endif

#ifndef _CCW_HAS_ENUM_CLASS
 #if defined(__cpp_scoped_enums) || (__cplusplus >= 201103L)
  #define _CCW_HAS_ENUM_CLASS       1
 #else
  #define _CCW_HAS_ENUM_CLASS       0
 #endif
#endif

#ifndef _ccw_constexpr11
 #if _ccw_cplusplus >= 201103L
  #define _ccw_constexpr11          constexpr
 #else
  #define _ccw_constexpr11
 #endif
#endif

#ifndef _ccw_constexpr17
 #if __cplusplus >= 201703L
  #define _ccw_constexpr17          constexpr
 #else
  #define _ccw_constexpr17
 #endif
#endif
#ifndef _ccw_constexpr14
 #if __cplusplus >= 201402L
  #define _ccw_constexpr14          constexpr
 #else
  #define _ccw_constexpr14
 #endif
#endif
#ifndef _ccw_constexpr_or_const
 #if __cplusplus >= 201103L
  #define _ccw_constexpr_or_const   constexpr
 #else
  #define _ccw_constexpr_or_const   const
 #endif
#endif
#ifndef _ccw_inline_const
 #if defined(__cpp_inline_variables) && __cpp_inline_variables >= 201606L
  #define _ccw_inline_const         inline const
 #elif defined(_WIN32) || defined(__CYGWIN__)
  #define _ccw_inline_const         extern __attribute__((selectany)) const
 #else
  #define _ccw_inline_const         extern __attribute__((weak)) const
 #endif
#endif
#ifndef _ccw_move_or_swap
 #if __cplusplus >= 201103L
  #define _ccw_move_or_swap(l,r)    ((l) = std::move(r))
 #else
  #define _ccw_move_or_swap(l,r)    swap((l), (r))
 #endif
#endif
#ifndef _ccw_move_or_copy
 #if __cplusplus >= 201103L
  #define _ccw_move_or_copy(l,r)    ((l) = std::move(r))
 #else
  #define _ccw_move_or_copy(l,r)    ((l) = (r))
 #endif
#endif
#ifndef _ccw_static_assert
 #define _ccw_static_assert(c)      static_assert((c), "`"#c "' is failed.")
#endif

namespace __ccw { namespace detail {} }

#endif  // __cplusplus

// ---------------------------------------------------------------------------
// cpu / width macros (kept minimal; expand if a backport needs more)

#if defined(__x86_64__) || defined(__amd64__)
 #define _CCW_CPU_X64
 #define _CCW_CPU_BIT           64
  #define _CCW_MEM_ACCESS_SAFE_REINTERPRET    1
#elif defined(__i386__)
 #define _CCW_CPU_X86
 #define _CCW_CPU_BIT           32
  #define _CCW_MEM_ACCESS_SAFE_REINTERPRET    1
#elif defined(__aarch64__)
 #define _CCW_CPU_ARM64
 #define _CCW_CPU_BIT           64
  #define _CCW_MEM_ACCESS_SAFE_REINTERPRET    1
#elif defined(__arm__)
 #define _CCW_CPU_ARM
 #define _CCW_CPU_BIT           32
#endif
#ifndef _CCW_CPU_BIT
 #if defined(__LP64__) || defined(_WIN64)
  #define _CCW_CPU_BIT          64
 #else
  #define _CCW_CPU_BIT          32
 #endif
#endif

#define _CCW_INT_BIT            32
#define _CCW_LLONG_BIT          64
#if _CCW_CPU_BIT == 64
 #define _CCW_PTR_BIT           64
#else
 #define _CCW_PTR_BIT           32
#endif

#ifndef _CCW_LONG_C
 #define _CCW_LONG_C(val)       val##L
 #define _CCW_ULONG_C(val)      val##UL
#endif
#ifndef _CCW_LLONG_C
 #define _CCW_LLONG_C(val)      val##LL
 #define _CCW_ULLONG_C(val)     val##ULL
#endif

#define _CCW_PFMT_LL            "ll"

#if defined(__cplusplus)
 #if defined(__EXCEPTIONS) || defined(_CPPUNWIND)
  #define _CCW_HAS_EXCEPTIONS 1
 #else
  #define _CCW_HAS_EXCEPTIONS 0
 #endif
 #if defined(__GXX_RTTI) || defined(_CPPRTTI)
  #define _CCW_HAS_RTTI 1
 #else
  #define _CCW_HAS_RTTI 0
 #endif
#endif

#if defined(__cplusplus) && __cplusplus < 201103L && !defined(_CCW_NO_MOVE03)
#include <../../detail/cxx/move03.hpp>
namespace std {
    using ::_ccw::move;
    using ::_ccw::move_if_noexcept;
}
#endif

#endif  // _CCW_CCWRAP_HEADER_H
