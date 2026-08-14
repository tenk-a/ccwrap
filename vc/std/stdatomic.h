// ccwrap <stdatomic.h>
#pragma once

#include <ccwrap_common.h>

#if !defined(__cplusplus) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L \
    && !defined(__STDC_NO_ATOMICS__) && defined(_MSC_VER) && _MSC_VER >= 1944

 #include _CCW_NATIVE_UC_HEADER_PATH(stdatomic.h)

#else

#include <intrin.h>
#include <stddef.h>

#if _MSC_VER < 1500
#pragma intrinsic(_InterlockedAnd)
#pragma intrinsic(_InterlockedOr)
#pragma intrinsic(_InterlockedXor)
#pragma intrinsic(_ReadWriteBarrier)
#endif

typedef enum memory_order {
    memory_order_relaxed, memory_order_consume, memory_order_acquire,
    memory_order_release, memory_order_acq_rel, memory_order_seq_cst
} memory_order;

#define _Atomic(T)  T volatile

typedef char volatile               atomic_char;
typedef signed char volatile        atomic_schar;
typedef unsigned char volatile      atomic_uchar;
typedef short volatile              atomic_short;
typedef unsigned short volatile     atomic_ushort;
typedef int volatile                atomic_int;
typedef unsigned int volatile       atomic_uint;
typedef long volatile               atomic_long;
typedef unsigned long volatile      atomic_ulong;
typedef _ccw_llong volatile         atomic_llong;
typedef _ccw_ullong volatile        atomic_ullong;
typedef size_t volatile             atomic_size_t;
typedef ptrdiff_t volatile          atomic_ptrdiff_t;
typedef unsigned char volatile      atomic_bool;

#define ATOMIC_BOOL_LOCK_FREE     2
#define ATOMIC_CHAR_LOCK_FREE     2
#define ATOMIC_SHORT_LOCK_FREE    2
#define ATOMIC_INT_LOCK_FREE      2
#define ATOMIC_LONG_LOCK_FREE     2
#define ATOMIC_LLONG_LOCK_FREE    2
#define ATOMIC_POINTER_LOCK_FREE  2

#define ATOMIC_VAR_INIT(v)        (v)
#define ATOMIC_FLAG_INIT          { 0 }

#define atomic_init(p, v)         ((void)(*(p) = (v)))
#define atomic_is_lock_free(p)    (sizeof(*(p)) <= 8)

#define atomic_load(p)                     (*(p))
#define atomic_load_explicit(p, mo)        (*(p))
#define atomic_store(p, v)                 ((void)(*(p) = (v)))
#define atomic_store_explicit(p, v, mo)    ((void)(*(p) = (v)))

#define atomic_exchange(p, v)              ((int)_InterlockedExchange((long volatile*)(p), (long)(v)))
#define atomic_exchange_explicit(p, v, mo) atomic_exchange(p, v)
#define atomic_fetch_add(p, v)             ((int)_InterlockedExchangeAdd((long volatile*)(p), (long)(v)))
#define atomic_fetch_add_explicit(p, v, mo) atomic_fetch_add(p, v)
#define atomic_fetch_sub(p, v)             ((int)_InterlockedExchangeAdd((long volatile*)(p), -(long)(v)))
#define atomic_fetch_sub_explicit(p, v, mo) atomic_fetch_sub(p, v)
#define atomic_fetch_or(p, v)              ((int)_InterlockedOr((long volatile*)(p), (long)(v)))
#define atomic_fetch_or_explicit(p, v, mo)  atomic_fetch_or(p, v)
#define atomic_fetch_and(p, v)             ((int)_InterlockedAnd((long volatile*)(p), (long)(v)))
#define atomic_fetch_and_explicit(p, v, mo) atomic_fetch_and(p, v)
#define atomic_fetch_xor(p, v)             ((int)_InterlockedXor((long volatile*)(p), (long)(v)))
#define atomic_fetch_xor_explicit(p, v, mo) atomic_fetch_xor(p, v)

static __inline int __ccw_atomic_cas(long volatile* p, long* expected, long desired) {
    long old = _InterlockedCompareExchange(p, desired, *expected);
    if (old == *expected) return 1;
    *expected = old;
    return 0;
}
#define atomic_compare_exchange_strong(p, expected, desired) \
    __ccw_atomic_cas((long volatile*)(p), (long*)(expected), (long)(desired))
#define atomic_compare_exchange_weak(p, expected, desired) \
    atomic_compare_exchange_strong(p, expected, desired)
#define atomic_compare_exchange_strong_explicit(p, e, d, s, f) atomic_compare_exchange_strong(p, e, d)
#define atomic_compare_exchange_weak_explicit(p, e, d, s, f)   atomic_compare_exchange_strong(p, e, d)

typedef struct atomic_flag { long volatile _v; } atomic_flag;
#define atomic_flag_test_and_set(p)              (_InterlockedExchange(&(p)->_v, 1) != 0)
#define atomic_flag_test_and_set_explicit(p, mo) atomic_flag_test_and_set(p)
#define atomic_flag_clear(p)                     ((void)_InterlockedExchange(&(p)->_v, 0))
#define atomic_flag_clear_explicit(p, mo)        atomic_flag_clear(p)

#define atomic_thread_fence(mo)   _ReadWriteBarrier()
#define atomic_signal_fence(mo)   _ReadWriteBarrier()

#define kill_dependency(y)        (y)

#endif
