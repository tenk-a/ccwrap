//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: _CCW_STD::vector for Open Watcom. libc++'s file layout is kept;
//  the body is a classic three-pointer dynamic array over an allocator.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___VECTOR_VECTOR_H
#define _CCW_LIBCPP___VECTOR_VECTOR_H
#include "../__config"
#include "../__memory/misc.h"   // uses_allocator
#include "../__fnctmpl.h"
#include "../__type_traits/integral_dispatch.h"
#include "../__utility/move.h"
#include "../__memory/allocator.h"
#include "../../cxx/ccw_check.hpp"
#include "../__memory/uninitialized_algorithms.h"
#include "../__iterator/reverse_iterator.h"
#include <cstddef>
#include <stdexcept>
#if _CCW_HAS_REAL_RVALUE_REF
#  include <initializer_list>
#endif
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp, class _Alloc = allocator<_Tp> >
class _CCW_LIBCPP_TEMPLATE_VIS vector {
public:
    typedef _Tp                                  value_type;
    typedef _Alloc                               allocator_type;
    typedef _CCW_STD::size_t                          size_type;
    typedef _CCW_STD::ptrdiff_t                       difference_type;
    typedef _Tp&                                 reference;
    typedef const _Tp&                           const_reference;
    typedef _Tp*                                 pointer;
    typedef const _Tp*                           const_pointer;
    typedef _Tp*                                 iterator;
    typedef const _Tp*                           const_iterator;
    typedef _CCW_STD::reverse_iterator<iterator>       reverse_iterator;
    typedef _CCW_STD::reverse_iterator<const_iterator> const_reverse_iterator;

private:
    struct __cap_alloc : allocator_type {
        pointer __cap_;
        _CCW_LIBCPP_HIDE_FROM_ABI __cap_alloc() : __cap_(0) {}
        _CCW_LIBCPP_HIDE_FROM_ABI explicit __cap_alloc(const allocator_type& __a) : allocator_type(__a), __cap_(0) {}
    };
    __cap_alloc __ca_;
    pointer     __begin_;
    pointer     __end_;

    _CCW_LIBCPP_HIDE_FROM_ABI allocator_type&       __alloc()       { return __ca_; }
    _CCW_LIBCPP_HIDE_FROM_ABI const allocator_type& __alloc() const { return __ca_; }

