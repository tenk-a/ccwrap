// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  __algorithm/ranges_minmax.h -- min / max / clamp, the element forms, the
//  permutation generators and the fold family.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_RANGES_MINMAX_H
#define _CCW_LIBCPP___ALGORITHM_RANGES_MINMAX_H

#include "../__config"

// vc12 has its own class template std::identity in <type_traits>, which collides
// with the ranges identity; the ranges algorithms therefore start at vc14.
#if _CCW_HAS_ALIAS_TEMPLATE && (!defined(_MSC_VER) || _MSC_VER >= 1900)

#include "ranges_util.h"
#include "ranges_sorting.h"
#include "min_max_result.h"
#include "in_found_result.h"
#include "out_value_result.h"
#include "in_value_result.h"
#include <optional>
#include "in_out_result.h"
#include <algorithm>
#include <initializer_list>

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
namespace ranges {

template <class _Tp> using minmax_result         = min_max_result<_Tp>;
template <class _Tp> using minmax_element_result = min_max_result<_Tp>;
template <class _Ip> using next_permutation_result = in_found_result<_Ip>;
template <class _Ip> using prev_permutation_result = in_found_result<_Ip>;
template <class _Op, class _Tp> using iota_result = out_value_result<_Op, _Tp>;

template <class _Tp, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI const _Tp&
min(const _Tp& __a, const _Tp& __b, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    return __c(ranges::__ccw_proj(__pr, const_cast<_Tp&>(__b)),
               ranges::__ccw_proj(__pr, const_cast<_Tp&>(__a))) ? __b : __a;
}
template <class _Tp, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _Tp
min(_CCW_STD::initializer_list<_Tp> __il, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    const _Tp* __b = __il.begin();
    const _Tp* __m = __b;
    for (const _Tp* __i = __b; __i != __il.end(); ++__i) {
        if (__c(ranges::__ccw_proj(__pr, const_cast<_Tp&>(*__i)),
                ranges::__ccw_proj(__pr, const_cast<_Tp&>(*__m)))) __m = __i;
    }
    return *__m;
}
template <class _Rp, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI
_CCW_RNG_IF(_Rp, typename _CCW_STD::iterator_traits<_CCW_RNG_IT(_Rp)>::value_type)
min(_Rp&& __r, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    _CCW_RNG_IT(_Rp) __f = ranges::__ccw_begin(__r);
    _CCW_RNG_IT(_Rp) __l = ranges::__ccw_end(__r);
    _CCW_RNG_IT(_Rp) __m = __f;
    for (; __f != __l; ++__f) {
        if (__c(ranges::__ccw_proj(__pr, *__f), ranges::__ccw_proj(__pr, *__m))) __m = __f;
    }
    return *__m;
}

template <class _Tp, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI const _Tp&
max(const _Tp& __a, const _Tp& __b, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    return __c(ranges::__ccw_proj(__pr, const_cast<_Tp&>(__a)),
               ranges::__ccw_proj(__pr, const_cast<_Tp&>(__b))) ? __b : __a;
}
template <class _Tp, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _Tp
max(_CCW_STD::initializer_list<_Tp> __il, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    const _Tp* __b = __il.begin();
    const _Tp* __m = __b;
    for (const _Tp* __i = __b; __i != __il.end(); ++__i) {
        if (__c(ranges::__ccw_proj(__pr, const_cast<_Tp&>(*__m)),
                ranges::__ccw_proj(__pr, const_cast<_Tp&>(*__i)))) __m = __i;
    }
    return *__m;
}
template <class _Rp, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI
_CCW_RNG_IF(_Rp, typename _CCW_STD::iterator_traits<_CCW_RNG_IT(_Rp)>::value_type)
max(_Rp&& __r, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    _CCW_RNG_IT(_Rp) __f = ranges::__ccw_begin(__r);
    _CCW_RNG_IT(_Rp) __l = ranges::__ccw_end(__r);
    _CCW_RNG_IT(_Rp) __m = __f;
    for (; __f != __l; ++__f) {
        if (__c(ranges::__ccw_proj(__pr, *__m), ranges::__ccw_proj(__pr, *__f))) __m = __f;
    }
    return *__m;
}

template <class _Tp, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI minmax_result<const _Tp&>
minmax(const _Tp& __a, const _Tp& __b, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    bool __blt = __c(ranges::__ccw_proj(__pr, const_cast<_Tp&>(__b)),
                     ranges::__ccw_proj(__pr, const_cast<_Tp&>(__a)));
    minmax_result<const _Tp&> __r = { __blt ? __b : __a, __blt ? __a : __b };
    return __r;
}
template <class _Tp, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI minmax_result<_Tp>
minmax(_CCW_STD::initializer_list<_Tp> __il, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    minmax_result<_Tp> __r = { ranges::min(__il, __c, __pr), ranges::max(__il, __c, __pr) };
    return __r;
}
template <class _Rp, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI
_CCW_RNG_IF(_Rp, minmax_result<typename _CCW_STD::iterator_traits<_CCW_RNG_IT(_Rp)>::value_type>)
minmax(_Rp&& __r, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    minmax_result<typename _CCW_STD::iterator_traits<_CCW_RNG_IT(_Rp)>::value_type> __o
        = { ranges::min(__r, __c, __pr), ranges::max(__r, __c, __pr) };
    return __o;
}

template <class _It, class _Sent, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
min_element(_It __f, _Sent __l, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    if (__f == __l) return __f;
    _It __m = __f;
    for (++__f; __f != __l; ++__f) {
        if (__c(ranges::__ccw_proj(__pr, *__f), ranges::__ccw_proj(__pr, *__m))) __m = __f;
    }
    return __m;
}
_CCW_RNG_RANGE_A0(min_element, less, _CCW_RNG_IT(_Rp))

template <class _It, class _Sent, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
max_element(_It __f, _Sent __l, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    if (__f == __l) return __f;
    _It __m = __f;
    for (++__f; __f != __l; ++__f) {
        if (__c(ranges::__ccw_proj(__pr, *__m), ranges::__ccw_proj(__pr, *__f))) __m = __f;
    }
    return __m;
}
_CCW_RNG_RANGE_A0(max_element, less, _CCW_RNG_IT(_Rp))

template <class _It, class _Sent, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, minmax_element_result<_It>)
minmax_element(_It __f, _Sent __l, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    minmax_element_result<_It> __r = { ranges::min_element(__f, __l, __c, __pr),
                                       ranges::max_element(__f, __l, __c, __pr) };
    return __r;
}
_CCW_RNG_RANGE_A0(minmax_element, less, minmax_element_result<_CCW_RNG_IT(_Rp)>)

