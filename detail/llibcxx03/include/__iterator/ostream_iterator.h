//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ITERATOR_OSTREAM_ITERATOR_H
#define _CCW_LIBCPP___ITERATOR_OSTREAM_ITERATOR_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
#include <ostream>
#include <cstddef>
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp>
class ostream_iterator {
    ostream*    __os_;
    const char* __delim_;
public:
    typedef output_iterator_tag iterator_category;
    typedef void                value_type;
    typedef _CCW_STD::ptrdiff_t      difference_type;
    typedef void                pointer;
    typedef void                reference;
    typedef char                        char_type;
    typedef _CCW_STD::char_traits<char> traits_type;
    typedef _CCW_STD::ostream           ostream_type;

    _CCW_LIBCPP_HIDE_FROM_ABI ostream_iterator(ostream& __os) : __os_(&__os), __delim_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI ostream_iterator(ostream& __os, const char* __d) : __os_(&__os), __delim_(__d) {}
    _CCW_LIBCPP_HIDE_FROM_ABI ostream_iterator& operator=(const _Tp& __v) { *__os_ << __v; if (__delim_) *__os_ << __delim_; return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI ostream_iterator& operator*()     { return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI ostream_iterator& operator++()    { return *this; }
    _CCW_LIBCPP_HIDE_FROM_ABI ostream_iterator& operator++(int) { return *this; }
};

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
