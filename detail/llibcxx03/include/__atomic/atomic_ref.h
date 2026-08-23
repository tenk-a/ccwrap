// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: std::atomic_ref (C++20).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ATOMIC_ATOMIC_REF_H
#define _CCW_LIBCPP___ATOMIC_ATOMIC_REF_H
#include "../__config"
#include "interlocked.h"
#include <cstring>   // memcmp: compare_exchange compares value representations
#include <cstddef>

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp>
struct atomic_ref {
    typedef _Tp value_type;
    typedef _Tp difference_type;
    static const size_t required_alignment = sizeof(_Tp);
    static const bool is_always_lock_free = false;

    _Tp* __ptr_;
    _CCW_LIBCPP_HIDE_FROM_ABI explicit atomic_ref(_Tp& __o) : __ptr_(&__o) {}
    _CCW_LIBCPP_HIDE_FROM_ABI atomic_ref(const atomic_ref& __r) : __ptr_(__r.__ptr_) {}

    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator=(_Tp __d) const { *__ptr_ = __d; return __d; }
    _CCW_LIBCPP_HIDE_FROM_ABI operator _Tp() const { return *__ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool is_lock_free() const { return false; }

    _CCW_LIBCPP_HIDE_FROM_ABI _Tp  load(memory_order = memory_order_seq_cst) const { return *__ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI void store(_Tp __d, memory_order = memory_order_seq_cst) const { *__ptr_ = __d; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp  exchange(_Tp __d, memory_order = memory_order_seq_cst) const { _Tp __o = *__ptr_; *__ptr_ = __d; return __o; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_strong(_Tp& __e, _Tp __d, memory_order = memory_order_seq_cst) const {
        if (_CCW_STD::memcmp((const void*)__ptr_, (const void*)&__e, sizeof(_Tp)) == 0) {
            *__ptr_ = __d; return true;
        }
        __e = *__ptr_; return false;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_strong(_Tp& __e, _Tp __d, memory_order, memory_order) const { return compare_exchange_strong(__e, __d); }
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_weak(_Tp& __e, _Tp __d, memory_order = memory_order_seq_cst) const { return compare_exchange_strong(__e, __d); }
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_weak(_Tp& __e, _Tp __d, memory_order, memory_order) const { return compare_exchange_strong(__e, __d); }

    _CCW_LIBCPP_HIDE_FROM_ABI void wait(_Tp __old, memory_order __mo = memory_order_seq_cst) const {
        for (;;) { _Tp __cur = load(__mo); if (_CCW_STD::memcmp(&__cur, &__old, sizeof(_Tp)) != 0) return; }
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void notify_one() const {}
    _CCW_LIBCPP_HIDE_FROM_ABI void notify_all() const {}

    _CCW_LIBCPP_HIDE_FROM_ABI _Tp fetch_add(_Tp __d, memory_order = memory_order_seq_cst) const { _Tp __o = *__ptr_; *__ptr_ = (_Tp)(__o + __d); return __o; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp fetch_sub(_Tp __d, memory_order = memory_order_seq_cst) const { _Tp __o = *__ptr_; *__ptr_ = (_Tp)(__o - __d); return __o; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp fetch_and(_Tp __d, memory_order = memory_order_seq_cst) const { _Tp __o = *__ptr_; *__ptr_ = (_Tp)(__o & __d); return __o; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp fetch_or (_Tp __d, memory_order = memory_order_seq_cst) const { _Tp __o = *__ptr_; *__ptr_ = (_Tp)(__o | __d); return __o; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp fetch_xor(_Tp __d, memory_order = memory_order_seq_cst) const { _Tp __o = *__ptr_; *__ptr_ = (_Tp)(__o ^ __d); return __o; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator++()    const { return (_Tp)(fetch_add((_Tp)1) + 1); }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator++(int) const { return fetch_add((_Tp)1); }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator--()    const { return (_Tp)(fetch_sub((_Tp)1) - 1); }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator--(int) const { return fetch_sub((_Tp)1); }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator+=(_Tp __d) const { return (_Tp)(fetch_add(__d) + __d); }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator-=(_Tp __d) const { return (_Tp)(fetch_sub(__d) - __d); }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator&=(_Tp __d) const { return (_Tp)(fetch_and(__d) & __d); }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator|=(_Tp __d) const { return (_Tp)(fetch_or (__d) | __d); }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator^=(_Tp __d) const { return (_Tp)(fetch_xor(__d) ^ __d); }
private:
    atomic_ref& operator=(const atomic_ref&);   // copy-assignment is deleted (use operator=(T))
};

#define _CCW_ATOMIC_REF_FLT_SPEC(_Tp)                                                                                  \
template <> struct atomic_ref<_Tp> {                                                                                   \
    typedef _Tp value_type;                                                                                            \
    typedef _Tp difference_type;                                                                                       \
    static const size_t required_alignment = sizeof(_Tp);                                                              \
    static const bool is_always_lock_free = false;                                                                     \
    _Tp* __ptr_;                                                                                                       \
    _CCW_LIBCPP_HIDE_FROM_ABI explicit atomic_ref(_Tp& __o) : __ptr_(&__o) {}                                          \
    _CCW_LIBCPP_HIDE_FROM_ABI atomic_ref(const atomic_ref& __r) : __ptr_(__r.__ptr_) {}                                \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator=(_Tp __d) const { *__ptr_ = __d; return __d; }                              \
    _CCW_LIBCPP_HIDE_FROM_ABI operator _Tp() const { return *__ptr_; }                                                 \
    _CCW_LIBCPP_HIDE_FROM_ABI bool is_lock_free() const { return false; }                                              \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp  load(memory_order = memory_order_seq_cst) const { return *__ptr_; }                 \
    _CCW_LIBCPP_HIDE_FROM_ABI void store(_Tp __d, memory_order = memory_order_seq_cst) const { *__ptr_ = __d; }        \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp  exchange(_Tp __d, memory_order = memory_order_seq_cst) const                        \
        { _Tp __o = *__ptr_; *__ptr_ = __d; return __o; }                                                              \
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_strong(_Tp& __e, _Tp __d, memory_order = memory_order_seq_cst) const \
        { if (*__ptr_ == __e) { *__ptr_ = __d; return true; } __e = *__ptr_; return false; }                           \
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_strong(_Tp& __e, _Tp __d, memory_order, memory_order) const        \
        { return compare_exchange_strong(__e, __d); }                                                                  \
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_weak(_Tp& __e, _Tp __d, memory_order = memory_order_seq_cst) const \
        { return compare_exchange_strong(__e, __d); }                                                                  \
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_weak(_Tp& __e, _Tp __d, memory_order, memory_order) const          \
        { return compare_exchange_strong(__e, __d); }                                                                  \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp fetch_add(_Tp __d, memory_order = memory_order_seq_cst) const                        \
        { _Tp __o = *__ptr_; *__ptr_ += __d; return __o; }                                                             \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp fetch_sub(_Tp __d, memory_order = memory_order_seq_cst) const                        \
        { _Tp __o = *__ptr_; *__ptr_ -= __d; return __o; }                                                             \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator+=(_Tp __d) const { return *__ptr_ += __d; }                                 \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator-=(_Tp __d) const { return *__ptr_ -= __d; }                                 \
    _CCW_LIBCPP_HIDE_FROM_ABI void wait(_Tp __old, memory_order __mo = memory_order_seq_cst) const                     \
        { while (load(__mo) == __old) {} }                                                                             \
    _CCW_LIBCPP_HIDE_FROM_ABI void notify_one() const {}                                                               \
    _CCW_LIBCPP_HIDE_FROM_ABI void notify_all() const {}                                                               \
private:                                                                                                               \
    atomic_ref& operator=(const atomic_ref&);                                                                          \
};
_CCW_ATOMIC_REF_FLT_SPEC(float)
_CCW_ATOMIC_REF_FLT_SPEC(double)
_CCW_ATOMIC_REF_FLT_SPEC(long double)
#undef _CCW_ATOMIC_REF_FLT_SPEC

template <class _Tp>
struct atomic_ref<_Tp*> {
    typedef _Tp*        value_type;
    typedef ptrdiff_t   difference_type;
    static const size_t required_alignment = sizeof(_Tp*);
    static const bool is_always_lock_free = false;

    _Tp** __ptr_;
    _CCW_LIBCPP_HIDE_FROM_ABI explicit atomic_ref(_Tp*& __o) : __ptr_(&__o) {}
    _CCW_LIBCPP_HIDE_FROM_ABI atomic_ref(const atomic_ref& __r) : __ptr_(__r.__ptr_) {}

    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* operator=(_Tp* __d) const { *__ptr_ = __d; return __d; }
    _CCW_LIBCPP_HIDE_FROM_ABI operator _Tp*() const { return *__ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool is_lock_free() const { return false; }

    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* load(memory_order = memory_order_seq_cst) const { return *__ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI void store(_Tp* __d, memory_order = memory_order_seq_cst) const { *__ptr_ = __d; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* exchange(_Tp* __d, memory_order = memory_order_seq_cst) const {
        _Tp* __o = *__ptr_; *__ptr_ = __d; return __o; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_strong(_Tp*& __e, _Tp* __d, memory_order = memory_order_seq_cst) const {
        if (*__ptr_ == __e) { *__ptr_ = __d; return true; } __e = *__ptr_; return false; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_strong(_Tp*& __e, _Tp* __d, memory_order, memory_order) const {
        return compare_exchange_strong(__e, __d); }
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_weak(_Tp*& __e, _Tp* __d, memory_order = memory_order_seq_cst) const {
        return compare_exchange_strong(__e, __d); }
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_weak(_Tp*& __e, _Tp* __d, memory_order, memory_order) const {
        return compare_exchange_strong(__e, __d); }

    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* fetch_add(difference_type __d, memory_order = memory_order_seq_cst) const {
        _Tp* __o = *__ptr_; *__ptr_ += __d; return __o; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* fetch_sub(difference_type __d, memory_order = memory_order_seq_cst) const {
        _Tp* __o = *__ptr_; *__ptr_ -= __d; return __o; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* operator++()    const { return ++*__ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* operator++(int) const { return (*__ptr_)++; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* operator--()    const { return --*__ptr_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* operator--(int) const { return (*__ptr_)--; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* operator+=(difference_type __d) const { return *__ptr_ += __d; }
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp* operator-=(difference_type __d) const { return *__ptr_ -= __d; }

    _CCW_LIBCPP_HIDE_FROM_ABI void wait(_Tp* __old, memory_order __mo = memory_order_seq_cst) const {
        while (load(__mo) == __old) {} }
    _CCW_LIBCPP_HIDE_FROM_ABI void notify_one() const {}
    _CCW_LIBCPP_HIDE_FROM_ABI void notify_all() const {}
private:
    atomic_ref& operator=(const atomic_ref&);
};

#if defined(_CCW_ATOMIC_REAL)
#  define _CCW_ATOMIC_REF_INT_SPEC(_Tp)                                                                                  \
template <> struct atomic_ref<_Tp> {                                                                                    \
    typedef _Tp value_type;                                                                                            \
    typedef _Tp difference_type;                                                                                       \
    static const size_t required_alignment = sizeof(_Tp);                                                             \
    static const bool is_always_lock_free = true;                                                                      \
    _Tp* __ptr_;                                                                                                       \
    _CCW_LIBCPP_HIDE_FROM_ABI explicit atomic_ref(_Tp& __o) : __ptr_(&__o) {}                                              \
    _CCW_LIBCPP_HIDE_FROM_ABI atomic_ref(const atomic_ref& __r) : __ptr_(__r.__ptr_) {}                                    \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator=(_Tp __d) const { store(__d); return __d; }                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI operator _Tp() const { return *__ptr_; }                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI bool is_lock_free() const { return true; }                                                   \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp  load(memory_order = memory_order_seq_cst) const { return *__ptr_; }                     \
    _CCW_LIBCPP_HIDE_FROM_ABI void store(_Tp __d, memory_order = memory_order_seq_cst) const { __ccw_ilk_t_xchg(__ptr_, __d); } \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp  exchange(_Tp __d, memory_order = memory_order_seq_cst) const { return (_Tp)__ccw_ilk_t_xchg(__ptr_, __d); } \
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_strong(_Tp& __e, _Tp __d, memory_order = memory_order_seq_cst) const {  \
        _Tp __old = __ccw_ilk_t_cas(__ptr_, __d, __e);                         \
        if (__old == __e) return true; __e = __old; return false; }                                        \
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_strong(_Tp& __e, _Tp __d, memory_order, memory_order) const { return compare_exchange_strong(__e, __d); } \
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_weak(_Tp& __e, _Tp __d, memory_order = memory_order_seq_cst) const { return compare_exchange_strong(__e, __d); } \
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_weak(_Tp& __e, _Tp __d, memory_order, memory_order) const { return compare_exchange_strong(__e, __d); } \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp fetch_add(_Tp __d, memory_order = memory_order_seq_cst) const { return (_Tp)__ccw_ilk_t_xadd(__ptr_, __d); } \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp fetch_sub(_Tp __d, memory_order = memory_order_seq_cst) const { return (_Tp)__ccw_ilk_t_xadd(__ptr_, (_Tp)(0 - __d)); } \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp fetch_and(_Tp __d, memory_order = memory_order_seq_cst) const { for (;;) { _Tp __o = *(_Tp volatile*)__ptr_, __n = (_Tp)(__o & __d); if (__ccw_ilk_t_cas(__ptr_, __n, __o) == __o) return __o; } } \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp fetch_or(_Tp __d,  memory_order = memory_order_seq_cst) const { for (;;) { _Tp __o = *(_Tp volatile*)__ptr_, __n = (_Tp)(__o | __d); if (__ccw_ilk_t_cas(__ptr_, __n, __o) == __o) return __o; } } \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp fetch_xor(_Tp __d, memory_order = memory_order_seq_cst) const { for (;;) { _Tp __o = *(_Tp volatile*)__ptr_, __n = (_Tp)(__o ^ __d); if (__ccw_ilk_t_cas(__ptr_, __n, __o) == __o) return __o; } } \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator++()    const { return (_Tp)__ccw_ilk_t_inc(__ptr_); }      \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator++(int) const { return (_Tp)(__ccw_ilk_t_inc(__ptr_) - 1); }\
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator--()    const { return (_Tp)__ccw_ilk_t_dec(__ptr_); }      \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator--(int) const { return (_Tp)(__ccw_ilk_t_dec(__ptr_) + 1); }\
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator+=(_Tp __d) const { return (_Tp)(fetch_add(__d) + __d); }                        \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator-=(_Tp __d) const { return (_Tp)(fetch_sub(__d) - __d); }                        \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator&=(_Tp __d) const { return (_Tp)(fetch_and(__d) & __d); } \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator|=(_Tp __d) const { return (_Tp)(fetch_or(__d) | __d); }  \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator^=(_Tp __d) const { return (_Tp)(fetch_xor(__d) ^ __d); } \
    _CCW_LIBCPP_HIDE_FROM_ABI void wait(_Tp __old, memory_order __mo = memory_order_seq_cst) const { while (load(__mo) == __old) {} } \
    _CCW_LIBCPP_HIDE_FROM_ABI void notify_one() const {}                                                                   \
    _CCW_LIBCPP_HIDE_FROM_ABI void notify_all() const {}                                                                   \
private:                                                                                                               \
    atomic_ref& operator=(const atomic_ref&);                                                                         \
};
_CCW_ATOMIC_REF_INT_SPEC(int)
_CCW_ATOMIC_REF_INT_SPEC(unsigned int)
_CCW_ATOMIC_REF_INT_SPEC(long)
_CCW_ATOMIC_REF_INT_SPEC(unsigned long)
#  undef _CCW_ATOMIC_REF_INT_SPEC
#endif // _CCW_ATOMIC_REAL

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP___ATOMIC_ATOMIC_REF_H
