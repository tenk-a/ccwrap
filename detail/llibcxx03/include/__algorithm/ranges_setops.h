// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  __algorithm/ranges_setops.h -- the binary-search, merge, set and heap
//  ranges:: algorithms ([alg.binary.search], [alg.merge], [alg.set.operations],
//  [alg.heap.operations]).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_RANGES_SETOPS_H
#define _CCW_LIBCPP___ALGORITHM_RANGES_SETOPS_H

#include "../__config"

#if _CCW_HAS_ALIAS_TEMPLATE && (!defined(_MSC_VER) || _MSC_VER >= 1900)

#include "ranges_util.h"
#include "ranges_sorting.h"
#include "in_in_out_result.h"
#include <algorithm>

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
namespace ranges {

template <class _I1, class _I2, class _Op> using merge_result = in_in_out_result<_I1, _I2, _Op>;
template <class _I1, class _I2, class _Op> using set_union_result = in_in_out_result<_I1, _I2, _Op>;
template <class _I1, class _I2, class _Op> using set_intersection_result = in_in_out_result<_I1, _I2, _Op>;
template <class _Ip, class _Op>            using set_difference_result = in_out_result<_Ip, _Op>;
template <class _I1, class _I2, class _Op> using set_symmetric_difference_result = in_in_out_result<_I1, _I2, _Op>;

template <class _It, class _Sent, class _Vp, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
lower_bound(_It __f, _Sent __l, const _Vp& __v, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f) { if (!__c(ranges::__ccw_proj(__pr, *__f), __v)) break; }
    return __f;
}
template <class _Rp, class _Vp, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, _CCW_RNG_IT(_Rp))
lower_bound(_Rp&& __r, const _Vp& __v, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    return ranges::lower_bound(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __v, __c, __pr);
}

template <class _It, class _Sent, class _Vp, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
upper_bound(_It __f, _Sent __l, const _Vp& __v, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f) { if (__c(__v, ranges::__ccw_proj(__pr, *__f))) break; }
    return __f;
}
template <class _Rp, class _Vp, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, _CCW_RNG_IT(_Rp))
upper_bound(_Rp&& __r, const _Vp& __v, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    return ranges::upper_bound(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __v, __c, __pr);
}

template <class _It, class _Sent, class _Vp, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, subrange<_It>)
equal_range(_It __f, _Sent __l, const _Vp& __v, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    _It __b = ranges::lower_bound(__f, __l, __v, __c, __pr);
    _It __e = ranges::upper_bound(__b, __l, __v, __c, __pr);
    return subrange<_It>(__b, __e);
}
template <class _Rp, class _Vp, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, subrange<_CCW_RNG_IT(_Rp)>)
equal_range(_Rp&& __r, const _Vp& __v, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    return ranges::equal_range(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __v, __c, __pr);
}

template <class _It, class _Sent, class _Vp, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, bool)
binary_search(_It __f, _Sent __l, const _Vp& __v, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    _It __b = ranges::lower_bound(__f, __l, __v, __c, __pr);
    return __b != __l && !__c(__v, ranges::__ccw_proj(__pr, *__b));
}
template <class _Rp, class _Vp, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, bool)
binary_search(_Rp&& __r, const _Vp& __v, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    return ranges::binary_search(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __v, __c, __pr);
}

template <class _I1, class _S1, class _I2, class _S2, class _Out, class _Comp = less,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_I1, merge_result<_I1 _CCW_RNG_C _I2 _CCW_RNG_C _Out>)
merge(_I1 __f1, _S1 __l1, _I2 __f2, _S2 __l2, _Out __o, _Comp __c = _Comp(),
      _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    while (__f1 != __l1 && __f2 != __l2) {
        if (__c(ranges::__ccw_proj(__p2, *__f2), ranges::__ccw_proj(__p1, *__f1))) { *__o = *__f2; ++__f2; }
        else                                                                       { *__o = *__f1; ++__f1; }
        ++__o;
    }
    for (; __f1 != __l1; ++__f1, (void)++__o) *__o = *__f1;
    for (; __f2 != __l2; ++__f2, (void)++__o) *__o = *__f2;
    merge_result<_I1, _I2, _Out> __r = { __f1, __f2, __o };
    return __r;
}
template <class _R1, class _R2, class _Out, class _Comp = less,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI
_CCW_RNG_IF(_R1, merge_result<_CCW_RNG_IT(_R1) _CCW_RNG_C _CCW_RNG_IT(_R2) _CCW_RNG_C _Out>)
merge(_R1&& __r1, _R2&& __r2, _Out __o, _Comp __c = _Comp(), _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    return ranges::merge(ranges::__ccw_begin(__r1), ranges::__ccw_end(__r1),
                         ranges::__ccw_begin(__r2), ranges::__ccw_end(__r2), __o, __c, __p1, __p2);
}

