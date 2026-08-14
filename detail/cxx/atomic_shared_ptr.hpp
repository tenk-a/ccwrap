/**
 *  @file   detail/cxx/atomic_shared_ptr.hpp
 *  @brief  C++20 std::atomic<shared_ptr<T>> / std::atomic<weak_ptr<T>>, on a spin lock.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_ATOMIC_SHARED_PTR_HPP
#define _CCW_DETAIL_ATOMIC_SHARED_PTR_HPP

#include <ccwrap_common.h>

#if defined(__cplusplus) && !defined(__cpp_lib_atomic_shared_ptr)

#include <atomic>
#include <memory>

namespace _ccw { namespace detail {

class __ccw_sp_spin {
public:
    explicit __ccw_sp_spin(::std::atomic<int>& __f) : __f_(__f) {
        for (;;) { int __x = 0; if (__f_.compare_exchange_weak(__x, 1)) break; }
    }
    ~__ccw_sp_spin() { __f_.store(0); }
private:
    ::std::atomic<int>& __f_;
    __ccw_sp_spin(const __ccw_sp_spin&);
    __ccw_sp_spin& operator=(const __ccw_sp_spin&);
};

}}  // namespace _ccw::detail

#define _CCW_ATOMIC_SMART_PTR_BODY(_Sp)                                                       \
    typedef _Sp<_Tp> value_type;                                                              \
    atomic() : __lk_(0) {}                                                                    \
    atomic(_Sp<_Tp> __d) : __lk_(0), __p_(__d) {}                                             \
    void operator=(_Sp<_Tp> __d) { store(__d); }                                              \
    operator _Sp<_Tp>() const { return load(); }                                              \
    bool is_lock_free() const { return false; }                                               \
    void store(_Sp<_Tp> __d, ::std::memory_order = ::std::memory_order_seq_cst) {             \
        ::_ccw::detail::__ccw_sp_spin __g(__lk_); __p_.swap(__d); }                           \
    _Sp<_Tp> load(::std::memory_order = ::std::memory_order_seq_cst) const {                  \
        ::_ccw::detail::__ccw_sp_spin __g(__lk_); return __p_; }                              \
    _Sp<_Tp> exchange(_Sp<_Tp> __d, ::std::memory_order = ::std::memory_order_seq_cst) {      \
        ::_ccw::detail::__ccw_sp_spin __g(__lk_); __p_.swap(__d); return __d; }               \
    bool compare_exchange_strong(_Sp<_Tp>& __e, _Sp<_Tp> __d,                                 \
            ::std::memory_order = ::std::memory_order_seq_cst) {                              \
        ::_ccw::detail::__ccw_sp_spin __g(__lk_);                                             \
        if (__ccw_same(__p_, __e)) { __p_ = __d; return true; }                               \
        __e = __p_; return false; }                                                           \
    bool compare_exchange_strong(_Sp<_Tp>& __e, _Sp<_Tp> __d,                                 \
            ::std::memory_order, ::std::memory_order) {                                       \
        return compare_exchange_strong(__e, __d); }                                           \
    bool compare_exchange_weak(_Sp<_Tp>& __e, _Sp<_Tp> __d,                                   \
            ::std::memory_order = ::std::memory_order_seq_cst) {                              \
        return compare_exchange_strong(__e, __d); }                                           \
    bool compare_exchange_weak(_Sp<_Tp>& __e, _Sp<_Tp> __d,                                   \
            ::std::memory_order, ::std::memory_order) {                                       \
        return compare_exchange_strong(__e, __d); }                                           \
    void wait(_Sp<_Tp> __old, ::std::memory_order = ::std::memory_order_seq_cst) const {      \
        while (__ccw_same_v(load(), __old)) {} }                                              \
    void notify_one() {}                                                                      \
    void notify_all() {}                                                                      \
    static const bool is_always_lock_free = false;                                            \
private:                                                                                      \
    mutable ::std::atomic<int> __lk_;                                                         \
    _Sp<_Tp> __p_;                                                                            \
    atomic(const atomic&);                                                                    \
    atomic& operator=(const atomic&)

namespace std {

template <class _Tp>
struct atomic< shared_ptr<_Tp> > {
    static bool __ccw_same(const shared_ptr<_Tp>& __a, const shared_ptr<_Tp>& __b) {
        return __a.get() == __b.get(); }
    static bool __ccw_same_v(const shared_ptr<_Tp>& __a, const shared_ptr<_Tp>& __b) {
        return __a.get() == __b.get(); }
public:
    _CCW_ATOMIC_SMART_PTR_BODY(shared_ptr);
};

template <class _Tp>
struct atomic< weak_ptr<_Tp> > {
    static bool __ccw_same(const weak_ptr<_Tp>& __a, const weak_ptr<_Tp>& __b) {
        return !__a.owner_before(__b) && !__b.owner_before(__a); }
    static bool __ccw_same_v(const weak_ptr<_Tp>& __a, const weak_ptr<_Tp>& __b) {
        return !__a.owner_before(__b) && !__b.owner_before(__a); }
public:
    _CCW_ATOMIC_SMART_PTR_BODY(weak_ptr);
};

}   // namespace std

#undef _CCW_ATOMIC_SMART_PTR_BODY

#endif  // !__cpp_lib_atomic_shared_ptr
#endif  // _CCW_DETAIL_ATOMIC_SHARED_PTR_HPP
