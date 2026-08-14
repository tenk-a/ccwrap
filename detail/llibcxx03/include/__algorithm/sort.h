//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_SORT_H
#define _CCW_LIBCPP___ALGORITHM_SORT_H
#include "../__config"
#include "../__utility/swap.h"
#include "../__iterator/iterator_traits.h"
#include "../__functional/operations.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _RandIt, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI _RandIt __sort_partition(_RandIt __lo, _RandIt __hi, _Comp __comp) {
  typename iterator_traits<_RandIt>::value_type __pivot = *(__lo + (__hi - __lo - 1) / 2);
  _RandIt __i = __lo;
  _RandIt __j = __hi;
  --__i; // one before lo
  for (;;) {
    do { ++__i; } while (__comp(*__i, __pivot));
    do { --__j; } while (__comp(__pivot, *__j));
    if (!(__i < __j)) return __j;
    _CCW_STD::swap(*__i, *__j);
  }
}

template <class _RandIt, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI void __sort_impl(_RandIt __lo, _RandIt __hi, _Comp __comp) {
  while (__hi - __lo > 1) {
    _RandIt __p = _CCW_STD::__sort_partition(__lo, __hi, __comp);
    _RandIt __mid = __p; ++__mid;
    if (__mid - __lo < __hi - __mid) { _CCW_STD::__sort_impl(__lo, __mid, __comp); __lo = __mid; }
    else                             { _CCW_STD::__sort_impl(__mid, __hi, __comp); __hi = __mid; }
  }
}

template <class _RandIt, class _Comp>
_CCW_LIBCPP_HIDE_FROM_ABI void sort(_RandIt __first, _RandIt __last, _Comp __comp) { _CCW_STD::__sort_impl(__first, __last, __comp); }

template <class _RandIt>
_CCW_LIBCPP_HIDE_FROM_ABI void sort(_RandIt __first, _RandIt __last) {
  _CCW_STD::__sort_impl(__first, __last, less<typename iterator_traits<_RandIt>::value_type>());
}

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