template <class _Tp, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI const _Tp&
clamp(const _Tp& __v, const _Tp& __lo, const _Tp& __hi, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    if (__c(ranges::__ccw_proj(__pr, const_cast<_Tp&>(__v)),
            ranges::__ccw_proj(__pr, const_cast<_Tp&>(__lo)))) return __lo;
    if (__c(ranges::__ccw_proj(__pr, const_cast<_Tp&>(__hi)),
            ranges::__ccw_proj(__pr, const_cast<_Tp&>(__v)))) return __hi;
    return __v;
}

template <class _I1, class _S1, class _I2, class _S2, class _Comp = less,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_I1, bool)
lexicographical_compare(_I1 __f1, _S1 __l1, _I2 __f2, _S2 __l2, _Comp __c = _Comp(),
                        _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    for (; __f1 != __l1 && __f2 != __l2; ++__f1, (void)++__f2) {
        if (__c(ranges::__ccw_proj(__p1, *__f1), ranges::__ccw_proj(__p2, *__f2))) return true;
        if (__c(ranges::__ccw_proj(__p2, *__f2), ranges::__ccw_proj(__p1, *__f1))) return false;
    }
    return __f1 == __l1 && __f2 != __l2;
}
template <class _R1, class _R2, class _Comp = less, class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_R1, bool)
lexicographical_compare(_R1&& __r1, _R2&& __r2, _Comp __c = _Comp(), _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    return ranges::lexicographical_compare(ranges::__ccw_begin(__r1), ranges::__ccw_end(__r1),
                                           ranges::__ccw_begin(__r2), ranges::__ccw_end(__r2), __c, __p1, __p2);
}

template <class _It, class _Sent, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, next_permutation_result<_It>)
next_permutation(_It __f, _Sent __l, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    bool __ok = _CCW_STD::next_permutation(__f, __l, ranges::__ccw_mkcomp(__c, __pr));
    next_permutation_result<_It> __r = { __l, __ok };
    return __r;
}
_CCW_RNG_RANGE_A0(next_permutation, less, next_permutation_result<_CCW_RNG_IT(_Rp)>)

