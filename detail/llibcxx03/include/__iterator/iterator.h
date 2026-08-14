//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ITERATOR_ITERATOR_H
#define _CCW_LIBCPP___ITERATOR_ITERATOR_H
#include "../__config"
#include <cstddef>
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _Category, class _Tp, class _Distance = _CCW_STD::ptrdiff_t,
          class _Pointer = _Tp*, class _Reference = _Tp&>
struct _CCW_LIBCPP_TEMPLATE_VIS iterator {
    typedef _Tp        value_type;
    typedef _Distance  difference_type;
    typedef _Pointer   pointer;
    typedef _Reference reference;
    typedef _Category  iterator_category;
};
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
