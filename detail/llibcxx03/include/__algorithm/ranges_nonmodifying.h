// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  __algorithm/ranges_nonmodifying.h -- the non-modifying ranges:: algorithms
//  ([alg.nonmodifying]), each in the iterator-pair and the range form.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_RANGES_NONMODIFYING_H
#define _CCW_LIBCPP___ALGORITHM_RANGES_NONMODIFYING_H

#include "../__config"

#if _CCW_HAS_ALIAS_TEMPLATE && (!defined(_MSC_VER) || _MSC_VER >= 1900)

#include "ranges_util.h"
#include <ranges>
#include "in_fun_result.h"
#include "in_in_result.h"

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
namespace ranges {

template <class _Ip, class _Fp> using for_each_result   = in_fun_result<_Ip, _Fp>;
template <class _Ip, class _Fp> using for_each_n_result = in_fun_result<_Ip, _Fp>;
template <class _I1, class _I2> using mismatch_result   = in_in_result<_I1, _I2>;

template <class _It, class _Sent, class _Pred, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, bool)
all_of(_It __f, _Sent __l, _Pred __p, _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f) { if (!__p(ranges::__ccw_proj(__pr, *__f))) return false; }
    return true;
}
_CCW_RNG_RANGE_A1(all_of, bool)

template <class _It, class _Sent, class _Pred, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, bool)
any_of(_It __f, _Sent __l, _Pred __p, _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f) { if (__p(ranges::__ccw_proj(__pr, *__f))) return true; }
    return false;
}
_CCW_RNG_RANGE_A1(any_of, bool)

template <class _It, class _Sent, class _Pred, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, bool)
none_of(_It __f, _Sent __l, _Pred __p, _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f) { if (__p(ranges::__ccw_proj(__pr, *__f))) return false; }
    return true;
}
_CCW_RNG_RANGE_A1(none_of, bool)

template <class _It, class _Sent, class _Fun, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, for_each_result<_It _CCW_RNG_C _Fun>)
for_each(_It __f, _Sent __l, _Fun __fn, _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f) __fn(ranges::__ccw_proj(__pr, *__f));
    for_each_result<_It, _Fun> __r = { __f, __fn };
    return __r;
}
_CCW_RNG_RANGE_A1(for_each, for_each_result<_CCW_RNG_IT(_Rp) _CCW_RNG_C _A0>)

template <class _It, class _Diff, class _Fun, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI for_each_n_result<_It, _Fun>
for_each_n(_It __f, _Diff __n, _Fun __fn, _Proj __pr = _Proj()) {
    for (; __n > 0; --__n, (void)++__f) __fn(ranges::__ccw_proj(__pr, *__f));
    for_each_n_result<_It, _Fun> __r = { __f, __fn };
    return __r;
}

template <class _It, class _Sent, class _Vp, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
find(_It __f, _Sent __l, const _Vp& __v, _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f) { if (ranges::__ccw_proj(__pr, *__f) == __v) break; }
    return __f;
}
_CCW_RNG_RANGE_A1(find, _CCW_RNG_IT(_Rp))

template <class _It, class _Sent, class _Pred, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
find_if(_It __f, _Sent __l, _Pred __p, _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f) { if (__p(ranges::__ccw_proj(__pr, *__f))) break; }
    return __f;
}
_CCW_RNG_RANGE_A1(find_if, _CCW_RNG_IT(_Rp))

template <class _It, class _Sent, class _Pred, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
find_if_not(_It __f, _Sent __l, _Pred __p, _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f) { if (!__p(ranges::__ccw_proj(__pr, *__f))) break; }
    return __f;
}
_CCW_RNG_RANGE_A1(find_if_not, _CCW_RNG_IT(_Rp))

