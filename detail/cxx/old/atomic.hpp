/**
 *  @file   detail/old/atomic.hpp
 *  @brief  <atomic> backport for pre-C++11 VC over the Interlocked intrinsics.
 *  @license Boost Software License Version 1.0
 *  @note
 *      Included by the forwarder (vc/atomic) only when native <atomic> is absent,
 *      so it lives directly in namespace std. A practical Interlocked-based backport
 *      (32-bit integer atomics -- the common cases; 64-bit / pointer / non-integral
 *      RMW are out of the light backport).
 */
#ifndef _CCW_DETAIL_ATOMIC_HPP
#define _CCW_DETAIL_ATOMIC_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <intrin.h>
#include <cstddef>

namespace std {

typedef enum memory_order {
    memory_order_relaxed, memory_order_consume, memory_order_acquire,
    memory_order_release, memory_order_acq_rel, memory_order_seq_cst
} memory_order;

template<class T>
class atomic {
    T volatile v_;
    atomic(const atomic&);
    atomic& operator=(const atomic&);
public:
    atomic() : v_() {}
    atomic(T d) : v_(d) {}

    bool is_lock_free() const { return sizeof(T) <= 8; }
    void store(T d, memory_order = memory_order_seq_cst)       { v_ = d; }
    T    load(memory_order = memory_order_seq_cst) const       { return v_; }
    operator T() const { return v_; }
    T    operator=(T d) { v_ = d; return d; }

    T exchange(T d, memory_order = memory_order_seq_cst) { return (T)_InterlockedExchange((long volatile*)&v_, (long)d); }
    bool compare_exchange_strong(T& expected, T desired, memory_order = memory_order_seq_cst) {
        long old = _InterlockedCompareExchange((long volatile*)&v_, (long)desired, (long)expected);
        if (old == (long)expected) return true;
        expected = (T)old;
        return false;
    }
    bool compare_exchange_weak(T& expected, T desired, memory_order mo = memory_order_seq_cst) {
        return compare_exchange_strong(expected, desired, mo);
    }
    T fetch_add(T d, memory_order = memory_order_seq_cst) { return (T)_InterlockedExchangeAdd((long volatile*)&v_, (long)d); }
    T fetch_sub(T d, memory_order = memory_order_seq_cst) { return (T)_InterlockedExchangeAdd((long volatile*)&v_, -(long)d); }
    T fetch_or (T d, memory_order = memory_order_seq_cst) { return (T)_InterlockedOr ((long volatile*)&v_, (long)d); }
    T fetch_and(T d, memory_order = memory_order_seq_cst) { return (T)_InterlockedAnd((long volatile*)&v_, (long)d); }
    T fetch_xor(T d, memory_order = memory_order_seq_cst) { return (T)_InterlockedXor((long volatile*)&v_, (long)d); }

    T operator++()    { return (T)(fetch_add(T(1)) + T(1)); }
    T operator++(int) { return fetch_add(T(1)); }
    T operator--()    { return (T)(fetch_sub(T(1)) - T(1)); }
    T operator--(int) { return fetch_sub(T(1)); }
    T operator+=(T d) { return (T)(fetch_add(d) + d); }
    T operator-=(T d) { return (T)(fetch_sub(d) - d); }
    T operator|=(T d) { return (T)(fetch_or(d) | d); }
    T operator&=(T d) { return (T)(fetch_and(d) & d); }
    T operator^=(T d) { return (T)(fetch_xor(d) ^ d); }
};

class atomic_flag {
    long volatile v_;
    atomic_flag(const atomic_flag&);
    atomic_flag& operator=(const atomic_flag&);
public:
    atomic_flag() : v_(0) {}
    bool test_and_set(memory_order = memory_order_seq_cst) { return _InterlockedExchange(&v_, 1) != 0; }
    void clear(memory_order = memory_order_seq_cst)        { _InterlockedExchange(&v_, 0); }
};
#define ATOMIC_FLAG_INIT {}

inline void atomic_thread_fence(memory_order) { _ReadWriteBarrier(); }
inline void atomic_signal_fence(memory_order) { _ReadWriteBarrier(); }

typedef atomic<bool>               atomic_bool;
typedef atomic<char>               atomic_char;
typedef atomic<signed char>        atomic_schar;
typedef atomic<unsigned char>      atomic_uchar;
typedef atomic<short>              atomic_short;
typedef atomic<unsigned short>     atomic_ushort;
typedef atomic<int>                atomic_int;
typedef atomic<unsigned int>       atomic_uint;
typedef atomic<long>               atomic_long;
typedef atomic<unsigned long>      atomic_ulong;
typedef atomic<_ccw_llong>         atomic_llong;
typedef atomic<_ccw_ullong>        atomic_ullong;
typedef atomic<std::size_t>        atomic_size_t;
typedef atomic<std::ptrdiff_t>     atomic_ptrdiff_t;

}   // namespace std

#endif  // _CCW_DETAIL_ATOMIC_HPP
