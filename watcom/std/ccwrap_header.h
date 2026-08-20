/**
 *  @file ccwrap_header.h
 *  @brief  for Open Watcom C/C++ (v1.9 / v2.0). Force-included config.
 *  @author Masashi Kitamura(tenka@6809.net)
 *  @license Boost Software License Version 1.0
 *  @note
 *    __WATCOMC__ : 1290 = OW 1.9,  1300 = OW 2.0(beta)
 */
#ifndef _CCW_CCWRAP_HEADER_H
#define _CCW_CCWRAP_HEADER_H
#pragma once

// ---------------------------------------------------------------------------
#if !defined(__WATCOMC__)
#error for Open Watcom C/C++ header
#endif

#define _CCW_COMPILER                   "Open Watcom C/C++ (" _CCW_M_STR(__WATCOMC__) ")"

// ---------------------------------------------------------------------------
#if defined(__NT__) || defined(_WIN32) || defined(__WINDOWS__)
 #define _CCW_OS_WIN                    1
#elif defined(__DOS__) || defined(MSDOS)
 #define _CCW_OS_DOS                    1
#endif

#define _CCW_CONFIG_DIR                 ../watcom
#define _CCW_CONFIG_PATH(x)             <_CCW_CONFIG_DIR/x>

#ifndef _CCW_NATIVE_C_HEADER_DIR
 #define _CCW_NATIVE_C_HEADER_DIR       ../h
#endif
#ifndef _CCW_NATIVE_C_HEADER_PATH
 #define _CCW_NATIVE_C_HEADER_PATH(x)   <_CCW_NATIVE_C_HEADER_DIR/x>
#endif
#ifndef _CCW_NATIVE_STL_HEADER_DIR
 #define _CCW_NATIVE_STL_HEADER_DIR     ../../detail/llibcxx03/include
#endif
#ifndef _CCW_NATIVE_STL_HEADER_PATH
 #define _CCW_NATIVE_STL_HEADER_PATH(x) <_CCW_NATIVE_STL_HEADER_DIR/x>
#endif

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
// c & c++

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
 #define _CCW_NO_VA_ARGS        0
#endif

#define _CCW_HAS_PRAGMA_ONCE

#if defined(__CHAR_SIGNED__)
 #define _CCW_IS_CHAR_UNSIGNED  0
#else
 #define _CCW_IS_CHAR_UNSIGNED  1
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

#define _CCW_MESSAGE(x)

#define _ccw_forceinline        __inline
#define _ccw_noinline
#define _ccw_weak
#define _ccw_dllimport          __declspec(dllimport)
#define _ccw_dllexport          __declspec(dllexport)
#define _ccw_fastcall
#define _ccw_cdecl              __cdecl
#define _ccw_stdcall            __stdcall
#define _ccw_native_decl        __watcall


// ---------------------------------------------------------------------------
// c++  (minimal; the C++ standard library is a separate from_stdcxx/ effort)

#if defined(__cplusplus)

 #define _ccw_cplusplus         199711L //__cplusplus

 #if !defined(nullptr)
  #define nullptr                   (0)
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
   #if !defined(_CPPUNWIND) || defined(__SW_XD) || defined(__SW_XDT) || defined(__SW_XDS)
    #define noexcept
   #else
    #define noexcept            throw()
   #endif
 #endif
 #if !defined(__func__)
  #define __func__                  __FUNCTION__
 #endif
 #if !defined(_Pragma)
  #define _Pragma(...)          //__pragma(__VA_ARGS__)
 #endif

 #ifndef _CCW_NO_VARIADIC_TEMPLATES
  #define _CCW_NO_VARIADIC_TEMPLATES 1
 #endif
 #ifndef _CCW_HAS_INIT_LIST
  #define _CCW_HAS_INIT_LIST 0
 #endif
 #ifndef _CCW_HAS_VAR_TEMPLATE
  #define _CCW_HAS_VAR_TEMPLATE 0
 #endif
 #ifndef _CCW_HAS_ALIAS_TEMPLATE
  #define _CCW_HAS_ALIAS_TEMPLATE 0
 #endif
 #ifndef _CCW_HAS_UDL
  #define _CCW_HAS_UDL 0
 #endif
 #ifndef _CCW_HAS_ENUM_CLASS
  #define _CCW_HAS_ENUM_CLASS 0
 #endif

 #define _ccw_noreturn
 #define _ccw_deprecated
 #define _ccw_carries_dependency
 #define _ccw_fallthrough
 #define _ccw_maybe_unused
 #define _ccw_nodiscard
 #ifndef _ccw_constexpr11
  #define _ccw_constexpr11
 #endif
 #ifndef _ccw_constexpr17
  #define _ccw_constexpr17
 #endif
 #ifndef _ccw_constexpr14
  #define _ccw_constexpr14
 #endif
 #ifndef _ccw_constexpr_or_const
  #define _ccw_constexpr_or_const   const
 #endif
 #ifndef _ccw_inline_const
  #define _ccw_inline_const         static const
 #endif

  #ifndef _CPPRTTI
    #define _CCW_NO_RTTI        1
  #endif
  #define _CCW_NO_STATIC_CONST_INT_MEMBER_INITIALIZATION 1
  #if !defined(static_assert)
        namespace __ccw { template<int x> struct static_assert_check{}; }
        template <bool x> struct __static_assert_FAILED_;
        template <> struct __static_assert_FAILED_<true> { enum { value = 1 }; };
        #define static_assert(c, ...)   typedef __ccw::static_assert_check<sizeof(__static_assert_FAILED_<(c) != 0>)> _CCW_M_CAT(__ccw_static_assert_L_, __LINE__)
  #endif
  #if !defined(alignof)
    namespace __ccw {
        template<class T> class __alignof {
            struct U { char a; T b; };
            enum { nt = sizeof(T), nu = sizeof(U), d = nu - nt };
        public:
            enum { value = (nt < d) ? nt : d };
        };
    }
    #define alignof(a)          (__ccw::__alignof<a>::value)
  #endif
  //#if !defined(alignas)
  //  #define alignas(a)         //__declspec(align(a))
  //#endif
  #define _CCW_16_32_DEFINED
  typedef unsigned short        char16_t;
  typedef unsigned int          char32_t;
  #if _CCW_TARGET_CXX >= 2020
   #ifndef __CCW_HAS_CHAR8_T
    #define __CCW_HAS_CHAR8_T   1
    typedef unsigned char       char8_t;
   #endif
  #endif
  //#if !defined(thread_local)
  //  #define thread_local      //__declspec(thread)
  //#endif
  typedef bool                 _ccw_bool;

