//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//  The ExecutionPolicy overloads of the <numeric> algorithms.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___NUMERIC_EXECUTION_OVERLOADS_H
#define _CCW_LIBCPP___NUMERIC_EXECUTION_OVERLOADS_H
#include "../__config"
#include <execution>
#include "../__iterator/iterator_traits.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

#define _CCW_NUM_EXEC_OVERLOADS(_Pol)                                          \
    template <class _It>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI typename iterator_traits<_It>::value_type                                      \
    reduce(const _CCW_STD::execution::_Pol&, _It __f, _It __l)                          \
    { return _CCW_STD::reduce(__f, __l); }                                      \
    template <class _It, class _Tp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp                                      \
    reduce(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Tp __init)                          \
    { return _CCW_STD::reduce(__f, __l, __init); }                                      \
    template <class _It, class _Tp, class _BinOp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp                                      \
    reduce(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Tp __init, _BinOp __op)                          \
    { return _CCW_STD::reduce(__f, __l, __init, __op); }                                      \
    template <class _I1, class _I2, class _Tp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp                                      \
    transform_reduce(const _CCW_STD::execution::_Pol&, _I1 __f1, _I1 __l1, _I2 __f2, _Tp __init)                          \
    { return _CCW_STD::transform_reduce(__f1, __l1, __f2, __init); }                                      \
    template <class _I1, class _I2, class _Tp, class _B1, class _B2>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp                                      \
    transform_reduce(const _CCW_STD::execution::_Pol&, _I1 __f1, _I1 __l1, _I2 __f2, _Tp __init, _B1 __r, _B2 __t)                          \
    { return _CCW_STD::transform_reduce(__f1, __l1, __f2, __init, __r, __t); }                                      \
    template <class _It, class _Tp, class _BinOp, class _UnOp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Tp                                      \
    transform_reduce(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Tp __init, _BinOp __b, _UnOp __u)                          \
    { return _CCW_STD::transform_reduce(__f, __l, __init, __b, __u); }                                      \
    template <class _It, class _Ot>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Ot                                      \
    inclusive_scan(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Ot __d)                          \
    { return _CCW_STD::inclusive_scan(__f, __l, __d); }                                      \
    template <class _It, class _Ot, class _BinOp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Ot                                      \
    inclusive_scan(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Ot __d, _BinOp __op)                          \
    { return _CCW_STD::inclusive_scan(__f, __l, __d, __op); }                                      \
    template <class _It, class _Ot, class _BinOp, class _Tp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Ot                                      \
    inclusive_scan(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Ot __d, _BinOp __op, _Tp __init)                          \
    { return _CCW_STD::inclusive_scan(__f, __l, __d, __op, __init); }                                      \
    template <class _It, class _Ot, class _Tp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Ot                                      \
    exclusive_scan(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Ot __d, _Tp __init)                          \
    { return _CCW_STD::exclusive_scan(__f, __l, __d, __init); }                                      \
    template <class _It, class _Ot, class _Tp, class _BinOp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Ot                                      \
    exclusive_scan(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Ot __d, _Tp __init, _BinOp __op)                          \
    { return _CCW_STD::exclusive_scan(__f, __l, __d, __init, __op); }                                      \
    template <class _It, class _Ot, class _BinOp, class _UnOp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Ot                                      \
    transform_inclusive_scan(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Ot __d, _BinOp __b, _UnOp __u)                          \
    { return _CCW_STD::transform_inclusive_scan(__f, __l, __d, __b, __u); }                                      \
    template <class _It, class _Ot, class _BinOp, class _UnOp, class _Tp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Ot                                      \
    transform_inclusive_scan(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Ot __d, _BinOp __b, _UnOp __u, _Tp __init)                          \
    { return _CCW_STD::transform_inclusive_scan(__f, __l, __d, __b, __u, __init); }                                      \
    template <class _It, class _Ot, class _Tp, class _BinOp, class _UnOp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Ot                                      \
    transform_exclusive_scan(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Ot __d, _Tp __init, _BinOp __b, _UnOp __u)                          \
    { return _CCW_STD::transform_exclusive_scan(__f, __l, __d, __init, __b, __u); }                                      \
    template <class _It, class _Ot>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Ot                                      \
    adjacent_difference(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Ot __d)                          \
    { return _CCW_STD::adjacent_difference(__f, __l, __d); }                                      \
    template <class _It, class _Ot, class _BinOp>                                                     \
    _CCW_LIBCPP_HIDE_FROM_ABI _Ot                                      \
    adjacent_difference(const _CCW_STD::execution::_Pol&, _It __f, _It __l, _Ot __d, _BinOp __op)                          \
    { return _CCW_STD::adjacent_difference(__f, __l, __d, __op); }

_CCW_NUM_EXEC_OVERLOADS(sequenced_policy)
_CCW_NUM_EXEC_OVERLOADS(parallel_policy)
_CCW_NUM_EXEC_OVERLOADS(parallel_unsequenced_policy)
_CCW_NUM_EXEC_OVERLOADS(unsequenced_policy)

#undef _CCW_NUM_EXEC_OVERLOADS

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
