#pragma once

#ifndef USE_TEST
#define USE_TEST
#endif

#include <cstddef>
#if 0
 #if defined(_MSC_VER)
  #include "../../vc/std/ccwrap_common.h"
 #elif defined(__GNUC__) || defined(__clang__)
  #include "../../gcc/std/ccwrap_common.h"
 #elif defined(__WATCOMC__)
  #include "../../watcom/std/ccwrap_common.h"
 #endif
#endif

#ifndef _tst_cplusplus
 #if defined(_ccw_cplusplus)
  #define _tst_cplusplus    _ccw_cplusplus
 #elif defined(__WATCOMC__)
  #define _tst_cplusplus    199711L
 #elif defined(_MSVC_LANG)
  #define _tst_cplusplus    _MSVC_LANG
 #else
  #define _tst_cplusplus    __cplusplus
 #endif
#endif

#ifndef TEST_TARGET_CXX
  #if defined(_CCW_TARGET_CXX)
    #define TEST_TARGET_CXX     _CCW_TARGET_CXX
  #else
    #if   _tst_cplusplus >  202302L
      #define TEST_TARGET_CXX   2026
    #elif _tst_cplusplus >= 202302L
      #define TEST_TARGET_CXX   2023
    #elif _tst_cplusplus >= 202002L
      #define TEST_TARGET_CXX   2020
    #elif _tst_cplusplus >= 201703L
      #define TEST_TARGET_CXX   2017
    #elif _tst_cplusplus >= 201402L
      #define TEST_TARGET_CXX   2014
    #elif _tst_cplusplus >= 201103L
      #define TEST_TARGET_CXX   2011
    #else
      #define TEST_TARGET_CXX   2003
    #endif
  #endif
#endif

#if _tst_cplusplus >= 201103L || _MSC_VER >= 1600
#  define _TST_HAS_NULLPTR   1
#  define _TST_HAS_DECLTYPE  1
#else
#  define _TST_HAS_NULLPTR   0
#  define _TST_HAS_DECLTYPE  0
#endif

#if TEST_TARGET_CXX >= 2017 && (_tst_cplusplus >= 201103L || _CCW_TARGET_CXX >= 2017)
#  define _TST_HAS_BYTE 1
#else
#  define _TST_HAS_BYTE 0
#endif

#if _tst_cplusplus >= 201703L || (defined(_CCW_TARGET_CXX) && !defined(_CCW_NO_VA_ARGS) && defined(static_assert))
#  define _TST_HAS_STATIC_ASSERT1 1
#else
#  define _TST_HAS_STATIC_ASSERT1 0
#endif

#if _tst_cplusplus >= 201103L || _MSC_VER >= 1800
#  define _TST_HAS_ALIAS_TEMPLATE 1
#else
#  define _TST_HAS_ALIAS_TEMPLATE 0
#endif

#if _tst_cplusplus >= 201103L || _MSC_VER >= 1600
#  define _TST_HAS_RVALUE_REF 1
#else
#  define _TST_HAS_RVALUE_REF 0
#endif

#if defined(_MSC_VER) && !defined(_MSVC_LANG)
 #define _TST_HAS_CXX11_LIB_MEMBERS (_MSC_VER >= 1700)
 #define _TST_HAS_CXX14_LIB_MEMBERS (_MSC_VER >= 1900)
 #define _TST_HAS_CXX17_LIB_MEMBERS 0
 #define _TST_HAS_CXX20_LIB_MEMBERS 0
 #define _TST_HAS_CXX23_LIB_MEMBERS 0
 #define _TST_HAS_CXX26_LIB_MEMBERS 0
