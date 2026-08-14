//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: the _CCW_STD::vector<bool> specialisation for Open Watcom. Bits are
//  packed into an array of words (space optimisation the standard mandates), so
//  operator[] hands back a PROXY reference rather than a real bool&. Provides the
//  proxy's operator bool / operator= / flip, the container's flip(), and a bit
//  iterator so range-for and the algorithms work.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___VECTOR_VECTOR_BOOL_H
#define _CCW_LIBCPP___VECTOR_VECTOR_BOOL_H
#include "../__config"
#include "../__memory/allocator.h"
#include <cstddef>
#include <climits>
#include <stdexcept>
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Alloc>
class vector<bool, _Alloc> {
public:
    typedef bool                          value_type;
    typedef _Alloc                        allocator_type;
    typedef _CCW_STD::size_t                   size_type;
    typedef _CCW_STD::ptrdiff_t                difference_type;
    typedef bool                          const_reference;
    typedef unsigned long                 __storage_type;   // one word of bits
    typedef typename _Alloc::template rebind<__storage_type>::other __storage_alloc;

    static const unsigned __bpw = (unsigned)(sizeof(__storage_type) * CHAR_BIT);

    class reference {
        __storage_type* __w_;
        __storage_type  __m_;
    public:
        _CCW_LIBCPP_HIDE_FROM_ABI reference(__storage_type* __w, __storage_type __m) : __w_(__w), __m_(__m) {}
        _CCW_LIBCPP_HIDE_FROM_ABI operator bool() const { return (*__w_ & __m_) != 0; }
        _CCW_LIBCPP_HIDE_FROM_ABI bool operator~() const { return (*__w_ & __m_) == 0; }
        _CCW_LIBCPP_HIDE_FROM_ABI reference& operator=(bool __b) {
            if (__b) *__w_ |= __m_; else *__w_ &= ~__m_;
            return *this;
        }
        _CCW_LIBCPP_HIDE_FROM_ABI reference& operator=(const reference& __r) { return *this = bool(__r); }
        _CCW_LIBCPP_HIDE_FROM_ABI void flip() { *__w_ ^= __m_; }
    };

    class __bit_iterator {
        __storage_type* __w_;    // current word
        unsigned        __b_;    // bit index within the word [0, __bpw)
        friend class vector;
        _CCW_LIBCPP_HIDE_FROM_ABI __bit_iterator(__storage_type* __w, unsigned __b) : __w_(__w), __b_(__b) {}
    public:
        typedef _CCW_STD::ptrdiff_t         difference_type;
        typedef bool                   value_type;
        typedef typename vector::reference reference;
        typedef void                   pointer;
        _CCW_LIBCPP_HIDE_FROM_ABI __bit_iterator() : __w_(0), __b_(0) {}
        _CCW_LIBCPP_HIDE_FROM_ABI reference operator*() const { return reference(__w_, __storage_type(1) << __b_); }
        _CCW_LIBCPP_HIDE_FROM_ABI __bit_iterator& operator++() { if (++__b_ == __bpw) { __b_ = 0; ++__w_; } return *this; }
        _CCW_LIBCPP_HIDE_FROM_ABI __bit_iterator  operator++(int) { __bit_iterator __t = *this; ++*this; return __t; }
        _CCW_LIBCPP_HIDE_FROM_ABI __bit_iterator& operator--() { if (__b_ == 0) { __b_ = __bpw - 1; --__w_; } else --__b_; return *this; }
        _CCW_LIBCPP_HIDE_FROM_ABI __bit_iterator  operator--(int) { __bit_iterator __t = *this; --*this; return __t; }
        _CCW_LIBCPP_HIDE_FROM_ABI __bit_iterator& operator+=(difference_type __n) {
            difference_type __p = (difference_type)__b_ + __n;
            __w_ += __p / (difference_type)__bpw; __p %= (difference_type)__bpw;
            if (__p < 0) { __p += __bpw; --__w_; }
            __b_ = (unsigned)__p; return *this;
        }
        _CCW_LIBCPP_HIDE_FROM_ABI __bit_iterator& operator-=(difference_type __n) { return *this += -__n; }
        _CCW_LIBCPP_HIDE_FROM_ABI __bit_iterator operator+(difference_type __n) const { __bit_iterator __t = *this; __t += __n; return __t; }
        _CCW_LIBCPP_HIDE_FROM_ABI __bit_iterator operator-(difference_type __n) const { __bit_iterator __t = *this; __t -= __n; return __t; }
        _CCW_LIBCPP_HIDE_FROM_ABI difference_type operator-(const __bit_iterator& __o) const {
            return (difference_type)(__w_ - __o.__w_) * (difference_type)__bpw + (difference_type)__b_ - (difference_type)__o.__b_;
        }
        _CCW_LIBCPP_HIDE_FROM_ABI reference operator[](difference_type __n) const { return *(*this + __n); }
        _CCW_LIBCPP_HIDE_FROM_ABI bool operator==(const __bit_iterator& __o) const { return __w_ == __o.__w_ && __b_ == __o.__b_; }
        _CCW_LIBCPP_HIDE_FROM_ABI bool operator!=(const __bit_iterator& __o) const { return !(*this == __o); }
        _CCW_LIBCPP_HIDE_FROM_ABI bool operator<(const __bit_iterator& __o) const { return __w_ < __o.__w_ || (__w_ == __o.__w_ && __b_ < __o.__b_); }
    };
    typedef __bit_iterator iterator;
    typedef __bit_iterator const_iterator;   // simplification: bits are read through the same proxy

private:
    struct __cap_alloc : __storage_alloc {
        size_type __cap_;             // capacity in WORDS
        _CCW_LIBCPP_HIDE_FROM_ABI __cap_alloc() : __cap_(0) {}
        _CCW_LIBCPP_HIDE_FROM_ABI explicit __cap_alloc(const __storage_alloc& __a)
            : __storage_alloc(__a), __cap_(0) {}
    };
    __cap_alloc     __ca_;
    __storage_type* __begin_;
    size_type       __size_;      // number of bits