    _CCW_LIBCPP_HIDE_FROM_ABI void __chk() const {
        _CCW_CHK(__begin_ <= __end_ && __end_ <= __ca_.__cap_, "vector range",
                 this, (void*)__begin_, (void*)__end_, (void*)__ca_.__cap_);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void __destroy_all() { __chk(); while (__end_ != __begin_) { --__end_; __alloc().destroy(__end_); } }
    _CCW_LIBCPP_HIDE_FROM_ABI void __reallocate(size_type __nc) {
        pointer __nb = __alloc().allocate(__nc);
        size_type __sz = size();
        _CCW_STD::uninitialized_copy(__begin_, __end_, __nb);
        __destroy_all();
        if (__begin_) __alloc().deallocate(__begin_, capacity());
        __begin_ = __nb; __end_ = __nb + __sz; __ca_.__cap_ = __nb + __nc;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void __ensure_one() { if (__end_ == __ca_.__cap_) __reallocate(capacity() ? capacity() * 2 : 1); }
    _CCW_LIBCPP_HIDE_FROM_ABI void __reallocate_with(size_type __nc, const _Tp& __v) {
        pointer __nb = __alloc().allocate(__nc);
        size_type __sz = size();
        __alloc().construct(__nb + __sz, __v);            // before the old buffer dies
        _CCW_STD::uninitialized_copy(__begin_, __end_, __nb);
        __destroy_all();
        if (__begin_) __alloc().deallocate(__begin_, capacity());
        __begin_ = __nb; __end_ = __nb + __sz + 1; __ca_.__cap_ = __nb + __nc;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI pointer __make_gap(size_type __idx) {
        pointer __p = __begin_ + __idx;
        if (__p != __end_) {
            __alloc().construct(__end_, *(__end_ - 1));
            for (pointer __q = __end_ - 1; __q != __p; --__q) *__q = *(__q - 1);
        }
        return __p;
    }

public:
    _CCW_LIBCPP_HIDE_FROM_ABI vector() : __begin_(0), __end_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI explicit vector(const allocator_type& __a)
        : __ca_(__a), __begin_(0), __end_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI explicit vector(size_type __n, const _Tp& __v = _Tp())
        : __begin_(0), __end_(0) {
        if (__n) { __begin_ = __alloc().allocate(__n); __ca_.__cap_ = __begin_ + __n;
                   _CCW_STD::uninitialized_fill_n(__begin_, __n, __v); __end_ = __begin_ + __n; }
    }
    template <class _InputIt>
    _CCW_LIBCPP_HIDE_FROM_ABI vector(_InputIt __f, _InputIt __l)
        : __begin_(0), __end_(0) {
        __ctor_disp(__f, __l, typename __ccw_dispatch_is_int<_InputIt>::_Tag());
    }
    template <class _T1, class _T2>
    _CCW_LIBCPP_HIDE_FROM_ABI vector(_T1 __a1, _T2 __a2, const allocator_type& __al)
        : __ca_(__al), __begin_(0), __end_(0) {
        __ctor3_disp(__a1, __a2, typename __ccw_dispatch_is_int<_T1>::_Tag());
    }
    _CCW_LIBCPP_HIDE_FROM_ABI vector(const vector& __o, const allocator_type& __a)
        : __ca_(__a), __begin_(0), __end_(0) {
        size_type __n = __o.size();
        if (__n) { __begin_ = __alloc().allocate(__n); __ca_.__cap_ = __begin_ + __n;
                   _CCW_STD::uninitialized_copy(__o.__begin_, __o.__end_, __begin_); __end_ = __begin_ + __n; }
    }
    _CCW_LIBCPP_HIDE_FROM_ABI vector(const vector& __o) : __ca_(__o.__alloc()), __begin_(0), __end_(0) {
        size_type __n = __o.size();
        if (__n) { __begin_ = __alloc().allocate(__n); __ca_.__cap_ = __begin_ + __n;
                   _CCW_STD::uninitialized_copy(__o.__begin_, __o.__end_, __begin_); __end_ = __begin_ + __n; }
    }
    _CCW_LIBCPP_HIDE_FROM_ABI vector& operator=(const vector& __o) {
        if (this != &__o) { clear(); reserve(__o.size());
            _CCW_STD::uninitialized_copy(__o.__begin_, __o.__end_, __begin_); __end_ = __begin_ + __o.size(); }
        return *this;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI ~vector() { __destroy_all(); if (__begin_) __alloc().deallocate(__begin_, capacity()); }

#if _CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI vector(vector&& __o) _CCW_LIBCPP_NOEXCEPT
        : __ca_(__o.__alloc()), __begin_(__o.__begin_), __end_(__o.__end_) {
        __ca_.__cap_ = __o.__ca_.__cap_;
        __o.__begin_ = __o.__end_ = __o.__ca_.__cap_ = 0;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI vector& operator=(vector&& __o) _CCW_LIBCPP_NOEXCEPT {
        if (this != &__o) {
            __destroy_all(); if (__begin_) __alloc().deallocate(__begin_, capacity());
            __begin_ = __o.__begin_; __end_ = __o.__end_; __ca_.__cap_ = __o.__ca_.__cap_;
            __o.__begin_ = __o.__end_ = __o.__ca_.__cap_ = 0;
        }
        return *this;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI vector(_CCW_STD::initializer_list<_Tp> __il)
        : __begin_(0), __end_(0) { __assign_range(__il.begin(), __il.end()); }
    _CCW_LIBCPP_HIDE_FROM_ABI void push_back(_Tp&& __v) {
        __ensure_one(); __alloc().construct(__end_, static_cast<_Tp&&>(__v)); ++__end_;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI iterator insert(iterator __pos, _Tp&& __v) {
        size_type __idx = static_cast<size_type>(__pos - __begin_);
        __ensure_one();
        pointer __p = __make_gap(__idx);
        if (__p == __end_) __alloc().construct(__p, static_cast<_Tp&&>(__v));
        else               *__p = static_cast<_Tp&&>(__v);
        ++__end_;
        return __begin_ + __idx;
    }
#else // !_CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI vector(_CCW_RV_REF(vector) __r)
        : __begin_(0), __end_(0) {
        vector& __o = _CCW_MOVED_OBJ(__r);
        __alloc() = __o.__alloc();
        __begin_ = __o.__begin_; __end_ = __o.__end_; __ca_.__cap_ = __o.__ca_.__cap_;
        __o.__begin_ = __o.__end_ = __o.__ca_.__cap_ = 0;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI vector& operator=(_CCW_RV_REF(vector) __r) {
        vector& __o = _CCW_MOVED_OBJ(__r);
        if (this != &__o) {
            __destroy_all(); if (__begin_) __alloc().deallocate(__begin_, capacity());
            __alloc() = __o.__alloc();
            __begin_ = __o.__begin_; __end_ = __o.__end_; __ca_.__cap_ = __o.__ca_.__cap_;
            __o.__begin_ = __o.__end_ = __o.__ca_.__cap_ = 0;
        }
        return *this;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void push_back(_CCW_RV_REF(_Tp) __v) {
        __ensure_one(); __alloc().construct(__end_, __v); ++__end_;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI iterator insert(iterator __pos, _CCW_RV_REF(_Tp) __v) {
        size_type __idx = static_cast<size_type>(__pos - __begin_);
        __ensure_one();
        pointer __p = __make_gap(__idx);
        if (__p == __end_) __alloc().construct(__p, __v);
        else               *__p = __v;
        ++__end_;
        return __begin_ + __idx;
    }
#endif // (_ccw_cplusplus >= 201103L)

    _CCW_LIBCPP_HIDE_FROM_ABI size_type size() const     { return static_cast<size_type>(__end_ - __begin_); }
    _CCW_LIBCPP_HIDE_FROM_ABI size_type capacity() const { return static_cast<size_type>(__ca_.__cap_ - __begin_); }
    _CCW_LIBCPP_HIDE_FROM_ABI bool      empty() const    { return __begin_ == __end_; }
    _CCW_LIBCPP_HIDE_FROM_ABI size_type max_size() const { return __alloc().max_size(); }

    _CCW_LIBCPP_HIDE_FROM_ABI reference       operator[](size_type __i)       { return __begin_[__i]; }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reference operator[](size_type __i) const { return __begin_[__i]; }
    _CCW_LIBCPP_HIDE_FROM_ABI reference       at(size_type __i)       { if (__i >= size()) _CCW_THROW(_CCW_STD::out_of_range("vector::at")); return __begin_[__i]; }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reference at(size_type __i) const { if (__i >= size()) _CCW_THROW(_CCW_STD::out_of_range("vector::at")); return __begin_[__i]; }
    _CCW_LIBCPP_HIDE_FROM_ABI reference       front()       { return *__begin_; }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reference front() const { return *__begin_; }
    _CCW_LIBCPP_HIDE_FROM_ABI reference       back()        { return *(__end_ - 1); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reference back() const  { return *(__end_ - 1); }
    _CCW_LIBCPP_HIDE_FROM_ABI pointer         data()        { return __begin_; }
    _CCW_LIBCPP_HIDE_FROM_ABI const_pointer   data() const  { return __begin_; }

    _CCW_LIBCPP_HIDE_FROM_ABI iterator       begin()       { return __begin_; }
    _CCW_LIBCPP_HIDE_FROM_ABI const_iterator begin() const { return __begin_; }
    _CCW_LIBCPP_HIDE_FROM_ABI iterator       end()         { return __end_; }
    _CCW_LIBCPP_HIDE_FROM_ABI const_iterator end() const   { return __end_; }
    _CCW_LIBCPP_HIDE_FROM_ABI const_iterator cbegin() const { return __begin_; }
    _CCW_LIBCPP_HIDE_FROM_ABI const_iterator cend() const   { return __end_; }
    _CCW_LIBCPP_HIDE_FROM_ABI reverse_iterator       rbegin()        { return reverse_iterator(end()); }
    _CCW_LIBCPP_HIDE_FROM_ABI reverse_iterator       rend()          { return reverse_iterator(begin()); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reverse_iterator rbegin() const  { return const_reverse_iterator(end()); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reverse_iterator rend() const    { return const_reverse_iterator(begin()); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reverse_iterator crend() const   { return const_reverse_iterator(begin()); }
    _CCW_LIBCPP_HIDE_FROM_ABI void swap(vector& __o) {
        pointer __tb = __begin_; __begin_ = __o.__begin_; __o.__begin_ = __tb;
        pointer __te = __end_;   __end_   = __o.__end_;   __o.__end_   = __te;
        __cap_alloc __t = __ca_; __ca_ = __o.__ca_; __o.__ca_ = __t;   // cap + allocator together
    }

    _CCW_LIBCPP_HIDE_FROM_ABI void reserve(size_type __n) {
        if (__n > max_size()) _CCW_THROW(_CCW_STD::length_error("vector::reserve"));
        if (__n > capacity()) __reallocate(__n);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void shrink_to_fit()        { if (__ca_.__cap_ != __end_ && __begin_) __reallocate(size()); }
    _CCW_LIBCPP_HIDE_FROM_ABI void push_back(const _Tp& __v) {
        if (__end_ == __ca_.__cap_) { __reallocate_with(capacity() ? capacity() * 2 : 1, __v); return; }
        __alloc().construct(__end_, __v); ++__end_;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void pop_back() { --__end_; __alloc().destroy(__end_); }
    _CCW_LIBCPP_HIDE_FROM_ABI void clear()    { __destroy_all(); }
    _CCW_LIBCPP_HIDE_FROM_ABI void resize(size_type __n, const _Tp& __v = _Tp()) {
        if (__n < size()) { while (size() > __n) pop_back(); }
        else { reserve(__n); while (size() < __n) push_back(__v); }
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void assign(size_type __n, const _Tp& __v) {
        clear(); reserve(__n); while (size() < __n) push_back(__v);
    }
    template <class _InputIt>
    _CCW_LIBCPP_HIDE_FROM_ABI void assign(_InputIt __f, _InputIt __l) {
        __assign_disp(__f, __l, typename __ccw_dispatch_is_int<_InputIt>::_Tag());
    }

    _CCW_LIBCPP_HIDE_FROM_ABI iterator insert(iterator __pos, const _Tp& __v) {
        size_type __idx = static_cast<size_type>(__pos - __begin_);
        __ensure_one();
        pointer __p = __make_gap(__idx);
        if (__p == __end_) __alloc().construct(__p, __v); else *__p = __v;
        ++__end_;
        return __begin_ + __idx;
    }
    template <class _A1, class _A2>
    _CCW_LIBCPP_HIDE_FROM_ABI iterator insert(iterator __pos, _A1 __a, _A2 __b) {
        size_type __idx = static_cast<size_type>(__pos - __begin_);
        __insert_disp(__idx, __a, __b, typename __ccw_dispatch_is_int<_A1>::_Tag());
        return __begin_ + __idx;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI iterator erase(iterator __pos) {
        for (pointer __q = __pos; __q + 1 != __end_; ++__q) *__q = *(__q + 1);
        --__end_; __alloc().destroy(__end_);
        return __pos;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI iterator erase(iterator __f, iterator __l) {
        pointer __w = __f;
        for (pointer __r = __l; __r != __end_; ++__r, ++__w) *__w = *__r;
        while (__end_ != __w) { --__end_; __alloc().destroy(__end_); }
        return __f;
    }

#if _CCW_NO_VARIADIC_TEMPLATES
    _CCW_LIBCPP_HIDE_FROM_ABI reference emplace_back() { __ensure_one(); __alloc().construct(__end_, value_type()); ++__end_; return back(); }
    _CCW_LIBCPP_HIDE_FROM_ABI iterator  emplace(iterator __pos) {
        size_type __idx = static_cast<size_type>(__pos - __begin_);
        __ensure_one(); pointer __p = __make_gap(__idx);
        if (__p == __end_) __alloc().construct(__p, value_type()); else *__p = value_type();
        ++__end_; return __begin_ + __idx;
    }
#  define _CCW_GEN_VEC_EMPLACE(N)                                                             \
    template <_CCW_FNCTMPL_TMPLARG(N)>                                                        \
    _CCW_LIBCPP_HIDE_FROM_ABI reference emplace_back(_CCW_FNCTMPL_DECLARG(N)) {                   \
        __ensure_one(); __alloc().construct(__end_, value_type(_CCW_FNCTMPL_FNCARG(N)));       \
        ++__end_; return back();                                                              \
    }                                                                                         \
    template <_CCW_FNCTMPL_TMPLARG(N)>                                                        \
    _CCW_LIBCPP_HIDE_FROM_ABI iterator emplace(iterator __pos _CCW_FNCTMPL_CM_DECLARG(N)) {       \
        size_type __idx = static_cast<size_type>(__pos - __begin_);                           \
        __ensure_one(); pointer __p = __make_gap(__idx);                                      \
        value_type __tmp(_CCW_FNCTMPL_FNCARG(N));                                             \
        if (__p == __end_) __alloc().construct(__p, __tmp); else *__p = __tmp;                 \
        ++__end_; return __begin_ + __idx;                                                    \
    }
    _CCW_GEN_VEC_EMPLACE(1) _CCW_GEN_VEC_EMPLACE(2) _CCW_GEN_VEC_EMPLACE(3) _CCW_GEN_VEC_EMPLACE(4)
#  undef _CCW_GEN_VEC_EMPLACE
#else
    template <class... _Args>
    _CCW_LIBCPP_HIDE_FROM_ABI reference emplace_back(_Args&&... __a) {
        __ensure_one(); __alloc().construct(__end_, value_type(static_cast<_Args&&>(__a)...));
        ++__end_; return back();
    }
    template <class... _Args>
    _CCW_LIBCPP_HIDE_FROM_ABI iterator emplace(iterator __pos, _Args&&... __a) {
        size_type __idx = static_cast<size_type>(__pos - __begin_);
        __ensure_one(); pointer __p = __make_gap(__idx);
        value_type __tmp(static_cast<_Args&&>(__a)...);
        if (__p == __end_) __alloc().construct(__p, __tmp); else *__p = __tmp;
        ++__end_; return __begin_ + __idx;
    }
#endif // _CCW_NO_VARIADIC_TEMPLATES

    _CCW_LIBCPP_HIDE_FROM_ABI allocator_type get_allocator() const { return __alloc(); }

private:
    template <class _InputIt>
    _CCW_LIBCPP_HIDE_FROM_ABI void __assign_range(_InputIt __f, _InputIt __l) {
        for (; __f != __l; ++__f) push_back(*__f);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void __fill_assign(size_type __n, const _Tp& __v) {
        reserve(__n); while (size() < __n) push_back(__v);
    }
    template <class _Integer>
    _CCW_LIBCPP_HIDE_FROM_ABI void __ctor_disp(_Integer __n, _Integer __v, __ccw_int_true_tag) {
        __fill_assign(static_cast<size_type>(__n), static_cast<_Tp>(__v));
    }
    template <class _InputIt>
    _CCW_LIBCPP_HIDE_FROM_ABI void __ctor_disp(_InputIt __f, _InputIt __l, __ccw_int_false_tag) { __assign_range(__f, __l); }
    template <class _Integer, class _Vp>
    _CCW_LIBCPP_HIDE_FROM_ABI void __ctor3_disp(_Integer __n, const _Vp& __v, __ccw_int_true_tag) {
        __fill_assign(static_cast<size_type>(__n), static_cast<_Tp>(__v));
    }
    template <class _InputIt>
    _CCW_LIBCPP_HIDE_FROM_ABI void __ctor3_disp(_InputIt __f, _InputIt __l, __ccw_int_false_tag) { __assign_range(__f, __l); }
    template <class _Integer>
    _CCW_LIBCPP_HIDE_FROM_ABI void __assign_disp(_Integer __n, _Integer __v, __ccw_int_true_tag) {
        clear(); __fill_assign(static_cast<size_type>(__n), static_cast<_Tp>(__v));
    }
    template <class _InputIt>
    _CCW_LIBCPP_HIDE_FROM_ABI void __assign_disp(_InputIt __f, _InputIt __l, __ccw_int_false_tag) { clear(); __assign_range(__f, __l); }

    template <class _Sz, class _Vp>
    _CCW_LIBCPP_HIDE_FROM_ABI void __insert_disp(size_type __idx, _Sz __n, const _Vp& __v, __ccw_int_true_tag) {
        for (_Sz __i = 0; __i < __n; ++__i) insert(__begin_ + __idx + static_cast<size_type>(__i), static_cast<_Tp>(__v));
    }
    template <class _InputIt>
    _CCW_LIBCPP_HIDE_FROM_ABI void __insert_disp(size_type __idx, _InputIt __f, _InputIt __l, __ccw_int_false_tag) {
        size_type __off = 0;
        for (; __f != __l; ++__f, ++__off) insert(__begin_ + __idx + __off, *__f);
    }
};

template <class _Tp, class _Alloc>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator==(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) {
    if (__x.size() != __y.size()) return false;
    for (typename vector<_Tp, _Alloc>::size_type __i = 0; __i < __x.size(); ++__i)
        if (!(__x[__i] == __y[__i])) return false;
    return true;
}
template <class _Tp, class _Alloc>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator!=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) { return !(__x == __y); }
template <class _Tp, class _Alloc>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator<(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) {
    typedef typename vector<_Tp, _Alloc>::size_type _Sz;
    _Sz __n = __x.size() < __y.size() ? __x.size() : __y.size();
    for (_Sz __i = 0; __i < __n; ++__i) { if (__x[__i] < __y[__i]) return true; if (__y[__i] < __x[__i]) return false; }
    return __x.size() < __y.size();
}
template <class _Tp, class _Alloc>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator>(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) { return __y < __x; }
template <class _Tp, class _Alloc>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator<=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) { return !(__y < __x); }
template <class _Tp, class _Alloc>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator>=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) { return !(__x < __y); }
template <class _Tp, class _Alloc, class _Pred>
_CCW_LIBCPP_HIDE_FROM_ABI typename vector<_Tp, _Alloc>::size_type erase_if(vector<_Tp, _Alloc>& __c, _Pred __pred) {
    typename vector<_Tp, _Alloc>::iterator __w = __c.begin();
    for (typename vector<_Tp, _Alloc>::iterator __r = __c.begin(); __r != __c.end(); ++__r)
        if (!__pred(*__r)) { *__w = *__r; ++__w; }
    typename vector<_Tp, _Alloc>::size_type __n = static_cast<typename vector<_Tp, _Alloc>::size_type>(__c.end() - __w);
    __c.erase(__w, __c.end());
    return __n;
}
template <class _Tp, class _Alloc, class _Up>
_CCW_LIBCPP_HIDE_FROM_ABI typename vector<_Tp, _Alloc>::size_type erase(vector<_Tp, _Alloc>& __c, const _Up& __v) {
    typename vector<_Tp, _Alloc>::iterator __w = __c.begin();
    for (typename vector<_Tp, _Alloc>::iterator __r = __c.begin(); __r != __c.end(); ++__r)
        if (!(*__r == __v)) { *__w = *__r; ++__w; }
    typename vector<_Tp, _Alloc>::size_type __n = static_cast<typename vector<_Tp, _Alloc>::size_type>(__c.end() - __w);
    __c.erase(__w, __c.end());
    return __n;
}

template <class _Tp, class _Alloc>
struct uses_allocator<vector<_Tp, _Alloc>, _Alloc> { static const bool value = true; };

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___VECTOR_VECTOR_H
