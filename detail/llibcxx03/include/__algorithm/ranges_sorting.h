// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  __algorithm/ranges_sorting.h -- the partitioning and sorting ranges::
//  algorithms ([alg.sorting]): is_partitioned .. nth_element.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_RANGES_SORTING_H
#define _CCW_LIBCPP___ALGORITHM_RANGES_SORTING_H

#include "../__config"

#if _CCW_HAS_ALIAS_TEMPLATE && (!defined(_MSC_VER) || _MSC_VER >= 1900)

#include "ranges_util.h"
#include "ranges_modifying.h"
#include "in_out_out_result.h"
#include "in_out_result.h"
#include <algorithm>
#include <vector>

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
namespace ranges {

template <class _Ip, class _O1, class _O2> using partition_copy_result   = in_out_out_result<_Ip, _O1, _O2>;
template <class _Ip, class _Op>            using partial_sort_copy_result = in_out_result<_Ip, _Op>;

template <class _Comp, class _Proj>
struct __ccw_pcomp {
    _Comp __c_;
    _Proj __p_;
    _CCW_LIBCPP_HIDE_FROM_ABI __ccw_pcomp(_Comp __c, _Proj __p) : __c_(__c), __p_(__p) {}
    template <class _Tp, class _Up>
    _CCW_LIBCPP_HIDE_FROM_ABI bool operator()(const _Tp& __a, const _Up& __b) {
        return __c_(ranges::__ccw_proj(__p_, const_cast<_Tp&>(__a)),
                    ranges::__ccw_proj(__p_, const_cast<_Up&>(__b)));
    }
};
template <class _Comp, class _Proj>
_CCW_LIBCPP_HIDE_FROM_ABI inline __ccw_pcomp<_Comp, _Proj> __ccw_mkcomp(_Comp __c, _Proj __p) {
    return __ccw_pcomp<_Comp, _Proj>(__c, __p);
}

template <class _It, class _Sent, class _Pred, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, bool)
is_partitioned(_It __f, _Sent __l, _Pred __p, _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f) { if (!__p(ranges::__ccw_proj(__pr, *__f))) break; }
    for (; __f != __l; ++__f) { if (__p(ranges::__ccw_proj(__pr, *__f))) return false; }
    return true;
}
_CCW_RNG_RANGE_A1(is_partitioned, bool)

template <class _It, class _Sent, class _Pred, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, subrange<_It>)
partition(_It __f, _Sent __l, _Pred __p, _Proj __pr = _Proj()) {
    _It __w = __f;
    for (_It __i = __f; __i != __l; ++__i) {
        if (__p(ranges::__ccw_proj(__pr, *__i))) {
            typename _CCW_STD::iterator_traits<_It>::value_type __t = *__w;
            *__w = *__i;
            *__i = __t;
            ++__w;
        }
    }
    return subrange<_It>(__w, __l);
}
_CCW_RNG_RANGE_A1(partition, subrange<_CCW_RNG_IT(_Rp)>)

template <class _It, class _Sent, class _Pred, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, subrange<_It>)
stable_partition(_It __f, _Sent __l, _Pred __p, _Proj __pr = _Proj()) {
    typedef typename _CCW_STD::iterator_traits<_It>::value_type _Vt;
    _CCW_STD::vector<_Vt> __yes, __no;
    for (_It __i = __f; __i != __l; ++__i) {
        if (__p(ranges::__ccw_proj(__pr, *__i))) __yes.push_back(*__i);
        else                                     __no.push_back(*__i);
    }
    _It __w = __f;
    for (typename _CCW_STD::vector<_Vt>::iterator __i = __yes.begin(); __i != __yes.end(); ++__i, (void)++__w) *__w = *__i;
    _It __mid = __w;
    for (typename _CCW_STD::vector<_Vt>::iterator __i = __no.begin(); __i != __no.end(); ++__i, (void)++__w) *__w = *__i;
    return subrange<_It>(__mid, __l);
}
_CCW_RNG_RANGE_A1(stable_partition, subrange<_CCW_RNG_IT(_Rp)>)

template <class _It, class _Sent, class _O1, class _O2, class _Pred, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, partition_copy_result<_It _CCW_RNG_C _O1 _CCW_RNG_C _O2>)
partition_copy(_It __f, _Sent __l, _O1 __o1, _O2 __o2, _Pred __p, _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f) {
        if (__p(ranges::__ccw_proj(__pr, *__f))) { *__o1 = *__f; ++__o1; }
        else                                     { *__o2 = *__f; ++__o2; }
    }
    partition_copy_result<_It, _O1, _O2> __r = { __f, __o1, __o2 };
    return __r;
}
template <class _Rp, class _O1, class _O2, class _Pred, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI
_CCW_RNG_IF(_Rp, partition_copy_result<_CCW_RNG_IT(_Rp) _CCW_RNG_C _O1 _CCW_RNG_C _O2>)
partition_copy(_Rp&& __r, _O1 __o1, _O2 __o2, _Pred __p, _Proj __pr = _Proj()) {
    return ranges::partition_copy(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __o1, __o2, __p, __pr);
}

