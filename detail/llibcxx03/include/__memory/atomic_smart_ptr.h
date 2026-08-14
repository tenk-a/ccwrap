// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: _CCW_STD::atomic<shared_ptr<T>> / _CCW_STD::atomic<weak_ptr<T>> (C++20).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___MEMORY_ATOMIC_SMART_PTR_H
#define _CCW_LIBCPP___MEMORY_ATOMIC_SMART_PTR_H

#include "../__config"
#include "../__memory/shared_ptr.h"
#include <atomic>                 // the primary atomic<> template + memory_order

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _SmartPtr>
_CCW_LIBCPP_HIDE_FROM_ABI inline bool __ccw_sp_owner_equiv(const _SmartPtr& __a, const _SmartPtr& __b) {
    return !__a.owner_before(__b) && !__b.owner_before(__a);
}

template <class _Tp>
struct atomic<shared_ptr<_Tp> > {
    typedef shared_ptr<_Tp> value_type;
    static const bool is_always_lock_free = false;

    shared_ptr<_Tp> __v_;
    _CCW_LIBCPP_HIDE_FROM_ABI atomic() {}
    _CCW_LIBCPP_HIDE_FROM_ABI atomic(shared_ptr<_Tp> __d) : __v_(__d) {}

    _CCW_LIBCPP_HIDE_FROM_ABI bool is_lock_free() const { return false; }

    _CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> load(memory_order = memory_order_seq_cst) const { return __v_; }
    _CCW_LIBCPP_HIDE_FROM_ABI void store(shared_ptr<_Tp> __d, memory_order = memory_order_seq_cst) { __v_ = __d; }
    _CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> exchange(shared_ptr<_Tp> __d, memory_order = memory_order_seq_cst) {
        shared_ptr<_Tp> __o = __v_; __v_ = __d; return __o;
    }

    _CCW_LIBCPP_HIDE_FROM_ABI operator shared_ptr<_Tp>() const { return __v_; }
    _CCW_LIBCPP_HIDE_FROM_ABI shared_ptr<_Tp> operator=(shared_ptr<_Tp> __d) { __v_ = __d; return __d; }

    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_strong(shared_ptr<_Tp>& __exp, shared_ptr<_Tp> __des, memory_order = memory_order_seq_cst) {
        if (__v_.get() == __exp.get() && __ccw_sp_owner_equiv(__v_, __exp)) { __v_ = __des; return true; }
        __exp = __v_; return false;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_strong(shared_ptr<_Tp>& __exp, shared_ptr<_Tp> __des, memory_order, memory_order) {
        return compare_exchange_strong(__exp, __des);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_weak(shared_ptr<_Tp>& __exp, shared_ptr<_Tp> __des, memory_order = memory_order_seq_cst) {
        return compare_exchange_strong(__exp, __des);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_weak(shared_ptr<_Tp>& __exp, shared_ptr<_Tp> __des, memory_order, memory_order) {
        return compare_exchange_strong(__exp, __des);
    }

    _CCW_LIBCPP_HIDE_FROM_ABI void wait(shared_ptr<_Tp> __old, memory_order = memory_order_seq_cst) const {
        while (__v_.get() == __old.get() && __ccw_sp_owner_equiv(__v_, __old)) {}
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void notify_one() {}
    _CCW_LIBCPP_HIDE_FROM_ABI void notify_all() {}
private:
    atomic(const atomic&);
    atomic& operator=(const atomic&);
};

template <class _Tp>
struct atomic<weak_ptr<_Tp> > {
    typedef weak_ptr<_Tp> value_type;
    static const bool is_always_lock_free = false;

    weak_ptr<_Tp> __v_;
    _CCW_LIBCPP_HIDE_FROM_ABI atomic() {}
    _CCW_LIBCPP_HIDE_FROM_ABI atomic(weak_ptr<_Tp> __d) : __v_(__d) {}

    _CCW_LIBCPP_HIDE_FROM_ABI bool is_lock_free() const { return false; }

    _CCW_LIBCPP_HIDE_FROM_ABI weak_ptr<_Tp> load(memory_order = memory_order_seq_cst) const { return __v_; }
    _CCW_LIBCPP_HIDE_FROM_ABI void store(weak_ptr<_Tp> __d, memory_order = memory_order_seq_cst) { __v_ = __d; }
    _CCW_LIBCPP_HIDE_FROM_ABI weak_ptr<_Tp> exchange(weak_ptr<_Tp> __d, memory_order = memory_order_seq_cst) {
        weak_ptr<_Tp> __o = __v_; __v_ = __d; return __o;
    }

    _CCW_LIBCPP_HIDE_FROM_ABI operator weak_ptr<_Tp>() const { return __v_; }
    _CCW_LIBCPP_HIDE_FROM_ABI weak_ptr<_Tp> operator=(weak_ptr<_Tp> __d) { __v_ = __d; return __d; }

    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_strong(weak_ptr<_Tp>& __exp, weak_ptr<_Tp> __des, memory_order = memory_order_seq_cst) {
        if (__ccw_sp_owner_equiv(__v_, __exp)) { __v_ = __des; return true; }
        __exp = __v_; return false;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_strong(weak_ptr<_Tp>& __exp, weak_ptr<_Tp> __des, memory_order, memory_order) {
        return compare_exchange_strong(__exp, __des);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_weak(weak_ptr<_Tp>& __exp, weak_ptr<_Tp> __des, memory_order = memory_order_seq_cst) {
        return compare_exchange_strong(__exp, __des);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI bool compare_exchange_weak(weak_ptr<_Tp>& __exp, weak_ptr<_Tp> __des, memory_order, memory_order) {
        return compare_exchange_strong(__exp, __des);
    }

    _CCW_LIBCPP_HIDE_FROM_ABI void wait(weak_ptr<_Tp> __old, memory_order = memory_order_seq_cst) const {
        while (__ccw_sp_owner_equiv(__v_, __old)) {}
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void notify_one() {}
    _CCW_LIBCPP_HIDE_FROM_ABI void notify_all() {}
private:
    atomic(const atomic&);
    atomic& operator=(const atomic&);
};

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP___MEMORY_ATOMIC_SMART_PTR_H
