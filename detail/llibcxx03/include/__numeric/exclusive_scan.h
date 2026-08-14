/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___NUMERIC_EXCLUSIVE_SCAN_H
#define _CCW_LIBCPP___NUMERIC_EXCLUSIVE_SCAN_H
#include "../__config"
#include "../__functional/operations.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _InputIt, class _OutputIt, class _Tp, class _BinaryOp>
_CCW_LIBCPP_HIDE_FROM_ABI _OutputIt exclusive_scan(_InputIt __f, _InputIt __l, _OutputIt __d, _Tp __init, _BinaryOp __op) {
    for (; __f != __l; ++__f) { _Tp __tmp = __op(__init, *__f); *__d = __init; ++__d; __init = __tmp; }
    return __d;
}
template <class _InputIt, class _OutputIt, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _OutputIt exclusive_scan(_InputIt __f, _InputIt __l, _OutputIt __d, _Tp __init) {
    return _CCW_STD::exclusive_scan(__f, __l, __d, __init, _CCW_STD::plus<_Tp>());
}
template <class _InputIt, class _OutputIt, class _Tp, class _BinaryOp, class _UnaryOp>
_CCW_LIBCPP_HIDE_FROM_ABI _OutputIt transform_exclusive_scan(_InputIt __f, _InputIt __l, _OutputIt __d, _Tp __init, _BinaryOp __bop, _UnaryOp __uop) {
    for (; __f != __l; ++__f) { _Tp __tmp = __bop(__init, __uop(*__f)); *__d = __init; ++__d; __init = __tmp; }
    return __d;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