template <class _It, class _Sent, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, prev_permutation_result<_It>)
prev_permutation(_It __f, _Sent __l, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    bool __ok = _CCW_STD::prev_permutation(__f, __l, ranges::__ccw_mkcomp(__c, __pr));
    prev_permutation_result<_It> __r = { __l, __ok };
    return __r;
}
_CCW_RNG_RANGE_A0(prev_permutation, less, prev_permutation_result<_CCW_RNG_IT(_Rp)>)

template <class _It, class _Sent, class _Gen>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It) shuffle(_It __f, _Sent __l, _Gen&& __g) {
    typename _CCW_STD::iterator_traits<_It>::difference_type __n = ranges::__ccw_dist(__f, __l);
    for (; __n > 1; --__n) {
        typename _CCW_STD::iterator_traits<_It>::difference_type __k =
            (typename _CCW_STD::iterator_traits<_It>::difference_type)(__g() % (unsigned long long)__n);
        _It __a = __f; for (typename _CCW_STD::iterator_traits<_It>::difference_type __i = 0; __i < __n - 1; ++__i) ++__a;
        _It __b = __f; for (typename _CCW_STD::iterator_traits<_It>::difference_type __i = 0; __i < __k;     ++__i) ++__b;
        typename _CCW_STD::iterator_traits<_It>::value_type __t = *__a;
        *__a = *__b;
        *__b = __t;
    }
    return __l;
}
template <class _Rp, class _Gen>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, _CCW_RNG_IT(_Rp)) shuffle(_Rp&& __r, _Gen&& __g) {
    return ranges::shuffle(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __g);
}

template <class _It, class _Sent, class _Out, class _Diff, class _Gen>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _Out)
sample(_It __f, _Sent __l, _Out __o, _Diff __n, _Gen&& __g) {
    typename _CCW_STD::iterator_traits<_It>::difference_type __left = ranges::__ccw_dist(__f, __l);
    for (; __f != __l && __n > 0; ++__f, (void)--__left) {
        if ((_Diff)(__g() % (unsigned long long)__left) < __n) { *__o = *__f; ++__o; --__n; }
    }
    return __o;
}
template <class _Rp, class _Out, class _Diff, class _Gen>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, _Out)
sample(_Rp&& __r, _Out __o, _Diff __n, _Gen&& __g) {
    return ranges::sample(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __o, __n, __g);
}

template <class _Ip, class _Tp> using fold_left_with_iter_result       = in_value_result<_Ip, _Tp>;
template <class _Ip, class _Tp> using fold_left_first_with_iter_result = in_value_result<_Ip, _Tp>;

template <class _It, class _Sent, class _Tp, class _Fun>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _Tp)
fold_left(_It __f, _Sent __l, _Tp __init, _Fun __fn) {
    for (; __f != __l; ++__f) __init = __fn(__init, *__f);
    return __init;
}
template <class _Rp, class _Tp, class _Fun>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, _Tp)
fold_left(_Rp&& __r, _Tp __init, _Fun __fn) {
    return ranges::fold_left(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __init, __fn);
}

template <class _It, class _Sent, class _Tp, class _Fun>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, fold_left_with_iter_result<_It _CCW_RNG_C _Tp>)
fold_left_with_iter(_It __f, _Sent __l, _Tp __init, _Fun __fn) {
    for (; __f != __l; ++__f) __init = __fn(__init, *__f);
    fold_left_with_iter_result<_It, _Tp> __r = { __f, __init };
    return __r;
}
template <class _Rp, class _Tp, class _Fun>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, fold_left_with_iter_result<_CCW_RNG_IT(_Rp) _CCW_RNG_C _Tp>)
fold_left_with_iter(_Rp&& __r, _Tp __init, _Fun __fn) {
    return ranges::fold_left_with_iter(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __init, __fn);
}

