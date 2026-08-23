// -*- C++ -*-
//===----------------------------------------------------------------------===//
// @license Boost Software License Version 1.0
// ccwrap llibcxx03: the Interlocked shims <atomic> and <__atomic/atomic_ref.h> share.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ATOMIC_INTERLOCKED_H
#define _CCW_LIBCPP___ATOMIC_INTERLOCKED_H
#include "../__config"

#if defined(__WATCOMC__) || defined(_MSC_VER)
#  include <intrin.h>
#  if defined(_MSC_VER)
#    pragma intrinsic(_InterlockedExchange, _InterlockedExchangeAdd, _InterlockedCompareExchange)
#    pragma intrinsic(_InterlockedIncrement, _InterlockedDecrement)
#  endif
inline long __ccw_ilk_xchg(long volatile* __p, long __v)          { return _InterlockedExchange(__p, __v); }
inline long __ccw_ilk_xadd(long volatile* __p, long __v)          { return _InterlockedExchangeAdd(__p, __v); }
inline long __ccw_ilk_cas(long volatile* __p, long __n, long __c) { return _InterlockedCompareExchange(__p, __n, __c); }
inline long __ccw_ilk_inc(long volatile* __p)                     { return _InterlockedIncrement(__p); }
inline long __ccw_ilk_dec(long volatile* __p)                     { return _InterlockedDecrement(__p); }

#  if defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) || defined(_M_ARM64)
typedef __int64 __ccw_ilk_iptr;
#    if defined(_MSC_VER)
#      pragma intrinsic(_InterlockedExchange64, _InterlockedExchangeAdd64, _InterlockedCompareExchange64)
#    endif
inline __ccw_ilk_iptr __ccw_ilk_pxchg(__ccw_ilk_iptr volatile* __p, __ccw_ilk_iptr __v) { return _InterlockedExchange64(__p, __v); }
inline __ccw_ilk_iptr __ccw_ilk_pxadd(__ccw_ilk_iptr volatile* __p, __ccw_ilk_iptr __v) { return _InterlockedExchangeAdd64(__p, __v); }
inline __ccw_ilk_iptr __ccw_ilk_pcas(__ccw_ilk_iptr volatile* __p, __ccw_ilk_iptr __n, __ccw_ilk_iptr __c) { return _InterlockedCompareExchange64(__p, __n, __c); }
#  else
typedef long __ccw_ilk_iptr;
inline __ccw_ilk_iptr __ccw_ilk_pxchg(__ccw_ilk_iptr volatile* __p, __ccw_ilk_iptr __v) { return __ccw_ilk_xchg(__p, __v); }
inline __ccw_ilk_iptr __ccw_ilk_pxadd(__ccw_ilk_iptr volatile* __p, __ccw_ilk_iptr __v) { return __ccw_ilk_xadd(__p, __v); }
inline __ccw_ilk_iptr __ccw_ilk_pcas(__ccw_ilk_iptr volatile* __p, __ccw_ilk_iptr __n, __ccw_ilk_iptr __c) { return __ccw_ilk_cas(__p, __n, __c); }
#  endif
#  define _CCW_ATOMIC_REAL 1

#elif defined(__GNUC__)

inline long __ccw_ilk_xchg(long volatile* __p, long __v)          { return __sync_lock_test_and_set(__p, __v); }
inline long __ccw_ilk_xadd(long volatile* __p, long __v)          { return __sync_fetch_and_add(__p, __v); }
inline long __ccw_ilk_cas(long volatile* __p, long __n, long __c) { return __sync_val_compare_and_swap(__p, __c, __n); }
inline long __ccw_ilk_inc(long volatile* __p)                     { return __sync_add_and_fetch(__p, 1L); }
inline long __ccw_ilk_dec(long volatile* __p)                     { return __sync_sub_and_fetch(__p, 1L); }

#  if defined(__SIZEOF_POINTER__) && defined(__SIZEOF_LONG__) && __SIZEOF_POINTER__ > __SIZEOF_LONG__
typedef long long __ccw_ilk_iptr;
#  else
typedef long __ccw_ilk_iptr;
#  endif
inline __ccw_ilk_iptr __ccw_ilk_pxchg(__ccw_ilk_iptr volatile* __p, __ccw_ilk_iptr __v) { return __sync_lock_test_and_set(__p, __v); }
inline __ccw_ilk_iptr __ccw_ilk_pxadd(__ccw_ilk_iptr volatile* __p, __ccw_ilk_iptr __v) { return __sync_fetch_and_add(__p, __v); }
inline __ccw_ilk_iptr __ccw_ilk_pcas(__ccw_ilk_iptr volatile* __p, __ccw_ilk_iptr __n, __ccw_ilk_iptr __c) { return __sync_val_compare_and_swap(__p, __c, __n); }

#  define _CCW_ATOMIC_REAL 1
#  define _CCW_ATOMIC_ILK_TYPED 1
#endif

#if defined(_CCW_ATOMIC_REAL)
#  if defined(_CCW_ATOMIC_ILK_TYPED)
template <class _Tp> inline _Tp __ccw_ilk_t_xchg(_Tp volatile* __p, _Tp __v)          { return __sync_lock_test_and_set(__p, __v); }
template <class _Tp> inline _Tp __ccw_ilk_t_xadd(_Tp volatile* __p, _Tp __v)          { return __sync_fetch_and_add(__p, __v); }
template <class _Tp> inline _Tp __ccw_ilk_t_cas(_Tp volatile* __p, _Tp __n, _Tp __c)  { return __sync_val_compare_and_swap(__p, __c, __n); }
template <class _Tp> inline _Tp __ccw_ilk_t_inc(_Tp volatile* __p)                    { return __sync_add_and_fetch(__p, (_Tp)1); }
template <class _Tp> inline _Tp __ccw_ilk_t_dec(_Tp volatile* __p)                    { return __sync_sub_and_fetch(__p, (_Tp)1); }
#  else
template <class _Tp> inline _Tp __ccw_ilk_t_xchg(_Tp volatile* __p, _Tp __v)          { return (_Tp)__ccw_ilk_xchg((long volatile*)__p, (long)__v); }
template <class _Tp> inline _Tp __ccw_ilk_t_xadd(_Tp volatile* __p, _Tp __v)          { return (_Tp)__ccw_ilk_xadd((long volatile*)__p, (long)__v); }
template <class _Tp> inline _Tp __ccw_ilk_t_cas(_Tp volatile* __p, _Tp __n, _Tp __c)  { return (_Tp)__ccw_ilk_cas((long volatile*)__p, (long)__n, (long)__c); }
template <class _Tp> inline _Tp __ccw_ilk_t_inc(_Tp volatile* __p)                    { return (_Tp)__ccw_ilk_inc((long volatile*)__p); }
template <class _Tp> inline _Tp __ccw_ilk_t_dec(_Tp volatile* __p)                    { return (_Tp)__ccw_ilk_dec((long volatile*)__p); }
#  endif
#endif
#endif // _CCW_LIBCPP___ATOMIC_INTERLOCKED_H
