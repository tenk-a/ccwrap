// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: _CCW_STD::deque for Open Watcom. libc++'s file layout is kept,
//  but the body is a self-contained classic implementation: a growable *circular
//  buffer* (single allocation, head index + size) giving amortized O(1) push/pop
//  at both ends and O(1) random access. This is simpler than libc++'s block-map
//  and sufficient for the container API; arbitrary middle insert/erase are not yet
//  provided (documented gap).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___DEQUE_DEQUE_H
#define _CCW_LIBCPP___DEQUE_DEQUE_H
#include "../__config"
#include <ranges>   // from_range_t
#include "../__memory/misc.h"   // uses_allocator
#include "../__fnctmpl.h"
#include "../__type_traits/integral_dispatch.h"
#include "../__utility/move.h"
#include "../__iterator/iterator_traits.h"
#include "../__iterator/reverse_iterator.h"
#include "../__memory/allocator.h"
#include <cstddef>
#include <stdexcept>
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp, class _Alloc> class deque;

template <class _Tp, class _DequeT, class _Ref, class _Ptr>
class __deque_iterator {
    _DequeT* __d_;
    _CCW_STD::size_t __i_;
public:
    typedef random_access_iterator_tag iterator_category;
    typedef _Tp                        value_type;
    typedef _CCW_STD::ptrdiff_t             difference_type;
    typedef _Ptr                       pointer;
    typedef _Ref                       reference;

    _CCW_LIBCPP_HIDE_FROM_ABI __deque_iterator() : __d_(0), __i_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI __deque_iterator(_DequeT* __d, _CCW_STD::size_t __i) : __d_(__d), __i_(__i) {}
    template <class _D2, class _R2, class _P2>
    _CCW_LIBCPP_HIDE_FROM_ABI __deque_iterator(const __deque_iterator<_Tp, _D2, _R2, _P2>& __o)
        : __d_(__o.__base()), __i_(__o.__index()) {}

    _CCW_LIBCPP_HIDE_FROM_ABI _DequeT*    __base()  const { return __d_; }
    _CCW_LIBCPP_HIDE_FROM_ABI _CCW_STD::size_t __index() const { return __i_; }

    _CCW_LIBCPP_HIDE_FROM_ABI reference operator*()  const { return (*__d_)[__i_]; }
    _CCW_LIBCPP_HIDE_FROM_ABI pointer   operator->() const { return &(*__d_)[__i_]; }
    _CCW_LIBCPP_HIDE_FROM_ABI reference operator[](difference_type __n) const { return (*__d_)[__i_ + __n]; }

