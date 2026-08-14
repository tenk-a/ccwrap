// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: <stdatomic.h> (C++23). In C++ this header pulls in <atomic>
//  and re-exports the atomic names into the global namespace, and defines the
//  _Atomic(T) spelling as _CCW_STD::atomic<T>. Classic C++03 body.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP_STDATOMIC_H
#define _CCW_LIBCPP_STDATOMIC_H
#include "__config"
#include <atomic>

#define _Atomic(_Tp) ::_CCW_STD::atomic<_Tp>

using _CCW_STD::memory_order;
using _CCW_STD::memory_order_relaxed;
using _CCW_STD::memory_order_consume;
using _CCW_STD::memory_order_acquire;
using _CCW_STD::memory_order_release;
using _CCW_STD::memory_order_acq_rel;
using _CCW_STD::memory_order_seq_cst;

using _CCW_STD::atomic;
using _CCW_STD::atomic_flag;

using _CCW_STD::atomic_bool;
using _CCW_STD::atomic_char;
using _CCW_STD::atomic_schar;
using _CCW_STD::atomic_uchar;
using _CCW_STD::atomic_short;
using _CCW_STD::atomic_ushort;
using _CCW_STD::atomic_int;
using _CCW_STD::atomic_uint;
using _CCW_STD::atomic_long;
using _CCW_STD::atomic_ulong;
using _CCW_STD::atomic_llong;
using _CCW_STD::atomic_ullong;
using _CCW_STD::atomic_wchar_t;
using _CCW_STD::atomic_int8_t;
using _CCW_STD::atomic_uint8_t;
using _CCW_STD::atomic_int16_t;
using _CCW_STD::atomic_uint16_t;
using _CCW_STD::atomic_int32_t;
using _CCW_STD::atomic_uint32_t;
using _CCW_STD::atomic_int64_t;
using _CCW_STD::atomic_uint64_t;
using _CCW_STD::atomic_size_t;
using _CCW_STD::atomic_ptrdiff_t;
using _CCW_STD::atomic_intptr_t;
using _CCW_STD::atomic_uintptr_t;
using _CCW_STD::atomic_intmax_t;
using _CCW_STD::atomic_uintmax_t;
#if defined(__cpp_char8_t) || defined(__CCW_HAS_CHAR8_T)
using _CCW_STD::atomic_char8_t;
#endif
using _CCW_STD::atomic_char16_t;
using _CCW_STD::atomic_char32_t;
using _CCW_STD::atomic_int_least8_t;
using _CCW_STD::atomic_uint_least8_t;
using _CCW_STD::atomic_int_least16_t;
using _CCW_STD::atomic_uint_least16_t;
using _CCW_STD::atomic_int_least32_t;
using _CCW_STD::atomic_uint_least32_t;
using _CCW_STD::atomic_int_least64_t;
using _CCW_STD::atomic_uint_least64_t;
using _CCW_STD::atomic_int_fast8_t;
using _CCW_STD::atomic_uint_fast8_t;
using _CCW_STD::atomic_int_fast16_t;
using _CCW_STD::atomic_uint_fast16_t;
using _CCW_STD::atomic_int_fast32_t;
using _CCW_STD::atomic_uint_fast32_t;
using _CCW_STD::atomic_int_fast64_t;
using _CCW_STD::atomic_uint_fast64_t;

using _CCW_STD::atomic_load;
using _CCW_STD::atomic_load_explicit;
using _CCW_STD::atomic_store;
using _CCW_STD::atomic_store_explicit;
using _CCW_STD::atomic_exchange;
using _CCW_STD::atomic_exchange_explicit;
using _CCW_STD::atomic_compare_exchange_strong;
using _CCW_STD::atomic_compare_exchange_strong_explicit;
using _CCW_STD::atomic_compare_exchange_weak;
using _CCW_STD::atomic_compare_exchange_weak_explicit;
using _CCW_STD::atomic_fetch_add;
using _CCW_STD::atomic_fetch_add_explicit;
using _CCW_STD::atomic_fetch_sub;
using _CCW_STD::atomic_fetch_sub_explicit;
using _CCW_STD::atomic_fetch_and;
using _CCW_STD::atomic_fetch_and_explicit;
using _CCW_STD::atomic_fetch_or;
using _CCW_STD::atomic_fetch_or_explicit;
using _CCW_STD::atomic_fetch_xor;
using _CCW_STD::atomic_fetch_xor_explicit;
using _CCW_STD::atomic_is_lock_free;
using _CCW_STD::atomic_init;
using _CCW_STD::atomic_wait;
using _CCW_STD::atomic_wait_explicit;
using _CCW_STD::atomic_notify_one;
using _CCW_STD::atomic_notify_all;
using _CCW_STD::atomic_flag_test_and_set;
using _CCW_STD::atomic_flag_test_and_set_explicit;
using _CCW_STD::atomic_flag_clear;
using _CCW_STD::atomic_flag_clear_explicit;
using _CCW_STD::atomic_flag_test;
using _CCW_STD::atomic_flag_test_explicit;
using _CCW_STD::atomic_flag_wait;
using _CCW_STD::atomic_flag_wait_explicit;
using _CCW_STD::atomic_flag_notify_one;
using _CCW_STD::atomic_flag_notify_all;
using _CCW_STD::atomic_thread_fence;
using _CCW_STD::atomic_signal_fence;

#ifndef __cpp_lib_stdatomic_h
#define __cpp_lib_stdatomic_h 202011L
#endif

#endif // _CCW_LIBCPP_STDATOMIC_H