template <class _It, class _Sent, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
inplace_merge(_It __f, _It __m, _Sent __l, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    _CCW_STD::inplace_merge(__f, __m, __l, ranges::__ccw_mkcomp(__c, __pr));
    return __l;
}
template <class _Rp, class _It, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, _CCW_RNG_IT(_Rp))
inplace_merge(_Rp&& __r, _It __m, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    return ranges::inplace_merge(ranges::__ccw_begin(__r), __m, ranges::__ccw_end(__r), __c, __pr);
}

template <class _I1, class _S1, class _I2, class _S2, class _Comp = less,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_I1, bool)
includes(_I1 __f1, _S1 __l1, _I2 __f2, _S2 __l2, _Comp __c = _Comp(),
         _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    for (; __f2 != __l2; ++__f1) {
        if (__f1 == __l1 || __c(ranges::__ccw_proj(__p2, *__f2), ranges::__ccw_proj(__p1, *__f1))) return false;
        if (!__c(ranges::__ccw_proj(__p1, *__f1), ranges::__ccw_proj(__p2, *__f2))) ++__f2;
    }
    return true;
}
template <class _R1, class _R2, class _Comp = less, class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_R1, bool)
includes(_R1&& __r1, _R2&& __r2, _Comp __c = _Comp(), _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    return ranges::includes(ranges::__ccw_begin(__r1), ranges::__ccw_end(__r1),
                            ranges::__ccw_begin(__r2), ranges::__ccw_end(__r2), __c, __p1, __p2);
}

#define _CCW_RNG_SETOP(_Nm, _Res)                                                                \
    template <class _R1, class _R2, class _Out, class _Comp = less,                               \
              class _P1 = identity, class _P2 = identity>                                        \
    _CCW_LIBCPP_HIDE_FROM_ABI                                                                    \
    _CCW_RNG_IF(_R1, _Res)                                                                       \
    _Nm(_R1&& __r1, _R2&& __r2, _Out __o, _Comp __c = _Comp(),                                   \
        _P1 __p1 = _P1(), _P2 __p2 = _P2()) {                                                    \
        return ranges::_Nm(ranges::__ccw_begin(__r1), ranges::__ccw_end(__r1),                   \
                           ranges::__ccw_begin(__r2), ranges::__ccw_end(__r2), __o, __c, __p1, __p2); }

template <class _I1, class _S1, class _I2, class _S2, class _Out, class _Comp = less,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_I1, set_union_result<_I1 _CCW_RNG_C _I2 _CCW_RNG_C _Out>)
set_union(_I1 __f1, _S1 __l1, _I2 __f2, _S2 __l2, _Out __o, _Comp __c = _Comp(),
          _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    while (__f1 != __l1 && __f2 != __l2) {
        if (__c(ranges::__ccw_proj(__p1, *__f1), ranges::__ccw_proj(__p2, *__f2))) { *__o = *__f1; ++__f1; }
        else if (__c(ranges::__ccw_proj(__p2, *__f2), ranges::__ccw_proj(__p1, *__f1))) { *__o = *__f2; ++__f2; }
        else { *__o = *__f1; ++__f1; ++__f2; }
        ++__o;
    }
    for (; __f1 != __l1; ++__f1, (void)++__o) *__o = *__f1;
    for (; __f2 != __l2; ++__f2, (void)++__o) *__o = *__f2;
    set_union_result<_I1, _I2, _Out> __r = { __f1, __f2, __o };
    return __r;
}
_CCW_RNG_SETOP(set_union, set_union_result<_CCW_RNG_IT(_R1) _CCW_RNG_C _CCW_RNG_IT(_R2) _CCW_RNG_C _Out>)

template <class _I1, class _S1, class _I2, class _S2, class _Out, class _Comp = less,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_I1, set_intersection_result<_I1 _CCW_RNG_C _I2 _CCW_RNG_C _Out>)
set_intersection(_I1 __f1, _S1 __l1, _I2 __f2, _S2 __l2, _Out __o, _Comp __c = _Comp(),
                 _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    while (__f1 != __l1 && __f2 != __l2) {
        if (__c(ranges::__ccw_proj(__p1, *__f1), ranges::__ccw_proj(__p2, *__f2))) ++__f1;
        else if (__c(ranges::__ccw_proj(__p2, *__f2), ranges::__ccw_proj(__p1, *__f1))) ++__f2;
        else { *__o = *__f1; ++__o; ++__f1; ++__f2; }
    }
    for (; __f1 != __l1; ++__f1) {}
    for (; __f2 != __l2; ++__f2) {}
    set_intersection_result<_I1, _I2, _Out> __r = { __f1, __f2, __o };
    return __r;
}
_CCW_RNG_SETOP(set_intersection, set_intersection_result<_CCW_RNG_IT(_R1) _CCW_RNG_C _CCW_RNG_IT(_R2) _CCW_RNG_C _Out>)

