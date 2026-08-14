//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ITERATOR_ADVANCE_H
#define _CCW_LIBCPP___ITERATOR_ADVANCE_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
#if !_CCW_LIBCPP_NATIVE_STD

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _InputIter, class _Distance>
_CCW_LIBCPP_HIDE_FROM_ABI void __advance(_InputIter& __i, _Distance __n, input_iterator_tag)
{ for (; __n > 0; --__n) ++__i; }

template <class _BiDirIter, class _Distance>
_CCW_LIBCPP_HIDE_FROM_ABI void __advance(_BiDirIter& __i, _Distance __n, bidirectional_iterator_tag)
{ if (__n >= 0) for (; __n > 0; --__n) ++__i; else for (; __n < 0; ++__n) --__i; }

template <class _RandIter, class _Distance>
_CCW_LIBCPP_HIDE_FROM_ABI void __advance(_RandIter& __i, _Distance __n, random_access_iterator_tag)
{ __i += __n; }

template <class _InputIter, class _Distance>
_CCW_LIBCPP_HIDE_FROM_ABI void advance(_InputIter& __i, _Distance __n)
{ _CCW_STD::__advance(__i, __n, typename iterator_traits<_InputIter>::iterator_category()); }

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_NATIVE_STD
#endif