template <class _It, class _Sent, class _Fun>
_CCW_LIBCPP_HIDE_FROM_ABI
_CCW_RNG_IFN(_It, _CCW_STD::optional<typename _CCW_STD::iterator_traits<_It>::value_type>)
fold_left_first(_It __f, _Sent __l, _Fun __fn) {
    typedef typename _CCW_STD::iterator_traits<_It>::value_type _Vt;
    if (__f == __l) return _CCW_STD::optional<_Vt>();
    _Vt __acc = *__f;
    for (++__f; __f != __l; ++__f) __acc = __fn(__acc, *__f);
    return _CCW_STD::optional<_Vt>(__acc);
}
template <class _Rp, class _Fun>
_CCW_LIBCPP_HIDE_FROM_ABI
_CCW_RNG_IF(_Rp, _CCW_STD::optional<typename _CCW_STD::iterator_traits<_CCW_RNG_IT(_Rp)>::value_type>)
fold_left_first(_Rp&& __r, _Fun __fn) {
    return ranges::fold_left_first(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __fn);
}

template <class _It, class _Sent, class _Fun>
_CCW_LIBCPP_HIDE_FROM_ABI
_CCW_RNG_IFN(_It, fold_left_first_with_iter_result<_It _CCW_RNG_C
             _CCW_STD::optional<typename _CCW_STD::iterator_traits<_It>::value_type> >)
fold_left_first_with_iter(_It __f, _Sent __l, _Fun __fn) {
    typedef typename _CCW_STD::iterator_traits<_It>::value_type _Vt;
    _CCW_STD::optional<_Vt> __acc;
    if (__f != __l) {
        _Vt __a = *__f;
        for (++__f; __f != __l; ++__f) __a = __fn(__a, *__f);
        __acc = _CCW_STD::optional<_Vt>(__a);
    }
    fold_left_first_with_iter_result<_It, _CCW_STD::optional<_Vt> > __r = { __f, __acc };
    return __r;
}
template <class _Rp, class _Fun>
_CCW_LIBCPP_HIDE_FROM_ABI
_CCW_RNG_IF(_Rp, fold_left_first_with_iter_result<_CCW_RNG_IT(_Rp) _CCW_RNG_C
            _CCW_STD::optional<typename _CCW_STD::iterator_traits<_CCW_RNG_IT(_Rp)>::value_type> >)
fold_left_first_with_iter(_Rp&& __r, _Fun __fn) {
    return ranges::fold_left_first_with_iter(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __fn);
}

template <class _It, class _Sent, class _Tp, class _Fun>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _Tp)
fold_right(_It __f, _Sent __l, _Tp __init, _Fun __fn) {
    _It __e = __l;
    while (__e != __f) { --__e; __init = __fn(*__e, __init); }
    return __init;
}
template <class _Rp, class _Tp, class _Fun>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, _Tp)
fold_right(_Rp&& __r, _Tp __init, _Fun __fn) {
    return ranges::fold_right(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __init, __fn);
}

template <class _It, class _Sent, class _Fun>
_CCW_LIBCPP_HIDE_FROM_ABI
_CCW_RNG_IFN(_It, _CCW_STD::optional<typename _CCW_STD::iterator_traits<_It>::value_type>)
fold_right_last(_It __f, _Sent __l, _Fun __fn) {
    typedef typename _CCW_STD::iterator_traits<_It>::value_type _Vt;
    if (__f == __l) return _CCW_STD::optional<_Vt>();
    _It __e = __l;
    --__e;
    _Vt __acc = *__e;
    while (__e != __f) { --__e; __acc = __fn(*__e, __acc); }
    return _CCW_STD::optional<_Vt>(__acc);
}
template <class _Rp, class _Fun>
_CCW_LIBCPP_HIDE_FROM_ABI
_CCW_RNG_IF(_Rp, _CCW_STD::optional<typename _CCW_STD::iterator_traits<_CCW_RNG_IT(_Rp)>::value_type>)
fold_right_last(_Rp&& __r, _Fun __fn) {
    return ranges::fold_right_last(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __fn);
}

template <class _It, class _Sent, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, iota_result<_It _CCW_RNG_C _Tp>)
iota(_It __f, _Sent __l, _Tp __v) {
    for (; __f != __l; ++__f, (void)++__v) *__f = __v;
    iota_result<_It, _Tp> __r = { __f, __v };
    return __r;
}
template <class _Rp, class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, iota_result<_CCW_RNG_IT(_Rp) _CCW_RNG_C _Tp>)
iota(_Rp&& __r, _Tp __v) {
    return ranges::iota(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __v);
}

} // namespace ranges
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_HAS_ALIAS_TEMPLATE && vc14+
#endif // _CCW_LIBCPP___ALGORITHM_RANGES_MINMAX_H
