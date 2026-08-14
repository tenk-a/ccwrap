/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ITERATOR_RANGES_ITER_OPS_H
#define _CCW_LIBCPP___ITERATOR_RANGES_ITER_OPS_H
#include "../__config"
#if _CCW_LIBCPP_NATIVE_STD
#  include <iterator>
#  include <utility>
#  if !_CCW_LIBCPP_HAS_NATIVE_CXX11_LIB
#    include "../__utility/move.h"   // native <utility> has no std::move below C++11
#  endif
#else
#  include "../__iterator/advance.h"
#  include "../__iterator/distance.h"
#  include "../__iterator/next_prev.h"
#  include "../__iterator/iterator_traits.h"
#  include "../__utility/move.h"
#  include "../__utility/swap.h"
#endif
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

namespace ranges {

template <class _Ip, class _Dp>
_CCW_LIBCPP_HIDE_FROM_ABI void advance(_Ip& __i, _Dp __n) { _CCW_STD::advance(__i, __n); }

template <class _Ip>
_CCW_LIBCPP_HIDE_FROM_ABI typename iterator_traits<_Ip>::difference_type
distance(_Ip __first, _Ip __last) { return _CCW_STD::distance(__first, __last); }

template <class _Ip>
_CCW_LIBCPP_HIDE_FROM_ABI _Ip next(_Ip __x) { return _CCW_STD::next(__x); }
template <class _Ip>
_CCW_LIBCPP_HIDE_FROM_ABI _Ip next(_Ip __x, typename iterator_traits<_Ip>::difference_type __n)
{ return _CCW_STD::next(__x, __n); }

template <class _Ip>
_CCW_LIBCPP_HIDE_FROM_ABI _Ip prev(_Ip __x) { return _CCW_STD::prev(__x); }
template <class _Ip>
_CCW_LIBCPP_HIDE_FROM_ABI _Ip prev(_Ip __x, typename iterator_traits<_Ip>::difference_type __n)
{ return _CCW_STD::prev(__x, __n); }

template <class _Ip>
_CCW_LIBCPP_HIDE_FROM_ABI typename iterator_traits<_Ip>::value_type iter_move(_Ip __i)
{ return _CCW_STD::move(*__i); }

template <class _Ip, class _Jp>
_CCW_LIBCPP_HIDE_FROM_ABI void iter_swap(_Ip __a, _Jp __b) { _CCW_STD::swap(*__a, *__b); }

} // namespace ranges

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___ITERATOR_RANGES_ITER_OPS_H
