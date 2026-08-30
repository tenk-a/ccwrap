/**
 *  @file   detail/version_macros.h
 *  @brief  The __cpp_lib_* feature-test macros ccwrap is entitled to claim.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_VERSION_MACROS_H
#define _CCW_DETAIL_VERSION_MACROS_H
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

#if defined(__WATCOMC__) || (defined(_MSC_VER) && _MSC_VER < 1600)
#ifndef _CCW_LIBCXX03
#define _CCW_LIBCXX03 1
#endif
#endif

#if defined(__WATCOMC__) || (defined(_MSC_VER) && _MSC_VER < 1600)
#ifndef _CCW_LIBCXX03
#define _CCW_LIBCXX03 1
#endif
#endif

#if _CCW_TARGET_CXX >= 2014
#if !defined(_MSC_VER) || _MSC_VER >= 1600
#ifndef __cpp_lib_make_unique
#  define __cpp_lib_make_unique 201304L
#endif
#endif
#ifndef __cpp_lib_shared_timed_mutex
#  define __cpp_lib_shared_timed_mutex 201402L
#endif
#endif
#if _CCW_TARGET_CXX >= 2017
#if defined(_CCW_LIBCXX03)
#ifndef __cpp_lib_filesystem
#  define __cpp_lib_filesystem 201703L
#endif
#endif
#ifndef __cpp_lib_as_const
#  define __cpp_lib_as_const 201510L
#endif
#ifndef __cpp_lib_clamp
#  define __cpp_lib_clamp 201603L
#endif
#ifndef __cpp_lib_gcd_lcm
#  define __cpp_lib_gcd_lcm 201606L
#endif
#ifndef __cpp_lib_invoke
#  define __cpp_lib_invoke 201411L
#endif
#ifndef __cpp_lib_launder
#  define __cpp_lib_launder 201606L
#endif
#ifndef __cpp_lib_hardware_interference_size
#  define __cpp_lib_hardware_interference_size 201703L
#endif
#ifndef __cpp_lib_nonmember_container_access
#  define __cpp_lib_nonmember_container_access 201411L
#endif
#ifndef __cpp_lib_chrono
#  define __cpp_lib_chrono 201611L
#endif
#ifndef __cpp_lib_shared_mutex
#  define __cpp_lib_shared_mutex 201505L
#endif
#ifndef __cpp_lib_scoped_lock
#  define __cpp_lib_scoped_lock 201703L
#endif
#endif
#if _CCW_TARGET_CXX >= 2020
#ifndef __cpp_lib_bit_cast
#  define __cpp_lib_bit_cast 201806L
#endif
#ifndef __cpp_lib_bitops
#  define __cpp_lib_bitops 201907L
#endif
#ifndef __cpp_lib_int_pow2
#  define __cpp_lib_int_pow2 202002L
#endif
#ifndef __cpp_lib_endian
#  define __cpp_lib_endian 201907L
#endif
#ifndef __cpp_lib_ssize
#  define __cpp_lib_ssize 201902L
#endif
#ifndef __cpp_lib_math_constants
#  define __cpp_lib_math_constants 201907L
#endif
#ifndef __cpp_lib_latch
#  define __cpp_lib_latch 201907L
#endif
#ifndef __cpp_lib_barrier
#  define __cpp_lib_barrier 201907L
#endif
#ifndef __cpp_lib_semaphore
#  define __cpp_lib_semaphore 201907L
#endif
#ifndef __cpp_lib_remove_cvref
#  define __cpp_lib_remove_cvref 201711L
#endif
#ifndef __cpp_lib_atomic_lock_free_type_aliases
#  define __cpp_lib_atomic_lock_free_type_aliases 201907L
#endif
#ifndef __cpp_lib_atomic_flag_test
#  define __cpp_lib_atomic_flag_test 201907L
#endif
#endif
#if _CCW_TARGET_CXX >= 2023
#ifndef __cpp_lib_byteswap
#  define __cpp_lib_byteswap 202110L
#endif
#ifndef __cpp_lib_stdatomic_h
#  define __cpp_lib_stdatomic_h 202011L
#endif
#ifndef __cpp_lib_string_resize_and_overwrite
#  define __cpp_lib_string_resize_and_overwrite 202110L
#endif
#endif

#endif  // _CCW_DETAIL_VERSION_MACROS_H