template <class _It, class _Sent, class _Pred = equal_to, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
adjacent_find(_It __f, _Sent __l, _Pred __p = _Pred(), _Proj __pr = _Proj()) {
    if (__f == __l) return __f;
    _It __n = __f;
    for (++__n; __n != __l; __f = __n, (void)++__n) {
        if (__p(ranges::__ccw_proj(__pr, *__f), ranges::__ccw_proj(__pr, *__n))) return __f;
    }
    return __n;
}
_CCW_RNG_RANGE_A0(adjacent_find, equal_to, _CCW_RNG_IT(_Rp))

template <class _It, class _Sent, class _Vp, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, subrange<_It>)
find_last(_It __f, _Sent __l, const _Vp& __v, _Proj __pr = _Proj()) {
    _It __best = __l;
    for (_It __i = __f; __i != __l; ++__i) { if (ranges::__ccw_proj(__pr, *__i) == __v) __best = __i; }
    return subrange<_It>(__best, __l);
}
_CCW_RNG_RANGE_A1(find_last, subrange<_CCW_RNG_IT(_Rp)>)

template <class _It, class _Sent, class _Pred, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, subrange<_It>)
find_last_if(_It __f, _Sent __l, _Pred __p, _Proj __pr = _Proj()) {
    _It __best = __l;
    for (_It __i = __f; __i != __l; ++__i) { if (__p(ranges::__ccw_proj(__pr, *__i))) __best = __i; }
    return subrange<_It>(__best, __l);
}
_CCW_RNG_RANGE_A1(find_last_if, subrange<_CCW_RNG_IT(_Rp)>)

template <class _It, class _Sent, class _Pred, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, subrange<_It>)
find_last_if_not(_It __f, _Sent __l, _Pred __p, _Proj __pr = _Proj()) {
    _It __best = __l;
    for (_It __i = __f; __i != __l; ++__i) { if (!__p(ranges::__ccw_proj(__pr, *__i))) __best = __i; }
    return subrange<_It>(__best, __l);
}
_CCW_RNG_RANGE_A1(find_last_if_not, subrange<_CCW_RNG_IT(_Rp)>)

template <class _I1, class _S1, class _I2, class _S2, class _Pred = equal_to,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_I1, subrange<_I1>)
find_end(_I1 __f1, _S1 __l1, _I2 __f2, _S2 __l2, _Pred __p = _Pred(),
         _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    _I1 __best = __l1, __bend = __l1;
    if (__f2 == __l2) return subrange<_I1>(__l1, __l1);
    for (_I1 __i = __f1; __i != __l1; ++__i) {
        _I1 __a = __i;
        _I2 __b = __f2;
        for (; __b != __l2 && __a != __l1; ++__a, (void)++__b) {
            if (!__p(ranges::__ccw_proj(__p1, *__a), ranges::__ccw_proj(__p2, *__b))) break;
        }
        if (__b == __l2) { __best = __i; __bend = __a; }
    }
    return subrange<_I1>(__best, __bend);
}
_CCW_RNG_RANGE_2R(find_end, subrange<_CCW_RNG_IT(_R1)>)

template <class _I1, class _S1, class _I2, class _S2, class _Pred = equal_to,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_I1, _I1)
find_first_of(_I1 __f1, _S1 __l1, _I2 __f2, _S2 __l2, _Pred __p = _Pred(),
              _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    for (; __f1 != __l1; ++__f1) {
        for (_I2 __b = __f2; __b != __l2; ++__b) {
            if (__p(ranges::__ccw_proj(__p1, *__f1), ranges::__ccw_proj(__p2, *__b))) return __f1;
        }
    }
    return __f1;
}
_CCW_RNG_RANGE_2R(find_first_of, _CCW_RNG_IT(_R1))

template <class _It, class _Sent, class _Vp, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, typename _CCW_STD::iterator_traits<_It>::difference_type)
count(_It __f, _Sent __l, const _Vp& __v, _Proj __pr = _Proj()) {
    typename _CCW_STD::iterator_traits<_It>::difference_type __n = 0;
    for (; __f != __l; ++__f) { if (ranges::__ccw_proj(__pr, *__f) == __v) ++__n; }
    return __n;
}
_CCW_RNG_RANGE_A1(count, typename _CCW_STD::iterator_traits<_CCW_RNG_IT(_Rp)>::difference_type)