#else   // ---- C mode ----------------------------------------------------------

  #if __STDC_VERSION__ < 199901L
    #define inline                  __inline
  #endif

  #if !defined(_Alignof)
    #define _Alignof(T)             ((size_t)((ptrdiff_t)(&((struct {char a_; T t_;}*)(0))->t_)))
  #endif
  #ifndef _Alignas
  // Open Watcom has no per-object alignment specifier; best-effort no-op.
    #define _Alignas(a)
  #endif
  #ifndef _Bool
    #define _Bool                   _ccw_bool
  #endif
  #ifndef _Noreturn
    #define _Noreturn
  #endif
  //#ifndef _Thread_local
  //  #define _Thread_local
  //#endif
  #ifndef _Static_assert
    #define _Static_assert(c, ...)    typedef char _CCW_M_CAT(__static_assert_failed_L,__LINE__)[(c) ? 1/*OK*/ : -1/*NG*/]
  #endif
  typedef unsigned char         _ccw_bool;

#endif

// ---------------------------------------------------------------------------
// cpu / width macros

#if defined(__386__)
 #define _CCW_CPU_X86
 #define _CCW_CPU_BIT           32
 #define _CCW_INT_BIT           32
 #define _CCW_PTR_BIT           32
#elif defined(__I86__)
 #define _CCW_CPU_X86_16
 #define _CCW_CPU_BIT           16
 #define _CCW_INT_BIT           16
 #if defined(__COMPACT__) || defined(__LARGE__) || defined(__HUGE__)
  #define _CCW_PTR_BIT          32
 #else
  #define _CCW_PTR_BIT          16
 #endif
#else
 #define _CCW_CPU_BIT           32
 #define _CCW_INT_BIT           32
 #define _CCW_PTR_BIT           32
#endif
#define _CCW_LLONG_BIT          64

#ifndef _CCW_LONG_C
 #define _CCW_LONG_C(val)       val##L
 #define _CCW_ULONG_C(val)      val##UL
#endif
#ifndef _CCW_LLONG_C
 #define _CCW_LLONG_C(val)      val##i64
 #define _CCW_ULLONG_C(val)     val##ui64
#endif

#define _CCW_PFMT_LL            "ll"

typedef signed __int8           _ccw_int8;
typedef unsigned __int8         _ccw_uint8;
typedef __int16                 _ccw_int16;
typedef unsigned __int16        _ccw_uint16;
typedef __int32                 _ccw_int32;
typedef unsigned __int32        _ccw_uint32;
typedef __int64                 _ccw_int64;
typedef unsigned __int64        _ccw_uint64;
typedef __int64                 _ccw_llong;
typedef unsigned __int64        _ccw_ullong;
typedef unsigned char           _ccw_char8;
typedef _ccw_uint16             _ccw_char16;
typedef _ccw_uint32             _ccw_char32;
#if __cplusplus
typedef wchar_t                 _ccw_wchar;
#else
typedef unsigned __int16        _ccw_wchar;
#endif
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
#define _ccw_char8              _ccw_char8
#define _ccw_char16             _ccw_char16
#define _ccw_char32             _ccw_char32
#define _ccw_wchar              _ccw_wchar
#define _ccw_bool               _ccw_bool

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

#endif  // _CCW_CCWRAP_HEADER_H