    _CCW_LIBCPP_HIDE_FROM_ABI __deque_iterator& operator++()    { ++__i_; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI __deque_iterator  operator++(int) { __deque_iterator __t = *this; ++__i_; return __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI __deque_iterator& operator--()    { --__i_; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI __deque_iterator  operator--(int) { __deque_iterator __t = *this; --__i_; return __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI __deque_iterator& operator+=(difference_type __n) { __i_ += __n; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI __deque_iterator& operator-=(difference_type __n) { __i_ -= __n; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI __deque_iterator  operator+(difference_type __n) const { return __deque_iterator(__d_, __i_ + __n); }
    _CCW_LIBCPP_HIDE_FROM_ABI __deque_iterator  operator-(difference_type __n) const { return __deque_iterator(__d_, __i_ - __n); }
    _CCW_LIBCPP_HIDE_FROM_ABI difference_type   operator-(const __deque_iterator& __o) const { return static_cast<difference_type>(__i_) - static_cast<difference_type>(__o.__i_); }

    _CCW_LIBCPP_HIDE_FROM_ABI bool operator==(const __deque_iterator& __o) const { return __i_ == __o.__i_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator!=(const __deque_iterator& __o) const { return __i_ != __o.__i_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator<(const __deque_iterator& __o)  const { return __i_ <  __o.__i_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator>(const __deque_iterator& __o)  const { return __i_ >  __o.__i_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator<=(const __deque_iterator& __o) const { return __i_ <= __o.__i_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator>=(const __deque_iterator& __o) const { return __i_ >= __o.__i_; }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class _CCW_LIBCPP_TEMPLATE_VIS deque {
public:
    typedef _Tp                                                    value_type;
    typedef _Alloc                                                 allocator_type;
    typedef _CCW_STD::size_t                                            size_type;
    typedef _CCW_STD::ptrdiff_t                                         difference_type;
    typedef _Tp&                                                   reference;
    typedef const _Tp&                                             const_reference;
    typedef _Tp*                                                   pointer;
    typedef const _Tp*                                             const_pointer;
    typedef __deque_iterator<_Tp, deque, _Tp&, _Tp*>               iterator;
    typedef __deque_iterator<_Tp, const deque, const _Tp&, const _Tp*> const_iterator;
    typedef _CCW_STD::reverse_iterator<iterator>        reverse_iterator;
    typedef _CCW_STD::reverse_iterator<const_iterator>  const_reverse_iterator;

private:
    struct __cap_alloc : allocator_type {
        size_type __cap_;
        _CCW_LIBCPP_HIDE_FROM_ABI __cap_alloc() : __cap_(0) {}
        _CCW_LIBCPP_HIDE_FROM_ABI explicit __cap_alloc(const allocator_type& __a) : allocator_type(__a), __cap_(0) {}
    };
    __cap_alloc __ca_;
    pointer   __buf_;
    size_type __head_;
    size_type __size_;

    _CCW_LIBCPP_HIDE_FROM_ABI allocator_type&       __alloc()       { return __ca_; }
    _CCW_LIBCPP_HIDE_FROM_ABI const allocator_type& __alloc() const { return __ca_; }

    _CCW_LIBCPP_HIDE_FROM_ABI size_type __phys(size_type __i) const { return (__head_ + __i) % __ca_.__cap_; }
    _CCW_LIBCPP_HIDE_FROM_ABI void __grow(size_type __nc) {
        pointer __nb = __alloc().allocate(__nc);
        for (size_type __i = 0; __i < __size_; ++__i) {
            __alloc().construct(__nb + __i, __buf_[__phys(__i)]);
            __alloc().destroy(__buf_ + __phys(__i));
        }
        if (__buf_) __alloc().deallocate(__buf_, __ca_.__cap_);
        __buf_ = __nb; __ca_.__cap_ = __nc; __head_ = 0;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void __ensure_one() { if (__size_ == __ca_.__cap_) __grow(__ca_.__cap_ ? __ca_.__cap_ * 2 : 1); }

    _CCW_LIBCPP_HIDE_FROM_ABI iterator __insert_moved(size_type __idx, _CCW_RV_REF(_Tp) __v) {
        if (__idx == __size_) { push_back(__v); return begin() + static_cast<difference_type>(__idx); }
        _Tp __tail = back();            // a local: push_back may reallocate
        push_back(__tail);
        for (size_type __i = __size_ - 2; __i > __idx; --__i) (*this)[__i] = (*this)[__i - 1];
        (*this)[__idx] = __v;           // the one and only use of __v
        return begin() + static_cast<difference_type>(__idx);
    }

public:
    _CCW_LIBCPP_HIDE_FROM_ABI deque() : __buf_(0), __head_(0), __size_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI explicit deque(size_type __n, const _Tp& __v = _Tp())
        : __buf_(0), __head_(0), __size_(0) { for (; __n; --__n) push_back(__v); }
    template <class _InputIt>
    _CCW_LIBCPP_HIDE_FROM_ABI deque(_InputIt __f, _InputIt __l)
        : __buf_(0), __head_(0), __size_(0) {
        __ctor_disp(__f, __l, typename __ccw_dispatch_is_int<_InputIt>::_Tag());
    }
    _CCW_LIBCPP_HIDE_FROM_ABI explicit deque(const allocator_type& __a)
        : __ca_(__a), __buf_(0), __head_(0), __size_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI deque(size_type __n, const allocator_type& __a)
        : __ca_(__a), __buf_(0), __head_(0), __size_(0) { for (; __n; --__n) push_back(_Tp()); }
    _CCW_LIBCPP_HIDE_FROM_ABI deque(size_type __n, const _Tp& __v, const allocator_type& __a)
        : __ca_(__a), __buf_(0), __head_(0), __size_(0) { for (; __n; --__n) push_back(__v); }
    template <class _InputIt>
    _CCW_LIBCPP_HIDE_FROM_ABI deque(_InputIt __f, _InputIt __l, const allocator_type& __a)
        : __ca_(__a), __buf_(0), __head_(0), __size_(0) {
        __ctor_disp(__f, __l, typename __ccw_dispatch_is_int<_InputIt>::_Tag());
    }

    template <class _Range>
    _CCW_LIBCPP_HIDE_FROM_ABI deque(from_range_t, const _Range& __r)
        : __buf_(0), __head_(0), __size_(0) {
        for (typename _Range::const_iterator __it = __r.begin(); __it != __r.end(); ++__it)
            push_back(*__it);
    }
    template <class _Range>
    _CCW_LIBCPP_HIDE_FROM_ABI deque(from_range_t, const _Range& __r, const allocator_type& __a)
        : __ca_(__a), __buf_(0), __head_(0), __size_(0) {
        for (typename _Range::const_iterator __it = __r.begin(); __it != __r.end(); ++__it)
            push_back(*__it);
    }

    _CCW_LIBCPP_HIDE_FROM_ABI deque(const deque& __o)
        : __buf_(0), __head_(0), __size_(0) {
        for (size_type __i = 0; __i < __o.__size_; ++__i) push_back(__o[__i]);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI deque(const deque& __o, const allocator_type& __a)
        : __ca_(__a), __buf_(0), __head_(0), __size_(0) {
        for (size_type __i = 0; __i < __o.__size_; ++__i) push_back(__o[__i]);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI deque& operator=(const deque& __o) {
        if (this != &__o) { clear(); for (size_type __i = 0; __i < __o.__size_; ++__i) push_back(__o[__i]); }
        return *this;
    }

    template <class _Range>
    _CCW_LIBCPP_HIDE_FROM_ABI void assign_range(const _Range& __r) {
        clear();
        for (typename _Range::const_iterator __it = __r.begin(); __it != __r.end(); ++__it)
            push_back(*__it);
    }
    template <class _Range>
    _CCW_LIBCPP_HIDE_FROM_ABI void append_range(const _Range& __r) {
        for (typename _Range::const_iterator __it = __r.begin(); __it != __r.end(); ++__it)
            push_back(*__it);
    }
    template <class _Range>
    _CCW_LIBCPP_HIDE_FROM_ABI void prepend_range(const _Range& __r) {
        typename _Range::const_iterator __b = __r.begin(), __it = __r.end();
        while (__it != __b) { --__it; push_front(*__it); }
    }
    template <class _Range>
    _CCW_LIBCPP_HIDE_FROM_ABI iterator insert_range(iterator __p, const _Range& __r) {
        difference_type __at = __p - begin();
        difference_type __k  = __at;
        for (typename _Range::const_iterator __it = __r.begin(); __it != __r.end(); ++__it)
            insert(begin() + __k++, *__it);
        return begin() + __at;
    }

    _CCW_LIBCPP_HIDE_FROM_ABI ~deque() { clear(); if (__buf_) __alloc().deallocate(__buf_, __ca_.__cap_); }

#if _CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI deque(deque&& __o) _CCW_LIBCPP_NOEXCEPT
        : __ca_(__o.__alloc()), __buf_(__o.__buf_), __head_(__o.__head_), __size_(__o.__size_) {
        __ca_.__cap_ = __o.__ca_.__cap_;
        __o.__buf_ = 0; __o.__ca_.__cap_ = __o.__head_ = __o.__size_ = 0;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI deque& operator=(deque&& __o) _CCW_LIBCPP_NOEXCEPT {
        if (this != &__o) {
            clear(); if (__buf_) __alloc().deallocate(__buf_, __ca_.__cap_);
            __buf_ = __o.__buf_; __ca_.__cap_ = __o.__ca_.__cap_; __head_ = __o.__head_; __size_ = __o.__size_;
            __o.__buf_ = 0; __o.__ca_.__cap_ = __o.__head_ = __o.__size_ = 0;
        }
        return *this;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI deque(deque&& __o, const allocator_type& __a)
        : __ca_(__a), __buf_(__o.__buf_), __head_(__o.__head_), __size_(__o.__size_) {
        __ca_.__cap_ = __o.__ca_.__cap_;
        __o.__buf_ = 0; __o.__ca_.__cap_ = __o.__head_ = __o.__size_ = 0;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void push_back(_Tp&& __v) {
        __ensure_one(); __alloc().construct(__buf_ + __phys(__size_), static_cast<_Tp&&>(__v)); ++__size_;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void push_front(_Tp&& __v) {
        __ensure_one(); __head_ = (__head_ + __ca_.__cap_ - 1) % __ca_.__cap_;
        __alloc().construct(__buf_ + __head_, static_cast<_Tp&&>(__v)); ++__size_;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI iterator insert(iterator __pos, _Tp&& __v) {
        return __insert_moved(static_cast<size_type>(__pos - begin()), static_cast<_Tp&&>(__v));
    }
#else // !_CCW_HAS_REAL_RVALUE_REF
    _CCW_LIBCPP_HIDE_FROM_ABI deque(_CCW_RV_REF(deque) __r)
        : __buf_(0), __head_(0), __size_(0) {
        deque& __o = _CCW_MOVED_OBJ(__r);
        __alloc() = __o.__alloc();
        __buf_ = __o.__buf_; __ca_.__cap_ = __o.__ca_.__cap_; __head_ = __o.__head_; __size_ = __o.__size_;
        __o.__buf_ = 0; __o.__ca_.__cap_ = __o.__head_ = __o.__size_ = 0;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI deque(_CCW_RV_REF(deque) __r, const allocator_type& __a)
        : __ca_(__a), __buf_(0), __head_(0), __size_(0) {
        deque& __o = _CCW_MOVED_OBJ(__r);
        __buf_ = __o.__buf_; __ca_.__cap_ = __o.__ca_.__cap_; __head_ = __o.__head_; __size_ = __o.__size_;
        __o.__buf_ = 0; __o.__ca_.__cap_ = __o.__head_ = __o.__size_ = 0;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI deque& operator=(_CCW_RV_REF(deque) __r) {
        deque& __o = _CCW_MOVED_OBJ(__r);
        if (this != &__o) {
            clear(); if (__buf_) __alloc().deallocate(__buf_, __ca_.__cap_);
            __alloc() = __o.__alloc();
            __buf_ = __o.__buf_; __ca_.__cap_ = __o.__ca_.__cap_; __head_ = __o.__head_; __size_ = __o.__size_;
            __o.__buf_ = 0; __o.__ca_.__cap_ = __o.__head_ = __o.__size_ = 0;
        }
        return *this;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void push_back(_CCW_RV_REF(_Tp) __v) {
        __ensure_one(); __alloc().construct(__buf_ + __phys(__size_), __v); ++__size_;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void push_front(_CCW_RV_REF(_Tp) __v) {
        __ensure_one(); __head_ = (__head_ + __ca_.__cap_ - 1) % __ca_.__cap_;
        __alloc().construct(__buf_ + __head_, __v); ++__size_;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI iterator insert(iterator __pos, _CCW_RV_REF(_Tp) __v) {
        return __insert_moved(static_cast<size_type>(__pos - begin()), __v);
    }
#endif

    _CCW_LIBCPP_HIDE_FROM_ABI size_type size() const  { return __size_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool      empty() const { return __size_ == 0; }
    _CCW_LIBCPP_HIDE_FROM_ABI size_type max_size() const { return __alloc().max_size(); }

    _CCW_LIBCPP_HIDE_FROM_ABI reference       operator[](size_type __i)       { return __buf_[__phys(__i)]; }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reference operator[](size_type __i) const { return __buf_[__phys(__i)]; }
    _CCW_LIBCPP_HIDE_FROM_ABI reference       at(size_type __i)       { if (__i >= __size_) _CCW_THROW(_CCW_STD::out_of_range("deque::at")); return (*this)[__i]; }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reference at(size_type __i) const { if (__i >= __size_) _CCW_THROW(_CCW_STD::out_of_range("deque::at")); return (*this)[__i]; }
    _CCW_LIBCPP_HIDE_FROM_ABI reference       front()       { return (*this)[0]; }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reference front() const { return (*this)[0]; }
    _CCW_LIBCPP_HIDE_FROM_ABI reference       back()        { return (*this)[__size_ - 1]; }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reference back() const  { return (*this)[__size_ - 1]; }

    _CCW_LIBCPP_HIDE_FROM_ABI iterator       begin()       { return iterator(this, 0); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_iterator begin() const { return const_iterator(this, 0); }
    _CCW_LIBCPP_HIDE_FROM_ABI iterator       end()         { return iterator(this, __size_); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_iterator end() const   { return const_iterator(this, __size_); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_iterator cbegin() const { return const_iterator(this, 0); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_iterator cend() const   { return const_iterator(this, __size_); }
    _CCW_LIBCPP_HIDE_FROM_ABI reverse_iterator       rbegin()       { return reverse_iterator(end()); }
    _CCW_LIBCPP_HIDE_FROM_ABI reverse_iterator       rend()         { return reverse_iterator(begin()); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reverse_iterator crend()   const { return const_reverse_iterator(begin()); }

    _CCW_LIBCPP_HIDE_FROM_ABI void push_back(const _Tp& __v) {
        __ensure_one(); __alloc().construct(__buf_ + __phys(__size_), __v); ++__size_;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void push_front(const _Tp& __v) {
        __ensure_one(); __head_ = (__head_ + __ca_.__cap_ - 1) % __ca_.__cap_;
        __alloc().construct(__buf_ + __head_, __v); ++__size_;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void pop_back()  { --__size_; __alloc().destroy(__buf_ + __phys(__size_)); }
    _CCW_LIBCPP_HIDE_FROM_ABI void pop_front() { __alloc().destroy(__buf_ + __head_); __head_ = (__head_ + 1) % __ca_.__cap_; --__size_; }
    _CCW_LIBCPP_HIDE_FROM_ABI void clear()     { while (__size_) pop_back(); }
    _CCW_LIBCPP_HIDE_FROM_ABI void resize(size_type __n, const _Tp& __v = _Tp()) {
        while (__size_ > __n) pop_back();
        while (__size_ < __n) push_back(__v);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void assign(size_type __n, const _Tp& __v) { clear(); for (; __n; --__n) push_back(__v); }
    template <class _InputIt>
    _CCW_LIBCPP_HIDE_FROM_ABI void assign(_InputIt __f, _InputIt __l) {
        __assign_disp(__f, __l, typename __ccw_dispatch_is_int<_InputIt>::_Tag());
    }

#if _CCW_NO_VARIADIC_TEMPLATES
    _CCW_LIBCPP_HIDE_FROM_ABI reference emplace_front() { push_front(value_type()); return front(); }
    _CCW_LIBCPP_HIDE_FROM_ABI reference emplace_back()  { push_back(value_type());  return back();  }
#  define _CCW_GEN_DEQ_EMPLACE(N)                                                                     \
    template <_CCW_FNCTMPL_TMPLARG(N)>                                                                \
    _CCW_LIBCPP_HIDE_FROM_ABI reference emplace_front(_CCW_FNCTMPL_DECLARG(N)) { push_front(value_type(_CCW_FNCTMPL_FNCARG(N))); return front(); } \
    template <_CCW_FNCTMPL_TMPLARG(N)>                                                                \
    _CCW_LIBCPP_HIDE_FROM_ABI reference emplace_back(_CCW_FNCTMPL_DECLARG(N))  { push_back(value_type(_CCW_FNCTMPL_FNCARG(N)));  return back();  }
    _CCW_GEN_DEQ_EMPLACE(1) _CCW_GEN_DEQ_EMPLACE(2) _CCW_GEN_DEQ_EMPLACE(3) _CCW_GEN_DEQ_EMPLACE(4)
#  undef _CCW_GEN_DEQ_EMPLACE
#else
    template <class... _Args>
    _CCW_LIBCPP_HIDE_FROM_ABI reference emplace_front(_Args&&... __a) { push_front(value_type(static_cast<_Args&&>(__a)...)); return front(); }
    template <class... _Args>
    _CCW_LIBCPP_HIDE_FROM_ABI reference emplace_back(_Args&&... __a)  { push_back(value_type(static_cast<_Args&&>(__a)...));  return back();  }
#endif

#if _CCW_NO_VARIADIC_TEMPLATES
    _CCW_LIBCPP_HIDE_FROM_ABI iterator emplace(iterator __pos) { return insert(__pos, value_type()); }
#  define _CCW_GEN_DEQ_EMPLACE_AT(N)                                                                          \
    template <_CCW_FNCTMPL_TMPLARG(N)>                                                                        \
    _CCW_LIBCPP_HIDE_FROM_ABI iterator emplace(iterator __pos _CCW_FNCTMPL_CM_DECLARG(N)) { return insert(__pos, value_type(_CCW_FNCTMPL_FNCARG(N))); }
    _CCW_GEN_DEQ_EMPLACE_AT(1) _CCW_GEN_DEQ_EMPLACE_AT(2) _CCW_GEN_DEQ_EMPLACE_AT(3) _CCW_GEN_DEQ_EMPLACE_AT(4)
#  undef _CCW_GEN_DEQ_EMPLACE_AT
#else
    template <class... _Args>
    _CCW_LIBCPP_HIDE_FROM_ABI iterator emplace(iterator __pos, _Args&&... __a) { return insert(__pos, value_type(static_cast<_Args&&>(__a)...)); }
#endif

    _CCW_LIBCPP_HIDE_FROM_ABI allocator_type get_allocator() const { return __alloc(); }

    _CCW_LIBCPP_HIDE_FROM_ABI iterator insert(iterator __pos, const _Tp& __v) {
        size_type __idx = static_cast<size_type>(__pos - begin());
        push_back(__v);
        for (size_type __i = __size_ - 1; __i > __idx; --__i) (*this)[__i] = (*this)[__i - 1];
        (*this)[__idx] = __v;
        return begin() + static_cast<difference_type>(__idx);
    }
    template <class _A1, class _A2>
    _CCW_LIBCPP_HIDE_FROM_ABI iterator insert(iterator __pos, _A1 __a1, _A2 __a2) {
        return __insert_disp(__pos, __a1, __a2, typename __ccw_dispatch_is_int<_A1>::_Tag());
    }
    _CCW_LIBCPP_HIDE_FROM_ABI iterator erase(iterator __pos) {
        size_type __idx = static_cast<size_type>(__pos - begin());
        for (size_type __i = __idx; __i + 1 < __size_; ++__i) (*this)[__i] = (*this)[__i + 1];
        pop_back();
        return begin() + static_cast<difference_type>(__idx);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI iterator erase(iterator __f, iterator __l) {
        size_type __idx = static_cast<size_type>(__f - begin());
        size_type __n = static_cast<size_type>(__l - __f);
        for (size_type __i = __idx; __i + __n < __size_; ++__i) (*this)[__i] = (*this)[__i + __n];
        for (size_type __k = 0; __k < __n; ++__k) pop_back();
        return begin() + static_cast<difference_type>(__idx);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void shrink_to_fit() { if (__ca_.__cap_ > __size_) __grow(__size_ ? __size_ : 1); }
    _CCW_LIBCPP_HIDE_FROM_ABI void swap(deque& __o) {
        pointer __b = __buf_; __buf_ = __o.__buf_; __o.__buf_ = __b;
        size_type __h = __head_; __head_ = __o.__head_; __o.__head_ = __h;
        size_type __s = __size_; __size_ = __o.__size_; __o.__size_ = __s;
        __cap_alloc __t = __ca_; __ca_ = __o.__ca_; __o.__ca_ = __t;   // cap + allocator together
    }

private:
    template <class _InputIt>
    _CCW_LIBCPP_HIDE_FROM_ABI void __assign_range(_InputIt __f, _InputIt __l) { for (; __f != __l; ++__f) push_back(*__f); }
    template <class _Integer>
    _CCW_LIBCPP_HIDE_FROM_ABI void __ctor_disp(_Integer __n, _Integer __v, __ccw_int_true_tag) { for (size_type __c = static_cast<size_type>(__n); __c; --__c) push_back(static_cast<_Tp>(__v)); }
    template <class _InputIt>
    _CCW_LIBCPP_HIDE_FROM_ABI void __ctor_disp(_InputIt __f, _InputIt __l, __ccw_int_false_tag) { __assign_range(__f, __l); }
    template <class _Integer>
    _CCW_LIBCPP_HIDE_FROM_ABI void __assign_disp(_Integer __n, _Integer __v, __ccw_int_true_tag) { assign(static_cast<size_type>(__n), static_cast<_Tp>(__v)); }
    template <class _InputIt>
    _CCW_LIBCPP_HIDE_FROM_ABI void __assign_disp(_InputIt __f, _InputIt __l, __ccw_int_false_tag) { clear(); __assign_range(__f, __l); }

    template <class _NN, class _VV>
    _CCW_LIBCPP_HIDE_FROM_ABI iterator __insert_disp(iterator __pos, _NN __n, _VV __v, __ccw_int_true_tag) {
        size_type __idx = static_cast<size_type>(__pos - begin()), __c = static_cast<size_type>(__n);
        const _Tp __val(__v);
        for (size_type __k = 0; __k < __c; ++__k) push_back(__val);
        for (size_type __i = __size_; __i-- > __idx + __c; ) (*this)[__i] = (*this)[__i - __c];
        for (size_type __k = 0; __k < __c; ++__k) (*this)[__idx + __k] = __val;
        return begin() + static_cast<difference_type>(__idx);
    }
    template <class _InputIt>
    _CCW_LIBCPP_HIDE_FROM_ABI iterator __insert_disp(iterator __pos, _InputIt __f, _InputIt __l, __ccw_int_false_tag) {
        size_type __idx = static_cast<size_type>(__pos - begin());
        size_type __n = 0; for (_InputIt __t = __f; __t != __l; ++__t) ++__n;
        for (size_type __k = 0; __k < __n; ++__k) push_back(_Tp());
        for (size_type __i = __size_; __i-- > __idx + __n; ) (*this)[__i] = (*this)[__i - __n];
        for (size_type __k = 0; __f != __l; ++__f, ++__k) (*this)[__idx + __k] = *__f;
        return begin() + static_cast<difference_type>(__idx);
    }
};

template <class _Tp, class _Alloc>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator==(const deque<_Tp, _Alloc>& __x, const deque<_Tp, _Alloc>& __y) {
    if (__x.size() != __y.size()) return false;
    for (typename deque<_Tp, _Alloc>::size_type __i = 0; __i < __x.size(); ++__i)
        if (!(__x[__i] == __y[__i])) return false;
    return true;
}
template <class _Tp, class _Alloc>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator!=(const deque<_Tp, _Alloc>& __x, const deque<_Tp, _Alloc>& __y) { return !(__x == __y); }
template <class _Tp, class _Alloc>
_CCW_LIBCPP_HIDE_FROM_ABI bool operator<(const deque<_Tp, _Alloc>& __x, const deque<_Tp, _Alloc>& __y) {
    typename deque<_Tp, _Alloc>::size_type __n = __x.size() < __y.size() ? __x.size() : __y.size();
    for (typename deque<_Tp, _Alloc>::size_type __i = 0; __i < __n; ++__i) { if (__x[__i] < __y[__i]) return true; if (__y[__i] < __x[__i]) return false; }
    return __x.size() < __y.size();
}
template <class _Tp, class _Alloc> _CCW_LIBCPP_HIDE_FROM_ABI bool operator>(const deque<_Tp, _Alloc>& __x, const deque<_Tp, _Alloc>& __y)  { return __y < __x; }
template <class _Tp, class _Alloc> _CCW_LIBCPP_HIDE_FROM_ABI bool operator<=(const deque<_Tp, _Alloc>& __x, const deque<_Tp, _Alloc>& __y) { return !(__y < __x); }
template <class _Tp, class _Alloc> _CCW_LIBCPP_HIDE_FROM_ABI bool operator>=(const deque<_Tp, _Alloc>& __x, const deque<_Tp, _Alloc>& __y) { return !(__x < __y); }
template <class _Tp, class _Alloc, class _Up>
_CCW_LIBCPP_HIDE_FROM_ABI typename deque<_Tp, _Alloc>::size_type erase(deque<_Tp, _Alloc>& __c, const _Up& __v) {
    typename deque<_Tp, _Alloc>::size_type __w = 0, __n = __c.size();
    for (typename deque<_Tp, _Alloc>::size_type __r = 0; __r < __n; ++__r) if (!(__c[__r] == __v)) { __c[__w] = __c[__r]; ++__w; }
    typename deque<_Tp, _Alloc>::size_type __removed = __n - __w;
    while (__c.size() > __w) __c.pop_back();
    return __removed;
}
template <class _Tp, class _Alloc, class _Pred>
_CCW_LIBCPP_HIDE_FROM_ABI typename deque<_Tp, _Alloc>::size_type erase_if(deque<_Tp, _Alloc>& __c, _Pred __pred) {
    typename deque<_Tp, _Alloc>::size_type __w = 0, __n = __c.size();
    for (typename deque<_Tp, _Alloc>::size_type __r = 0; __r < __n; ++__r) if (!__pred(__c[__r])) { __c[__w] = __c[__r]; ++__w; }
    typename deque<_Tp, _Alloc>::size_type __removed = __n - __w;
    while (__c.size() > __w) __c.pop_back();
    return __removed;
}

template <class _Tp, class _Alloc>
struct uses_allocator<deque<_Tp, _Alloc>, _Alloc> { static const bool value = true; };

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___DEQUE_DEQUE_H
