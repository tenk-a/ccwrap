/*
 * ccwrap: libc++ layout, classic C++03 body.
 *  _CCW_STD::move / _CCW_STD::move_backward algorithms. Each element is assigned through the
 *  ccwrap move cast (_CCW_STD::move): a type with a move assignment taking rv<T>& is moved
 *  from, everything else copies -- the same graceful degradation used elsewhere on
 *  Open Watcom (no true rvalue references). For copyable types the effect is identical
 *  to _CCW_STD::copy / _CCW_STD::copy_backward.
 */
#ifndef _CCW_LIBCPP___ALGORITHM_MOVE_H
#define _CCW_LIBCPP___ALGORITHM_MOVE_H
#include "../__config"
#include "../__utility/move.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _InputIt, class _OutputIt>
_CCW_LIBCPP_HIDE_FROM_ABI _OutputIt move(_InputIt __f, _InputIt __l, _OutputIt __d) {
    for (; __f != __l; ++__f, ++__d) *__d = _CCW_STD::move(*__f);
    return __d;
}
template <class _BidIt1, class _BidIt2>
_CCW_LIBCPP_HIDE_FROM_ABI _BidIt2 move_backward(_BidIt1 __f, _BidIt1 __l, _BidIt2 __dl) {
    while (__l != __f) { --__l; --__dl; *__dl = _CCW_STD::move(*__l); }
    return __dl;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