#elif defined(__WATCOMC__)
 #define _TST_HAS_CXX11_LIB_MEMBERS (TEST_TARGET_CXX >= 2011)
 #define _TST_HAS_CXX14_LIB_MEMBERS (TEST_TARGET_CXX >= 2014)
 #define _TST_HAS_CXX17_LIB_MEMBERS (TEST_TARGET_CXX >= 2017)
 #define _TST_HAS_CXX20_LIB_MEMBERS (TEST_TARGET_CXX >= 2020)
 #define _TST_HAS_CXX23_LIB_MEMBERS (TEST_TARGET_CXX >= 2023)
 #define _TST_HAS_CXX26_LIB_MEMBERS (TEST_TARGET_CXX >= 2026)
#else
 #define _TST_HAS_CXX11_LIB_MEMBERS (_tst_cplusplus >= 201103L)
 #define _TST_HAS_CXX14_LIB_MEMBERS (_tst_cplusplus >= 201402L)
 #define _TST_HAS_CXX17_LIB_MEMBERS (_tst_cplusplus >= 201703L)
 #define _TST_HAS_CXX20_LIB_MEMBERS (_tst_cplusplus >= 202002L)
 #define _TST_HAS_CXX23_LIB_MEMBERS (_tst_cplusplus >= 202303L)
 #define _TST_HAS_CXX26_LIB_MEMBERS (_tst_cplusplus >= 202600L)
#endif

#if defined(_MSC_VER) && _MSC_VER < 1910
#  define _TST_HAS_CONTAINER_N_ALLOC 0
#else
#  define _TST_HAS_CONTAINER_N_ALLOC _TST_HAS_CXX14_LIB_MEMBERS
#endif

#if defined(_MSC_VER) && _MSC_VER < 1700
#  define _TST_ATOMIC_LIB_CXX17     1
#  define _TST_ATOMIC_LIB_CXX20     1
#else
#  define _TST_ATOMIC_LIB_CXX17     _TST_HAS_CXX17_LIB_MEMBERS
#  define _TST_ATOMIC_LIB_CXX20     _TST_HAS_CXX20_LIB_MEMBERS
#endif

#if TEST_TARGET_CXX < 2014 || (defined(_MSC_VER) && _MSC_VER < 1800)
#  define _TST_HAS_TRANSPARENT_CMP  0
#else
#  define _TST_HAS_TRANSPARENT_CMP  1
#endif

#if _tst_cplusplus >= 201103L || _MSC_VER >= 1800
#  define _TST_HAS_FINAL 1
#else
#  define _TST_HAS_FINAL 0
#endif

#if defined _MSC_VER
 #if _MSC_VER >= 1914 && _MSVC_LANG >= 201703L
  #define _TST_HAS_CTAD 1
 #else
  #define _TST_HAS_CTAD 0
 #endif
#elif _tst_cplusplus >= 201703L
 #define _TST_HAS_CTAD  1
#else
 #define _TST_HAS_CTAD  0
#endif


#define _TST_HAS_VAR_TEMPLATE   (_tst_cplusplus >= 201402L && TEST_TARGET_CXX >= 2017)

#define _TST_HAS_UDL            (_tst_cplusplus >= 201103L && TEST_TARGET_CXX >= 2014)

#if _tst_cplusplus >= 201103L || _MSC_VER >= 1600 || (defined(__WATCOMC__) && TEST_TARGET_CXX >= 2011)
 #define _TST_HAS_EXCEPTION_PTR 1
#else
 #define _TST_HAS_EXCEPTION_PTR 0
#endif

#if _tst_cplusplus >= 201103L || _MSC_VER >= 1700
 #define _TST_HAS_FUTURE 1
#else
 #define _TST_HAS_FUTURE 0
#endif

#if _tst_cplusplus >= 201103L || _MSC_VER >= 1800
 #define _TST_HAS_INIT_LIST 1
#else
 #define _TST_HAS_INIT_LIST 0
#endif

#if _tst_cplusplus >= 201103L || _MSC_VER >= 1900 || (defined(__WATCOMC__) && TEST_TARGET_CXX >= 2011)
 #define _TST_HAS_CHAR16_32  1
#else
 #define _TST_HAS_CHAR16_32  0
#endif

