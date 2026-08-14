/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___FUNCTIONAL_OPERATIONS_H
#define _CCW_LIBCPP___FUNCTIONAL_OPERATIONS_H
#include "../__config"
#include "../__functional/binary_function.h"
#include "../__functional/unary_function.h"
#if _CCW_LIBCPP_NATIVE_STD
#  include <functional>
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS plus       : binary_function<_Tp, _Tp, _Tp> { _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator()(const _Tp& __x, const _Tp& __y) const { return __x + __y; } };
template <class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS minus      : binary_function<_Tp, _Tp, _Tp> { _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator()(const _Tp& __x, const _Tp& __y) const { return __x - __y; } };
template <class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS multiplies : binary_function<_Tp, _Tp, _Tp> { _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator()(const _Tp& __x, const _Tp& __y) const { return __x * __y; } };
template <class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS divides    : binary_function<_Tp, _Tp, _Tp> { _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator()(const _Tp& __x, const _Tp& __y) const { return __x / __y; } };
template <class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS modulus    : binary_function<_Tp, _Tp, _Tp> { _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator()(const _Tp& __x, const _Tp& __y) const { return __x % __y; } };
template <class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS negate     : unary_function<_Tp, _Tp>       { _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator()(const _Tp& __x) const { return -__x; } };

template <class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS equal_to      : binary_function<_Tp, _Tp, bool> { _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _Tp& __x, const _Tp& __y) const { return __x == __y; } };
template <class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS not_equal_to  : binary_function<_Tp, _Tp, bool> { _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _Tp& __x, const _Tp& __y) const { return __x != __y; } };
template <class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS less          : binary_function<_Tp, _Tp, bool> { _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _Tp& __x, const _Tp& __y) const { return __x <  __y; } };
template <class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS greater       : binary_function<_Tp, _Tp, bool> { _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _Tp& __x, const _Tp& __y) const { return __x >  __y; } };
template <class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS less_equal    : binary_function<_Tp, _Tp, bool> { _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _Tp& __x, const _Tp& __y) const { return __x <= __y; } };
template <class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS greater_equal : binary_function<_Tp, _Tp, bool> { _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _Tp& __x, const _Tp& __y) const { return __x >= __y; } };

template <class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS logical_and : binary_function<_Tp, _Tp, bool> { _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _Tp& __x, const _Tp& __y) const { return __x && __y; } };
template <class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS logical_or  : binary_function<_Tp, _Tp, bool> { _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _Tp& __x, const _Tp& __y) const { return __x || __y; } };
template <class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS logical_not : unary_function<_Tp, bool>       { _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _Tp& __x) const { return !__x; } };

struct __ccw_is_transparent { typedef void is_transparent; };
template <> struct _CCW_LIBCPP_TEMPLATE_VIS equal_to<void>      : __ccw_is_transparent { template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _T1& __x, const _T2& __y) const { return __x == __y; } };
template <> struct _CCW_LIBCPP_TEMPLATE_VIS not_equal_to<void>  : __ccw_is_transparent { template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _T1& __x, const _T2& __y) const { return __x != __y; } };
template <> struct _CCW_LIBCPP_TEMPLATE_VIS less<void>          : __ccw_is_transparent { template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _T1& __x, const _T2& __y) const { return __x <  __y; } };
template <> struct _CCW_LIBCPP_TEMPLATE_VIS greater<void>       : __ccw_is_transparent { template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _T1& __x, const _T2& __y) const { return __x >  __y; } };
template <> struct _CCW_LIBCPP_TEMPLATE_VIS less_equal<void>    : __ccw_is_transparent { template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _T1& __x, const _T2& __y) const { return __x <= __y; } };
template <> struct _CCW_LIBCPP_TEMPLATE_VIS greater_equal<void> : __ccw_is_transparent { template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _T1& __x, const _T2& __y) const { return __x >= __y; } };
template <> struct _CCW_LIBCPP_TEMPLATE_VIS logical_and<void>   : __ccw_is_transparent { template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _T1& __x, const _T2& __y) const { return __x && __y; } };
template <> struct _CCW_LIBCPP_TEMPLATE_VIS logical_or<void>    : __ccw_is_transparent { template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _T1& __x, const _T2& __y) const { return __x || __y; } };
template <> struct _CCW_LIBCPP_TEMPLATE_VIS logical_not<void>   : __ccw_is_transparent { template <class _T1> _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _T1& __x) const { return !__x; } };

template <class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS bit_and : binary_function<_Tp, _Tp, _Tp> { _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator()(const _Tp& __x, const _Tp& __y) const { return __x & __y; } };
template <class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS bit_or  : binary_function<_Tp, _Tp, _Tp> { _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator()(const _Tp& __x, const _Tp& __y) const { return __x | __y; } };
template <class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS bit_xor : binary_function<_Tp, _Tp, _Tp> { _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator()(const _Tp& __x, const _Tp& __y) const { return __x ^ __y; } };
template <class _Tp = void> struct _CCW_LIBCPP_TEMPLATE_VIS bit_not : unary_function<_Tp, _Tp>       { _CCW_LIBCPP_HIDE_FROM_ABI _Tp operator()(const _Tp& __x) const { return ~__x; } };  // C++14
template <> struct _CCW_LIBCPP_TEMPLATE_VIS plus<void>       : __ccw_is_transparent { template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI _T1 operator()(const _T1& __x, const _T2& __y) const { return __x + __y; } };
template <> struct _CCW_LIBCPP_TEMPLATE_VIS minus<void>      : __ccw_is_transparent { template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI _T1 operator()(const _T1& __x, const _T2& __y) const { return __x - __y; } };
template <> struct _CCW_LIBCPP_TEMPLATE_VIS multiplies<void> : __ccw_is_transparent { template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI _T1 operator()(const _T1& __x, const _T2& __y) const { return __x * __y; } };
template <> struct _CCW_LIBCPP_TEMPLATE_VIS divides<void>    : __ccw_is_transparent { template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI _T1 operator()(const _T1& __x, const _T2& __y) const { return __x / __y; } };
template <> struct _CCW_LIBCPP_TEMPLATE_VIS modulus<void>    : __ccw_is_transparent { template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI _T1 operator()(const _T1& __x, const _T2& __y) const { return __x % __y; } };
template <> struct _CCW_LIBCPP_TEMPLATE_VIS negate<void>     : __ccw_is_transparent { template <class _T1> _CCW_LIBCPP_HIDE_FROM_ABI _T1 operator()(const _T1& __x) const { return -__x; } };
template <> struct _CCW_LIBCPP_TEMPLATE_VIS bit_and<void>    : __ccw_is_transparent { template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI _T1 operator()(const _T1& __x, const _T2& __y) const { return __x & __y; } };
template <> struct _CCW_LIBCPP_TEMPLATE_VIS bit_or<void>     : __ccw_is_transparent { template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI _T1 operator()(const _T1& __x, const _T2& __y) const { return __x | __y; } };
template <> struct _CCW_LIBCPP_TEMPLATE_VIS bit_xor<void>    : __ccw_is_transparent { template <class _T1, class _T2> _CCW_LIBCPP_HIDE_FROM_ABI _T1 operator()(const _T1& __x, const _T2& __y) const { return __x ^ __y; } };
template <> struct _CCW_LIBCPP_TEMPLATE_VIS bit_not<void>    : __ccw_is_transparent { template <class _T1> _CCW_LIBCPP_HIDE_FROM_ABI _T1 operator()(const _T1& __x) const { return ~__x; } };

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_NATIVE_STD
#endif
