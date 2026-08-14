/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___NUMERIC_INCLUSIVE_SCAN_H
#define _CCW_LIBCPP___NUMERIC_INCLUSIVE_SCAN_H
#include "../__config"
#include "../__functional/operations.h"
#include "../__iterator/iterator_traits.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _InputIt, class _OutputIt, class _BinaryOp, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _OutputIt inclusive_scan(_InputIt __f, _InputIt __l, _OutputIt __d, _BinaryOp __op, _Tp __init) {
    for (; __f != __l; ++__f) { __init = __op(__init, *__f); *__d = __init; ++__d; }
    return __d;
}
template <class _InputIt, class _OutputIt, class _BinaryOp>
_CCW_LIBCPP_HIDE_FROM_ABI _OutputIt inclusive_scan(_InputIt __f, _InputIt __l, _OutputIt __d, _BinaryOp __op) {
    if (__f == __l) return __d;
    typename iterator_traits<_InputIt>::value_type __acc = *__f; *__d = __acc; ++__d; ++__f;
    for (; __f != __l; ++__f) { __acc = __op(__acc, *__f); *__d = __acc; ++__d; }
    return __d;
}
template <class _InputIt, class _OutputIt>
_CCW_LIBCPP_HIDE_FROM_ABI _OutputIt inclusive_scan(_InputIt __f, _InputIt __l, _OutputIt __d) {
    return _CCW_STD::inclusive_scan(__f, __l, __d, _CCW_STD::plus<typename iterator_traits<_InputIt>::value_type>());
}
template <class _InputIt, class _OutputIt, class _BinaryOp, class _UnaryOp, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _OutputIt transform_inclusive_scan(_InputIt __f, _InputIt __l, _OutputIt __d, _BinaryOp __bop, _UnaryOp __uop, _Tp __init) {
    for (; __f != __l; ++__f) { __init = __bop(__init, __uop(*__f)); *__d = __init; ++__d; }
    return __d;
}
template <class _InputIt, class _OutputIt, class _BinaryOp, class _UnaryOp>
_CCW_LIBCPP_HIDE_FROM_ABI _OutputIt transform_inclusive_scan(_InputIt __f, _InputIt __l, _OutputIt __d, _BinaryOp __bop, _UnaryOp __uop) {
    if (__f == __l) return __d;
    typename iterator_traits<_InputIt>::value_type __acc = __uop(*__f); *__d = __acc; ++__d; ++__f;
    for (; __f != __l; ++__f) { __acc = __bop(__acc, __uop(*__f)); *__d = __acc; ++__d; }
    return __d;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
