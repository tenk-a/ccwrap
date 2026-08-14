//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ITERATOR_ISTREAM_ITERATOR_H
#define _CCW_LIBCPP___ITERATOR_ISTREAM_ITERATOR_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
#include <istream>
#include <cstddef>
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp>
class istream_iterator {
    istream* __is_;
    _Tp      __value_;
    bool     __end_;
    _CCW_LIBCPP_HIDE_FROM_ABI void __read() { if (__is_ && !(*__is_ >> __value_)) { __is_ = 0; __end_ = true; } }
public:
    typedef input_iterator_tag iterator_category;
    typedef _Tp                value_type;
    typedef _CCW_STD::ptrdiff_t     difference_type;
    typedef const _Tp*         pointer;
    typedef const _Tp&         reference;

    _CCW_LIBCPP_HIDE_FROM_ABI istream_iterator() : __is_(0), __value_(), __end_(true) {}
    _CCW_LIBCPP_HIDE_FROM_ABI istream_iterator(istream& __is) : __is_(&__is), __value_(), __end_(false) { __read(); }
    _CCW_LIBCPP_HIDE_FROM_ABI const _Tp& operator*()  const { return __value_; }
    _CCW_LIBCPP_HIDE_FROM_ABI const _Tp* operator->() const { return &__value_; }
    _CCW_LIBCPP_HIDE_FROM_ABI istream_iterator& operator++()    { __read(); return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI istream_iterator  operator++(int) { istream_iterator __t = *this; __read(); return __t; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator==(const istream_iterator& __o) const { return __end_ == __o.__end_; }
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator!=(const istream_iterator& __o) const { return !(*this == __o); }
};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