template <class _It, class _Sent, class _Pred, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, typename _CCW_STD::iterator_traits<_It>::difference_type)
count_if(_It __f, _Sent __l, _Pred __p, _Proj __pr = _Proj()) {
    typename _CCW_STD::iterator_traits<_It>::difference_type __n = 0;
    for (; __f != __l; ++__f) { if (__p(ranges::__ccw_proj(__pr, *__f))) ++__n; }
    return __n;
}
_CCW_RNG_RANGE_A1(count_if, typename _CCW_STD::iterator_traits<_CCW_RNG_IT(_Rp)>::difference_type)

template <class _I1, class _S1, class _I2, class _S2, class _Pred = equal_to,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_I1, mismatch_result<_I1 _CCW_RNG_C _I2>)
mismatch(_I1 __f1, _S1 __l1, _I2 __f2, _S2 __l2, _Pred __p = _Pred(),
         _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    for (; __f1 != __l1 && __f2 != __l2; ++__f1, (void)++__f2) {
        if (!__p(ranges::__ccw_proj(__p1, *__f1), ranges::__ccw_proj(__p2, *__f2))) break;
    }
    mismatch_result<_I1, _I2> __r = { __f1, __f2 };
    return __r;
}
_CCW_RNG_RANGE_2R(mismatch, mismatch_result<_CCW_RNG_IT(_R1) _CCW_RNG_C _CCW_RNG_IT(_R2)>)

template <class _I1, class _S1, class _I2, class _S2, class _Pred = equal_to,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_I1, bool)
equal(_I1 __f1, _S1 __l1, _I2 __f2, _S2 __l2, _Pred __p = _Pred(),
      _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    for (; __f1 != __l1 && __f2 != __l2; ++__f1, (void)++__f2) {
        if (!__p(ranges::__ccw_proj(__p1, *__f1), ranges::__ccw_proj(__p2, *__f2))) return false;
    }
    return __f1 == __l1 && __f2 == __l2;
}
_CCW_RNG_RANGE_2R(equal, bool)

template <class _I1, class _S1, class _I2, class _S2, class _Pred = equal_to,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_I1, bool)
is_permutation(_I1 __f1, _S1 __l1, _I2 __f2, _S2 __l2, _Pred __p = _Pred(),
               _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    if (ranges::__ccw_dist(__f1, __l1) != ranges::__ccw_dist(__f2, __l2)) return false;
    for (_I1 __i = __f1; __i != __l1; ++__i) {
        bool __seen = false;
        for (_I1 __j = __f1; __j != __i; ++__j) {
            if (__p(ranges::__ccw_proj(__p1, *__j), ranges::__ccw_proj(__p1, *__i))) { __seen = true; break; }
        }
        if (__seen) continue;
        long __c1 = 0, __c2 = 0;
        for (_I1 __j = __f1; __j != __l1; ++__j) {
            if (__p(ranges::__ccw_proj(__p1, *__j), ranges::__ccw_proj(__p1, *__i))) ++__c1;
        }
        for (_I2 __j = __f2; __j != __l2; ++__j) {
            if (__p(ranges::__ccw_proj(__p2, *__j), ranges::__ccw_proj(__p1, *__i))) ++__c2;
        }
        if (__c1 != __c2) return false;
    }
    return true;
}
_CCW_RNG_RANGE_2R(is_permutation, bool)