#if _tst_cplusplus >= 201103L || _MSC_VER >= 1700
#  define _TST_HAS_SCOPED_ENUM 1
#else
#  define _TST_HAS_SCOPED_ENUM 0
#endif

#if TEST_TARGET_CXX < 2023
#  define _TST_HAS_STD_C23_LIB 0
#elif (defined(__GNUC__) || defined(__clang__)) && !defined(_MSC_VER)
#  define _TST_HAS_STD_C23_LIB 0
#else
#  define _TST_HAS_STD_C23_LIB 1
#endif

#if defined(__has_include)
#  define _TST_TEST_HAS(HDR)    __has_include(HDR)
#else
#  define _TST_TEST_HAS(HDR)    1
#endif

#if TEST_TARGET_CXX >= 2023 && _tst_cplusplus >= 202302L \
    && _TST_TEST_HAS(<print>) && !defined(__WATCOMC__) \
    && !(defined(_MSC_VER) && !defined(__has_include))
#  define _TST_HAS_PRINT    1
#else
#  define _TST_HAS_PRINT    0
#endif

#if TEST_TARGET_CXX >= 2020 && defined(__cpp_char8_t)
#  define _TST_HAS_CHAR8_T  1
#else
#  define _TST_HAS_CHAR8_T  0
#endif
#if TEST_TARGET_CXX >= 2020 && (defined(__cpp_char8_t) || defined(__CCW_HAS_CHAR8_T))
#  define _TST_HAS_CHAR8_NAME   1
#else
#  define _TST_HAS_CHAR8_NAME   0
#endif

#if defined(_CPPRTTI) || defined(__GXX_RTTI) || defined(__cpp_rtti)
#  define _TST_HAS_RTTI     1
#else
#  define _TST_HAS_RTTI     0
#endif

#if defined(__WATCOMC__) && (defined(__DOS__) || defined(MSDOS))
#  define _TST_HAS_OS_THREADS 0
#else
#  define _TST_HAS_OS_THREADS 1
#endif

#if defined(__cpp_impl_three_way_comparison) && (__cpp_impl_three_way_comparison) >= 201907L
#  define _TST_HAS_THREE_WAY    1
#else
#  define _TST_HAS_THREE_WAY    0
#endif


#if defined(_LIBCPP_VERSION) && !defined(_CCW_TARGET_CXX)
#  define _TST_LIB_HONOURS_REMOVAL 1
#else
#  define _TST_LIB_HONOURS_REMOVAL 0
#endif
#if !_TST_LIB_HONOURS_REMOVAL || _tst_cplusplus <= 201402L
#  define _TST_HAS_REMOVED_CXX17 1
#else
#  define _TST_HAS_REMOVED_CXX17 0
#endif
#if !_TST_LIB_HONOURS_REMOVAL || _tst_cplusplus <= 202302L
#  define _TST_HAS_REMOVED_CXX26 1
#else
#  define _TST_HAS_REMOVED_CXX26 0
#endif

#if TEST_TARGET_CXX >= 2017 && (_CCW_TARGET_CXX >= 2017 || defined(__cpp_lib_execution))
#  define _TST_HAS_EXECUTION_POLICY 1
#else
#  define _TST_HAS_EXECUTION_POLICY 0
#endif

#if TEST_TARGET_CXX >= 2020 && (_CCW_TARGET_CXX >= 2020 || defined(__cpp_lib_atomic_shared_ptr))
#  define _TST_HAS_ATOMIC_SHARED_PTR 1
#else
#  define _TST_HAS_ATOMIC_SHARED_PTR 0
#endif

#if TEST_TARGET_CXX >= 2020 && (_CCW_TARGET_CXX >= 2020 || (defined(__cpp_lib_chrono) && (__cpp_lib_chrono) >= 201907L))
#  define _TST_HAS_CHRONO_CXX20 1
#else
#  define _TST_HAS_CHRONO_CXX20 0
#endif

