/**
 *  @file ccwrap_header.h
 *  @brief  for open watcom c/c++ v1.9
 *  @license Boost Software Lisence Version 1.0
 *  @note
 *      mada tekikou
 */
#pragma once

#if !defined(__WATCOMC__)
#error this header is for open watcom c/c++
#endif

#define __CCWRAP_COMPILER       "WATCOM-C" //"WATCOM C (" __WATCOMC__ ")"


#ifndef __CCWRAP_M_CAT
  #define __CCWRAP_M_CAT(a,b)       __CCWRAP_M_CAT_S2(a,b)
  #define __CCWRAP_M_CAT_S2(a,b)    a##b
#endif

#ifndef __CCWRAP_M_TO_STR
  #define __CCWRAP_M_TO_STR(x)      __CCWRAP_M_TO_STR_2(x)
#endif
#ifndef __CCWRAP_M_TO_STR_2
  #define __CCWRAP_M_TO_STR_2(x)    #x
#endif

#ifndef __CCWRAP_NATIVE_C_HEADER_DIR
   #define __CCWRAP_NATIVE_C_HEADER_DIR     ../h
#endif
#ifndef __CCWRAP_NATIVE_C_HEADER_PATH
  #define __CCWRAP_NATIVE_C_HEADER_PATH(x)   <../h/##x>
#endif

#ifndef __CCWRAP_NATIVE_STL_HEADER_DIR
  #define __CCWRAP_NATIVE_STL_HEADER_DIR    ../h
#endif
#ifndef __CCWRAP_NATIVE_STL_HEADER_PATH
  #define __CCWRAP_NATIVE_STL_HEADER_PATH(x) <../h/##x>
#endif


//#ifndef __CCWRAP_NATIVE_UC_HEADER_PATH
//  #define __CCWRAP_NATIVE_UC_HEADER_PATH(x)   <__CCWRAP_NATIVE_C_HEADER_DIR/##x>
//#endif

#define __CCWRAP_HAS_PRAGMA_ONCE

#define __CCWRAP_NO_ALIGNAS
#define __CCWRAP_NO_THREAD_LOCAL

//#if !defined(__has_include)
//  #define __has_include(x)    0
//#endif
//#if !defined(__has_feature)
//  #define __has_feature(x)    0
//#endif

#if !defined(__func__)
  #define __func__              __FUNCTION__
#endif
#if !defined(_Pragma)
  #define _Pragma(...)          //__pragma(__VA_ARGS__)
#endif

#define __CCWRAP_LONG_BIT           32
#define __CCWRAP_LDOUBLE_BIT        64

#define __CCWRAP_HAS_STDINT         1

#if defined(__cplusplus)
  #define __CCWRAP_NO_STATIC_CONST_INT_MEMBER_INITIALIZATION 1
  #if !defined(override)
    #define override
  #endif
  #if !defined(final)
    #define final
  #endif
  #if !defined(noexcept)
    #define noexcept            throw()
  #endif
  #if !defined(nullptr)
    #define nullptr             (0)
  #endif
  #if !defined(constexpr)
    #define constexpr
  #endif
  #if !defined(static_assert)
        namespace __ccwrap { template<int x> struct static_assert_check{}; }
        template <bool x> struct __static_assert_FAILED_;
        template <> struct __static_assert_FAILED_<true> { enum { value = 1 }; };
        #define static_assert(c, ...)   typedef __ccwrap::static_assert_check<sizeof(__static_assert_FAILED_<(c) != 0>)> __CCWRAP_M_CAT(__ccwrap_static_assert_L_, __LINE__)
  #endif
  #if !defined(alignof)
    namespace __ccwrap_detail {
        template<class T> class __alignof {
            struct U { char a; T b; };
            enum { nt = sizeof(T), nu = sizeof(U), d = nu - nt };
        public:
            enum { value = (nt < d) ? nt : d };
        };
    }
    #define alignof(a)          (__ccwrap_detail::__alignof<a>::value)
  #endif
  //#if !defined(alignas)
  //  #define alignas(a)         //__declspec(align(a))
  //#endif
  typedef unsigned short        char16_t;
  typedef unsigned              char32_t;
  //#if !defined(thread_local)
  //  #define thread_local      //__declspec(thread)
  //#endif
  #define __CCWRAP_NO_WCHAR
#else
  #if !defined(_Alignof)
    #define _Alignof(T)             ((size_t)((ptrdiff_t)(&((struct {char a_; T t_;}*)(0))->t_)))
  #endif
  //#ifndef _Alignas
  //  #define _Alignas(a)
  //#endif
  #ifndef _Bool
    #define _Bool                   char
  #endif
  #ifndef _Noreturn
    #define _Noreturn
  #endif
  //#ifndef _Thread_local
  //  #define _Thread_local
  //#endif
  #ifndef _Static_assert
    #define _Static_assert(c, ...)    typedef char __CCWRAP_M_CAT(__static_assert_failed_L,__LINE__)[(c) ? 1/*OK*/ : -1/*NG*/]
  #endif
#endif  // __cplusplus

#include "../ccwrap/ccwrap_misc/ccwrap_header_sub.hh"