template <class _I1, class _S1, class _I2, class _S2, class _Pred = equal_to,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_I1, subrange<_I1>)
search(_I1 __f1, _S1 __l1, _I2 __f2, _S2 __l2, _Pred __p = _Pred(),
       _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    if (__f2 == __l2) return subrange<_I1>(__f1, __f1);
    for (_I1 __i = __f1; __i != __l1; ++__i) {
        _I1 __a = __i;
        _I2 __b = __f2;
        for (; __b != __l2 && __a != __l1; ++__a, (void)++__b) {
            if (!__p(ranges::__ccw_proj(__p1, *__a), ranges::__ccw_proj(__p2, *__b))) break;
        }
        if (__b == __l2) return subrange<_I1>(__i, __a);
    }
    return subrange<_I1>(__l1, __l1);
}
_CCW_RNG_RANGE_2R(search, subrange<_CCW_RNG_IT(_R1)>)

template <class _It, class _Sent, class _Diff, class _Vp, class _Pred = equal_to, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, subrange<_It>)
search_n(_It __f, _Sent __l, _Diff __n, const _Vp& __v, _Pred __p = _Pred(), _Proj __pr = _Proj()) {
    if (__n <= 0) return subrange<_It>(__f, __f);
    for (_It __i = __f; __i != __l; ++__i) {
        _It  __a = __i;
        _Diff __k = 0;
        for (; __k < __n && __a != __l; ++__a, (void)++__k) {
            if (!__p(ranges::__ccw_proj(__pr, *__a), __v)) break;
        }
        if (__k == __n) return subrange<_It>(__i, __a);
    }
    return subrange<_It>(__l, __l);
}
template <class _Rp, class _Diff, class _Vp, class _Pred = equal_to, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, subrange<_CCW_RNG_IT(_Rp)>)
search_n(_Rp&& __r, _Diff __n, const _Vp& __v, _Pred __p = _Pred(), _Proj __pr = _Proj()) {
    return ranges::search_n(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __n, __v, __p, __pr);
}

template <class _It, class _Sent, class _Vp, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, bool)
contains(_It __f, _Sent __l, const _Vp& __v, _Proj __pr = _Proj()) {
    return ranges::find(__f, __l, __v, __pr) != __l;
}
_CCW_RNG_RANGE_A1(contains, bool)

template <class _I1, class _S1, class _I2, class _S2, class _Pred = equal_to,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_I1, bool)
contains_subrange(_I1 __f1, _S1 __l1, _I2 __f2, _S2 __l2, _Pred __p = _Pred(),
                  _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    if (__f2 == __l2) return true;
    return !ranges::search(__f1, __l1, __f2, __l2, __p, __p1, __p2).empty();
}
_CCW_RNG_RANGE_2R(contains_subrange, bool)

template <class _I1, class _S1, class _I2, class _S2, class _Pred = equal_to,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_I1, bool)
starts_with(_I1 __f1, _S1 __l1, _I2 __f2, _S2 __l2, _Pred __p = _Pred(),
            _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    for (; __f2 != __l2; ++__f1, (void)++__f2) {
        if (__f1 == __l1) return false;
        if (!__p(ranges::__ccw_proj(__p1, *__f1), ranges::__ccw_proj(__p2, *__f2))) return false;
    }
    return true;
}
_CCW_RNG_RANGE_2R(starts_with, bool)

template <class _I1, class _S1, class _I2, class _S2, class _Pred = equal_to,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_I1, bool)
ends_with(_I1 __f1, _S1 __l1, _I2 __f2, _S2 __l2, _Pred __p = _Pred(),
          _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    typename _CCW_STD::iterator_traits<_I1>::difference_type __n1 = ranges::__ccw_dist(__f1, __l1);
    typename _CCW_STD::iterator_traits<_I2>::difference_type __n2 = ranges::__ccw_dist(__f2, __l2);
    if (__n1 < __n2) return false;
    for (; __n1 > __n2; --__n1) ++__f1;
    return ranges::equal(__f1, __l1, __f2, __l2, __p, __p1, __p2);
}
_CCW_RNG_RANGE_2R(ends_with, bool)

} // namespace ranges
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_HAS_ALIAS_TEMPLATE && vc14+
#endif // _CCW_LIBCPP___ALGORITHM_RANGES_NONMODIFYING_H