#if TEST_TARGET_CXX >= 2020 && defined(__cpp_lib_chrono) && (__cpp_lib_chrono) >= 201907L
#  define _TST_HAS_CHRONO_TZDB 1
#else
#  define _TST_HAS_CHRONO_TZDB 0
#endif

#if TEST_TARGET_CXX >= 2026 && \
    ((_CCW_TARGET_CXX >= 2026 && (defined(__WATCOMC__) || (defined(_MSC_VER) && _MSC_VER < 1600))) || \
     (defined(__cpp_lib_chrono) && (__cpp_lib_chrono) >= 202306L))
#  define _TST_HAS_CHRONO_HASH 1
#else
#  define _TST_HAS_CHRONO_HASH 0
#endif

#if TEST_TARGET_CXX >= 2020 && (_CCW_TARGET_CXX >= 2020 || defined(__cpp_lib_syncbuf))
#  define _TST_HAS_SYNCSTREAM 1
#else
#  define _TST_HAS_SYNCSTREAM 0
#endif

#if TEST_TARGET_CXX >= 2017 && (_CCW_TARGET_CXX >= 2017 || defined(__cpp_lib_math_special_functions))
#  define _TST_HAS_SPECIAL_MATH 1
#else
#  define _TST_HAS_SPECIAL_MATH 0
#endif

#if TEST_TARGET_CXX >= 2011 && !(defined(_LIBCPP_VERSION) && !defined(_CCW_TARGET_CXX))
#  define _TST_HAS_MUTEX_NATIVE_HANDLE 1
#else
#  define _TST_HAS_MUTEX_NATIVE_HANDLE 0
#endif

#if TEST_TARGET_CXX >= 2017 && !(defined(_LIBCPP_VERSION) && !defined(_CCW_TARGET_CXX))
#  define _TST_HAS_FROM_CHARS_LONG_DOUBLE 1
#else
#  define _TST_HAS_FROM_CHARS_LONG_DOUBLE 0
#endif

#if TEST_TARGET_CXX >= 2023 && defined(_CCW_STD_RELOCATED)
#  define _TST_HAS_MDSPAN_CALL_OP 1
#else
#  define _TST_HAS_MDSPAN_CALL_OP 0
#endif

#ifndef STD
#  ifdef _CCW_STD
#    define STD _CCW_STD
#  else
#    define STD std
#  endif
#endif

#ifndef STD_NS
#  define STD_NS STD::
#endif

#if defined(TST_GLOBAL_C_NAMES) && defined(_MSC_VER)
#  define _TST_NO_GLOBAL_HYPOT3 1
#else
#  define _TST_NO_GLOBAL_HYPOT3 0
#endif

#if defined(TST_GLOBAL_C_NAMES) && defined(_MSC_VER) && _MSC_VER < 1700
#  define _TST_NO_GLOBAL_MATH_INT 1
#else
#  define _TST_NO_GLOBAL_MATH_INT 0
#endif

#if defined(__WATCOMC__) || (defined(__GNUC__) && !defined(_MSC_VER) && __cplusplus < 201103L)
#  define _TST_IS_CONVERTIBLE_CLASS 0
#else
#  define _TST_IS_CONVERTIBLE_CLASS 1
#endif

#include "../src/test.hpp"


namespace _tst_ty {
    template <class _A, class _B> struct same_ { enum { value = 0 }; };
    template <class _A>           struct same_<_A, _A> { enum { value = 1 }; };
}
template <class _Want, class _Got>
bool tst_type_is(const _Got&) { return _tst_ty::same_<_Want, _Got>::value != 0; }


#if defined(_LIBCPP_VERSION) && !defined(_CCW_TARGET_CXX)
#  define TEST_SKIP_LIBCXX(...)  do { TEST_NOTE("" __VA_ARGS__); TEST_SKIP1(); } while (0)
#else
#  define TEST_SKIP_LIBCXX(...)  ((void)sizeof("" __VA_ARGS__))
#endif
