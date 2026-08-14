/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___NUMERIC_REDUCE_H
#define _CCW_LIBCPP___NUMERIC_REDUCE_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It>
_CCW_LIBCPP_HIDE_FROM_ABI typename iterator_traits<_It>::value_type reduce(_It __f, _It __l) {
    typedef typename iterator_traits<_It>::value_type _Vt;
    _Vt __init = _Vt();
    for (; __f != __l; ++__f) __init = __init + *__f;
    return __init;
}
template <class _It, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _Tp reduce(_It __f, _It __l, _Tp __init) { for (; __f != __l; ++__f) __init = __init + *__f; return __init; }
template <class _It, class _Tp, class _BinOp>
_CCW_LIBCPP_HIDE_FROM_ABI _Tp reduce(_It __f, _It __l, _Tp __init, _BinOp __op) { for (; __f != __l; ++__f) __init = __op(__init, *__f); return __init; }
template <class _It1, class _It2, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _Tp transform_reduce(_It1 __f1, _It1 __l1, _It2 __f2, _Tp __init) { for (; __f1 != __l1; ++__f1, ++__f2) __init = __init + (*__f1) * (*__f2); return __init; }
template <class _It1, class _It2, class _Tp, class _BinOp1, class _BinOp2>
_CCW_LIBCPP_HIDE_FROM_ABI _Tp transform_reduce(_It1 __f1, _It1 __l1, _It2 __f2, _Tp __init, _BinOp1 __r, _BinOp2 __t) { for (; __f1 != __l1; ++__f1, ++__f2) __init = __r(__init, __t(*__f1, *__f2)); return __init; }
template <class _It, class _Tp, class _BinOp, class _UnOp>
_CCW_LIBCPP_HIDE_FROM_ABI _Tp transform_reduce(_It __f, _It __l, _Tp __init, _BinOp __b, _UnOp __u) { for (; __f != __l; ++__f) __init = __b(__init, __u(*__f)); return __init; }
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