template <class _It, class _Sent, class _Pred, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
partition_point(_It __f, _Sent __l, _Pred __p, _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f) { if (!__p(ranges::__ccw_proj(__pr, *__f))) break; }
    return __f;
}
_CCW_RNG_RANGE_A1(partition_point, _CCW_RNG_IT(_Rp))

template <class _It, class _Sent, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
sort(_It __f, _Sent __l, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    _CCW_STD::sort(__f, __l, ranges::__ccw_mkcomp(__c, __pr));
    return __l;
}
_CCW_RNG_RANGE_A0(sort, less, _CCW_RNG_IT(_Rp))

template <class _It, class _Sent, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
stable_sort(_It __f, _Sent __l, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    _CCW_STD::stable_sort(__f, __l, ranges::__ccw_mkcomp(__c, __pr));
    return __l;
}
_CCW_RNG_RANGE_A0(stable_sort, less, _CCW_RNG_IT(_Rp))

template <class _It, class _Sent, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, bool)
is_sorted(_It __f, _Sent __l, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    if (__f == __l) return true;
    _It __n = __f;
    for (++__n; __n != __l; __f = __n, (void)++__n) {
        if (__c(ranges::__ccw_proj(__pr, *__n), ranges::__ccw_proj(__pr, *__f))) return false;
    }
    return true;
}
_CCW_RNG_RANGE_A0(is_sorted, less, bool)

template <class _It, class _Sent, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
is_sorted_until(_It __f, _Sent __l, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    if (__f == __l) return __f;
    _It __n = __f;
    for (++__n; __n != __l; __f = __n, (void)++__n) {
        if (__c(ranges::__ccw_proj(__pr, *__n), ranges::__ccw_proj(__pr, *__f))) return __n;
    }
    return __n;
}
_CCW_RNG_RANGE_A0(is_sorted_until, less, _CCW_RNG_IT(_Rp))

template <class _It, class _Sent, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
partial_sort(_It __f, _It __m, _Sent __l, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    _CCW_STD::partial_sort(__f, __m, __l, ranges::__ccw_mkcomp(__c, __pr));
    return __l;
}
template <class _Rp, class _It, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, _CCW_RNG_IT(_Rp))
partial_sort(_Rp&& __r, _It __m, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    return ranges::partial_sort(ranges::__ccw_begin(__r), __m, ranges::__ccw_end(__r), __c, __pr);
}

template <class _I1, class _S1, class _I2, class _S2, class _Comp = less,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_I1, partial_sort_copy_result<_I1 _CCW_RNG_C _I2>)
partial_sort_copy(_I1 __f1, _S1 __l1, _I2 __f2, _S2 __l2, _Comp __c = _Comp(),
                  _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    _I2 __o = __f2;
    for (; __f1 != __l1 && __o != __l2; ++__f1, (void)++__o) *__o = *__f1;
    _CCW_STD::sort(__f2, __o, ranges::__ccw_mkcomp(__c, __p2));
    for (; __f1 != __l1; ++__f1) {
        if (__c(ranges::__ccw_proj(__p1, *__f1), ranges::__ccw_proj(__p2, *(__o - 1)))) {
            *(__o - 1) = *__f1;
            _CCW_STD::sort(__f2, __o, ranges::__ccw_mkcomp(__c, __p2));
        }
    }
    partial_sort_copy_result<_I1, _I2> __r = { __f1, __o };
    return __r;
}
template <class _R1, class _R2, class _Comp = less, class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI
_CCW_RNG_IF(_R1, partial_sort_copy_result<_CCW_RNG_IT(_R1) _CCW_RNG_C _CCW_RNG_IT(_R2)>)
partial_sort_copy(_R1&& __r1, _R2&& __r2, _Comp __c = _Comp(), _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    return ranges::partial_sort_copy(ranges::__ccw_begin(__r1), ranges::__ccw_end(__r1),
                                     ranges::__ccw_begin(__r2), ranges::__ccw_end(__r2), __c, __p1, __p2);
}

template <class _It, class _Sent, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
nth_element(_It __f, _It __n, _Sent __l, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    _CCW_STD::nth_element(__f, __n, __l, ranges::__ccw_mkcomp(__c, __pr));
    return __l;
}
template <class _Rp, class _It, class _Comp = less, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, _CCW_RNG_IT(_Rp))
nth_element(_Rp&& __r, _It __n, _Comp __c = _Comp(), _Proj __pr = _Proj()) {
    return ranges::nth_element(ranges::__ccw_begin(__r), __n, ranges::__ccw_end(__r), __c, __pr);
}

} // namespace ranges
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_HAS_ALIAS_TEMPLATE && vc14+
#endif // _CCW_LIBCPP___ALGORITHM_RANGES_SORTING_H