    _CCW_LIBCPP_HIDE_FROM_ABI __storage_alloc&       __sa()        { return __ca_; }
    _CCW_LIBCPP_HIDE_FROM_ABI const __storage_alloc& __sa() const  { return __ca_; }
    _CCW_LIBCPP_HIDE_FROM_ABI size_type&             __cap()       { return __ca_.__cap_; }
    _CCW_LIBCPP_HIDE_FROM_ABI size_type              __cap() const { return __ca_.__cap_; }

    _CCW_LIBCPP_HIDE_FROM_ABI static size_type __words_for(size_type __bits) { return (__bits + __bpw - 1) / __bpw; }
    _CCW_LIBCPP_HIDE_FROM_ABI void __grow(size_type __need_bits) {
        size_type __nw = __words_for(__need_bits);
        if (__nw <= __cap()) return;
        size_type __gw = __cap() ? __cap() * 2 : 1;
        if (__gw < __nw) __gw = __nw;
        __storage_type* __nb = __sa().allocate(__gw);
        for (size_type __i = 0; __i < __words_for(__size_); ++__i) __nb[__i] = __begin_[__i];
        if (__begin_) __sa().deallocate(__begin_, __cap());
        __begin_ = __nb; __cap() = __gw;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void __set(size_type __i, bool __b) {
        __storage_type __m = __storage_type(1) << (__i % __bpw);
        if (__b) __begin_[__i / __bpw] |= __m; else __begin_[__i / __bpw] &= ~__m;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI bool __get(size_type __i) const {
        return (__begin_[__i / __bpw] & (__storage_type(1) << (__i % __bpw))) != 0;
    }

public:
    _CCW_LIBCPP_HIDE_FROM_ABI vector() : __begin_(0), __size_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI explicit vector(const allocator_type& __a)
        : __ca_(__storage_alloc(__a)), __begin_(0), __size_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI explicit vector(size_type __n, bool __v = false) : __begin_(0), __size_(0) {
        if (__n) { __grow(__n); for (size_type __i = 0; __i < __n; ++__i) __set(__i, __v); __size_ = __n; }
    }
    _CCW_LIBCPP_HIDE_FROM_ABI vector(const vector& __o) : __ca_(__storage_alloc(__o.__sa())), __begin_(0), __size_(0) {
        if (__o.__size_) { __grow(__o.__size_); size_type __nw = __words_for(__o.__size_);
            for (size_type __i = 0; __i < __nw; ++__i) __begin_[__i] = __o.__begin_[__i]; __size_ = __o.__size_; }
    }
    template <class _InputIt>
    _CCW_LIBCPP_HIDE_FROM_ABI vector(_InputIt __f, _InputIt __l) : __begin_(0), __size_(0) {
        for (; __f != __l; ++__f) push_back(*__f);
    }
    _CCW_LIBCPP_HIDE_FROM_ABI ~vector() { if (__begin_) __sa().deallocate(__begin_, __cap()); }

    _CCW_LIBCPP_HIDE_FROM_ABI vector& operator=(const vector& __o) {
        if (this != &__o) { clear(); if (__o.__size_) { __grow(__o.__size_);
            size_type __nw = __words_for(__o.__size_);
            for (size_type __i = 0; __i < __nw; ++__i) __begin_[__i] = __o.__begin_[__i]; __size_ = __o.__size_; } }
        return *this;
    }

    _CCW_LIBCPP_HIDE_FROM_ABI size_type size() const     { return __size_; }
    _CCW_LIBCPP_HIDE_FROM_ABI size_type capacity() const { return __cap() * __bpw; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool      empty() const    { return __size_ == 0; }
    _CCW_LIBCPP_HIDE_FROM_ABI size_type max_size() const { return (size_type)-1; }
    _CCW_LIBCPP_HIDE_FROM_ABI allocator_type get_allocator() const { return allocator_type(__sa()); }

    _CCW_LIBCPP_HIDE_FROM_ABI reference       operator[](size_type __i)       { return reference(__begin_ + __i / __bpw, __storage_type(1) << (__i % __bpw)); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reference operator[](size_type __i) const { return __get(__i); }
    _CCW_LIBCPP_HIDE_FROM_ABI reference       at(size_type __i)       { if (__i >= __size_) _CCW_THROW(_CCW_STD::out_of_range("vector<bool>::at")); return (*this)[__i]; }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reference at(size_type __i) const { if (__i >= __size_) _CCW_THROW(_CCW_STD::out_of_range("vector<bool>::at")); return __get(__i); }
    _CCW_LIBCPP_HIDE_FROM_ABI reference       front()       { return (*this)[0]; }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reference front() const { return __get(0); }
    _CCW_LIBCPP_HIDE_FROM_ABI reference       back()        { return (*this)[__size_ - 1]; }
    _CCW_LIBCPP_HIDE_FROM_ABI const_reference back() const  { return __get(__size_ - 1); }

    _CCW_LIBCPP_HIDE_FROM_ABI iterator begin() { return iterator(__begin_, 0); }
    _CCW_LIBCPP_HIDE_FROM_ABI iterator end()   { return iterator(__begin_ + __size_ / __bpw, (unsigned)(__size_ % __bpw)); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_iterator begin() const { return const_iterator(__begin_, 0); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_iterator end()   const { return const_iterator(__begin_ + __size_ / __bpw, (unsigned)(__size_ % __bpw)); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_iterator cbegin() const { return begin(); }
    _CCW_LIBCPP_HIDE_FROM_ABI const_iterator cend()   const { return end(); }

    _CCW_LIBCPP_HIDE_FROM_ABI void reserve(size_type __n) { __grow(__n); }
    _CCW_LIBCPP_HIDE_FROM_ABI void push_back(bool __v) { __grow(__size_ + 1); __set(__size_, __v); ++__size_; }
    _CCW_LIBCPP_HIDE_FROM_ABI void pop_back() { --__size_; }
    _CCW_LIBCPP_HIDE_FROM_ABI void clear() { __size_ = 0; }
    _CCW_LIBCPP_HIDE_FROM_ABI void resize(size_type __n, bool __v = false) {
        if (__n > __size_) { __grow(__n); for (size_type __i = __size_; __i < __n; ++__i) __set(__i, __v); }
        __size_ = __n;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI void assign(size_type __n, bool __v) { clear(); if (__n) { __grow(__n); for (size_type __i = 0; __i < __n; ++__i) __set(__i, __v); __size_ = __n; } }

    _CCW_LIBCPP_HIDE_FROM_ABI void swap(vector& __o) {
        __storage_type* __tb = __begin_; __begin_ = __o.__begin_; __o.__begin_ = __tb;
        size_type __ts = __size_; __size_ = __o.__size_; __o.__size_ = __ts;
        __cap_alloc __t = __ca_; __ca_ = __o.__ca_; __o.__ca_ = __t;
    }

    _CCW_LIBCPP_HIDE_FROM_ABI void flip() {
        size_type __nw = __words_for(__size_);
        for (size_type __i = 0; __i < __nw; ++__i) __begin_[__i] = ~__begin_[__i];
    }

    _CCW_LIBCPP_HIDE_FROM_ABI static void swap(reference __x, reference __y) { bool __t = __x; __x = __y; __y = __t; }
};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___VECTOR_VECTOR_BOOL_H