template <class _I1, class _S1, class _I2, class _S2, class _Out, class _Comp = less,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_I1, set_difference_result<_I1 _CCW_RNG_C _Out>)
set_difference(_I1 __f1, _S1 __l1, _I2 __f2, _S2 __l2, _Out __o, _Comp __c = _Comp(),
               _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    while (__f1 != __l1 && __f2 != __l2) {
        if (__c(ranges::__ccw_proj(__p1, *__f1), ranges::__ccw_proj(__p2, *__f2))) { *__o = *__f1; ++__o; ++__f1; }
        else if (__c(ranges::__ccw_proj(__p2, *__f2), ranges::__ccw_proj(__p1, *__f1))) ++__f2;
        else { ++__f1; ++__f2; }
    }
    for (; __f1 != __l1; ++__f1, (void)++__o) *__o = *__f1;
    set_difference_result<_I1, _Out> __r = { __f1, __o };
    return __r;
}
_CCW_RNG_SETOP(set_difference, set_difference_result<_CCW_RNG_IT(_R1) _CCW_RNG_C _Out>)

template <class _I1, class _S1, class _I2, class _S2, class _Out, class _Comp = less,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI
_CCW_RNG_IFN(_I1, set_symmetric_difference_result<_I1 _CCW_RNG_C _I2 _CCW_RNG_C _Out>)
set_symmetric_difference(_I1 __f1, _S1 __l1, _I2 __f2, _S2 __l2, _Out __o, _Comp __c = _Comp(),
                         _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    while (__f1 != __l1 && __f2 != __l2) {
        if (__c(ranges::__ccw_proj(__p1, *__f1), ranges::__ccw_proj(__p2, *__f2))) { *__o = *__f1; ++__o; ++__f1; }
        else if (__c(ranges::__ccw_proj(__p2, *__f2), ranges::__ccw_proj(__p1, *__f1))) { *__o = *__f2; ++__o; ++__f2; }
        else { ++__f1; ++__f2; }
    }
    for (; __f1 != __l1; ++__f1, (void)++__o) *__o = *__f1;
    for (; __f2 != __l2; ++__f2, (void)++__o) *__o = *__f2;
    set_symmetric_difference_result<_I1, _I2, _Out> __r = { __f1, __f2, __o };
    return __r;
}
_CCW_RNG_SETOP(set_symmetric_difference,
               set_symmetric_difference_result<_CCW_RNG_IT(_R1) _CCW_RNG_C _CCW_RNG_IT(_R2) _CCW_RNG_C _Out>)

#undef _CCW_RNG_SETOP

template <class _It, class _Sent, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
make_heap(_It __f, _Sent __l, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    _CCW_STD::make_heap(__f, __l, ranges::__ccw_mkcomp(__c, __pr));
    return __l;
}
_CCW_RNG_RANGE_A0(make_heap, less, _CCW_RNG_IT(_Rp))

template <class _It, class _Sent, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
push_heap(_It __f, _Sent __l, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    _CCW_STD::push_heap(__f, __l, ranges::__ccw_mkcomp(__c, __pr));
    return __l;
}
_CCW_RNG_RANGE_A0(push_heap, less, _CCW_RNG_IT(_Rp))

template <class _It, class _Sent, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
pop_heap(_It __f, _Sent __l, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    _CCW_STD::pop_heap(__f, __l, ranges::__ccw_mkcomp(__c, __pr));
    return __l;
}
_CCW_RNG_RANGE_A0(pop_heap, less, _CCW_RNG_IT(_Rp))

template <class _It, class _Sent, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
sort_heap(_It __f, _Sent __l, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    _CCW_STD::sort_heap(__f, __l, ranges::__ccw_mkcomp(__c, __pr));
    return __l;
}
_CCW_RNG_RANGE_A0(sort_heap, less, _CCW_RNG_IT(_Rp))

template <class _It, class _Sent, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, bool)
is_heap(_It __f, _Sent __l, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    return _CCW_STD::is_heap(__f, __l, ranges::__ccw_mkcomp(__c, __pr));
}
_CCW_RNG_RANGE_A0(is_heap, less, bool)

template <class _It, class _Sent, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
is_heap_until(_It __f, _Sent __l, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    return _CCW_STD::is_heap_until(__f, __l, ranges::__ccw_mkcomp(__c, __pr));
}
_CCW_RNG_RANGE_A0(is_heap_until, less, _CCW_RNG_IT(_Rp))

} // namespace ranges
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_HAS_ALIAS_TEMPLATE && vc14+
#endif // _CCW_LIBCPP___ALGORITHM_RANGES_SETOPS_H
