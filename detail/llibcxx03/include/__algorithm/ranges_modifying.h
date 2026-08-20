// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  __algorithm/ranges_modifying.h -- the mutating ranges:: algorithms
//  ([alg.modifying.operations]): copy .. swap_ranges.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_RANGES_MODIFYING_H
#define _CCW_LIBCPP___ALGORITHM_RANGES_MODIFYING_H

#include "../__config"

#if _CCW_HAS_ALIAS_TEMPLATE && (!defined(_MSC_VER) || _MSC_VER >= 1900)

#include "ranges_util.h"
#include "ranges_nonmodifying.h"
#include "in_out_result.h"
#include "in_in_out_result.h"
#include "../__utility/move.h"

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
namespace ranges {

template <class _Ip, class _Op> using copy_result          = in_out_result<_Ip, _Op>;
template <class _Ip, class _Op> using copy_if_result       = in_out_result<_Ip, _Op>;
template <class _Ip, class _Op> using copy_n_result        = in_out_result<_Ip, _Op>;
template <class _Ip, class _Op> using copy_backward_result = in_out_result<_Ip, _Op>;
template <class _Ip, class _Op> using move_result          = in_out_result<_Ip, _Op>;
template <class _Ip, class _Op> using move_backward_result = in_out_result<_Ip, _Op>;
template <class _I1, class _I2> using swap_ranges_result   = in_in_result<_I1, _I2>;

template <class _It, class _Sent, class _Out>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, copy_result<_It _CCW_RNG_C _Out>)
copy(_It __f, _Sent __l, _Out __o) {
    for (; __f != __l; ++__f, (void)++__o) *__o = *__f;
    copy_result<_It, _Out> __r = { __f, __o };
    return __r;
}
template <class _Rp, class _Out>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, copy_result<_CCW_RNG_IT(_Rp) _CCW_RNG_C _Out>)
copy(_Rp&& __r, _Out __o) {
    return ranges::copy(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __o);
}

template <class _It, class _Sent, class _Out, class _Pred, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, copy_if_result<_It _CCW_RNG_C _Out>)
copy_if(_It __f, _Sent __l, _Out __o, _Pred __p, _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f) {
        if (__p(ranges::__ccw_proj(__pr, *__f))) { *__o = *__f; ++__o; }
    }
    copy_if_result<_It, _Out> __r = { __f, __o };
    return __r;
}
template <class _Rp, class _Out, class _Pred, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, copy_if_result<_CCW_RNG_IT(_Rp) _CCW_RNG_C _Out>)
copy_if(_Rp&& __r, _Out __o, _Pred __p, _Proj __pr = _Proj()) {
    return ranges::copy_if(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __o, __p, __pr);
}

template <class _It, class _Diff, class _Out>
_CCW_LIBCPP_HIDE_FROM_ABI copy_n_result<_It, _Out> copy_n(_It __f, _Diff __n, _Out __o) {
    for (; __n > 0; --__n, (void)++__f, (void)++__o) *__o = *__f;
    copy_n_result<_It, _Out> __r = { __f, __o };
    return __r;
}

template <class _It, class _Sent, class _Out>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, copy_backward_result<_It _CCW_RNG_C _Out>)
copy_backward(_It __f, _Sent __l, _Out __o) {
    _It __e = __l;
    while (__e != __f) { --__e; --__o; *__o = *__e; }
    copy_backward_result<_It, _Out> __r = { __l, __o };
    return __r;
}
template <class _Rp, class _Out>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, copy_backward_result<_CCW_RNG_IT(_Rp) _CCW_RNG_C _Out>)
copy_backward(_Rp&& __r, _Out __o) {
    return ranges::copy_backward(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __o);
}

template <class _It, class _Sent, class _Out>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, move_result<_It _CCW_RNG_C _Out>)
move(_It __f, _Sent __l, _Out __o) {
    for (; __f != __l; ++__f, (void)++__o) *__o = ::_ccw::move(*__f);
    move_result<_It, _Out> __r = { __f, __o };
    return __r;
}
template <class _Rp, class _Out>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, move_result<_CCW_RNG_IT(_Rp) _CCW_RNG_C _Out>)
move(_Rp&& __r, _Out __o) {
    return ranges::move(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __o);
}

template <class _It, class _Sent, class _Out>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, move_backward_result<_It _CCW_RNG_C _Out>)
move_backward(_It __f, _Sent __l, _Out __o) {
    _It __e = __l;
    while (__e != __f) { --__e; --__o; *__o = ::_ccw::move(*__e); }
    move_backward_result<_It, _Out> __r = { __l, __o };
    return __r;
}
template <class _Rp, class _Out>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, move_backward_result<_CCW_RNG_IT(_Rp) _CCW_RNG_C _Out>)
move_backward(_Rp&& __r, _Out __o) {
    return ranges::move_backward(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __o);
}

template <class _I1, class _S1, class _I2, class _S2>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_I1, swap_ranges_result<_I1 _CCW_RNG_C _I2>)
swap_ranges(_I1 __f1, _S1 __l1, _I2 __f2, _S2 __l2) {
    for (; __f1 != __l1 && __f2 != __l2; ++__f1, (void)++__f2) {
        typename _CCW_STD::iterator_traits<_I1>::value_type __t = *__f1;
        *__f1 = *__f2;
        *__f2 = __t;
    }
    swap_ranges_result<_I1, _I2> __r = { __f1, __f2 };
    return __r;
}
template <class _R1, class _R2>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_R1, swap_ranges_result<_CCW_RNG_IT(_R1) _CCW_RNG_C _CCW_RNG_IT(_R2)>)
swap_ranges(_R1&& __r1, _R2&& __r2) {
    return ranges::swap_ranges(ranges::__ccw_begin(__r1), ranges::__ccw_end(__r1),
                               ranges::__ccw_begin(__r2), ranges::__ccw_end(__r2));
}

template <class _Ip, class _Op> using unary_transform_result  = in_out_result<_Ip, _Op>;
template <class _I1, class _I2, class _Op> using binary_transform_result = in_in_out_result<_I1, _I2, _Op>;
template <class _Ip, class _Op> using replace_copy_result    = in_out_result<_Ip, _Op>;
template <class _Ip, class _Op> using replace_copy_if_result = in_out_result<_Ip, _Op>;

template <class _It, class _Sent, class _Out, class _Fun, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, unary_transform_result<_It _CCW_RNG_C _Out>)
transform(_It __f, _Sent __l, _Out __o, _Fun __fn, _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f, (void)++__o) *__o = __fn(ranges::__ccw_proj(__pr, *__f));
    unary_transform_result<_It, _Out> __r = { __f, __o };
    return __r;
}
template <class _Rp, class _Out, class _Fun, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF1N(_Rp, _Out, unary_transform_result<_CCW_RNG_IT(_Rp) _CCW_RNG_C _Out>)
transform(_Rp&& __r, _Out __o, _Fun __fn, _Proj __pr = _Proj()) {
    return ranges::transform(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __o, __fn, __pr);
}
template <class _I1, class _S1, class _I2, class _S2, class _Out, class _Fun,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_I1, binary_transform_result<_I1 _CCW_RNG_C _I2 _CCW_RNG_C _Out>)
transform(_I1 __f1, _S1 __l1, _I2 __f2, _S2 __l2, _Out __o, _Fun __fn,
          _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    for (; __f1 != __l1 && __f2 != __l2; ++__f1, (void)++__f2, (void)++__o) {
        *__o = __fn(ranges::__ccw_proj(__p1, *__f1), ranges::__ccw_proj(__p2, *__f2));
    }
    binary_transform_result<_I1, _I2, _Out> __r = { __f1, __f2, __o };
    return __r;
}
template <class _R1, class _R2, class _Out, class _Fun,
          class _P1 = identity, class _P2 = identity>
_CCW_LIBCPP_HIDE_FROM_ABI
_CCW_RNG_IF(_R1, binary_transform_result<_CCW_RNG_IT(_R1) _CCW_RNG_C _CCW_RNG_IT(_R2) _CCW_RNG_C _Out>)
transform(_R1&& __r1, _R2&& __r2, _Out __o, _Fun __fn, _P1 __p1 = _P1(), _P2 __p2 = _P2()) {
    return ranges::transform(ranges::__ccw_begin(__r1), ranges::__ccw_end(__r1),
                             ranges::__ccw_begin(__r2), ranges::__ccw_end(__r2), __o, __fn, __p1, __p2);
}

template <class _It, class _Sent, class _Vp, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
replace(_It __f, _Sent __l, const _Vp& __old, const _Vp& __new, _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f) { if (ranges::__ccw_proj(__pr, *__f) == __old) *__f = __new; }
    return __f;
}
template <class _Rp, class _Vp, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, _CCW_RNG_IT(_Rp))
replace(_Rp&& __r, const _Vp& __old, const _Vp& __new, _Proj __pr = _Proj()) {
    return ranges::replace(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __old, __new, __pr);
}

template <class _It, class _Sent, class _Pred, class _Vp, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It)
replace_if(_It __f, _Sent __l, _Pred __p, const _Vp& __new, _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f) { if (__p(ranges::__ccw_proj(__pr, *__f))) *__f = __new; }
    return __f;
}
template <class _Rp, class _Pred, class _Vp, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, _CCW_RNG_IT(_Rp))
replace_if(_Rp&& __r, _Pred __p, const _Vp& __new, _Proj __pr = _Proj()) {
    return ranges::replace_if(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __p, __new, __pr);
}

template <class _It, class _Sent, class _Out, class _Vp, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, replace_copy_result<_It _CCW_RNG_C _Out>)
replace_copy(_It __f, _Sent __l, _Out __o, const _Vp& __old, const _Vp& __new, _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f, (void)++__o) {
        *__o = (ranges::__ccw_proj(__pr, *__f) == __old) ? __new : *__f;
    }
    replace_copy_result<_It, _Out> __r = { __f, __o };
    return __r;
}
template <class _Rp, class _Out, class _Vp, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, replace_copy_result<_CCW_RNG_IT(_Rp) _CCW_RNG_C _Out>)
replace_copy(_Rp&& __r, _Out __o, const _Vp& __old, const _Vp& __new, _Proj __pr = _Proj()) {
    return ranges::replace_copy(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __o, __old, __new, __pr);
}

template <class _It, class _Sent, class _Out, class _Pred, class _Vp, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, replace_copy_if_result<_It _CCW_RNG_C _Out>)
replace_copy_if(_It __f, _Sent __l, _Out __o, _Pred __p, const _Vp& __new, _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f, (void)++__o) {
        *__o = __p(ranges::__ccw_proj(__pr, *__f)) ? __new : *__f;
    }
    replace_copy_if_result<_It, _Out> __r = { __f, __o };
    return __r;
}
template <class _Rp, class _Out, class _Pred, class _Vp, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, replace_copy_if_result<_CCW_RNG_IT(_Rp) _CCW_RNG_C _Out>)
replace_copy_if(_Rp&& __r, _Out __o, _Pred __p, const _Vp& __new, _Proj __pr = _Proj()) {
    return ranges::replace_copy_if(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __o, __p, __new, __pr);
}

template <class _It, class _Sent, class _Vp>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It) fill(_It __f, _Sent __l, const _Vp& __v) {
    for (; __f != __l; ++__f) *__f = __v;
    return __f;
}
template <class _Rp, class _Vp>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, _CCW_RNG_IT(_Rp)) fill(_Rp&& __r, const _Vp& __v) {
    return ranges::fill(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __v);
}
template <class _Out, class _Diff, class _Vp>
_CCW_LIBCPP_HIDE_FROM_ABI _Out fill_n(_Out __o, _Diff __n, const _Vp& __v) {
    for (; __n > 0; --__n, (void)++__o) *__o = __v;
    return __o;
}

template <class _It, class _Sent, class _Fun>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It) generate(_It __f, _Sent __l, _Fun __fn) {
    for (; __f != __l; ++__f) *__f = __fn();
    return __f;
}
template <class _Rp, class _Fun>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, _CCW_RNG_IT(_Rp)) generate(_Rp&& __r, _Fun __fn) {
    return ranges::generate(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __fn);
}
template <class _Out, class _Diff, class _Fun>
_CCW_LIBCPP_HIDE_FROM_ABI _Out generate_n(_Out __o, _Diff __n, _Fun __fn) {
    for (; __n > 0; --__n, (void)++__o) *__o = __fn();
    return __o;
}

template <class _Ip, class _Op> using remove_copy_result    = in_out_result<_Ip, _Op>;
template <class _Ip, class _Op> using remove_copy_if_result = in_out_result<_Ip, _Op>;
template <class _Ip, class _Op> using unique_copy_result    = in_out_result<_Ip, _Op>;
template <class _Ip, class _Op> using reverse_copy_result   = in_out_result<_Ip, _Op>;
template <class _Ip, class _Op> using rotate_copy_result    = in_out_result<_Ip, _Op>;

template <class _It, class _Sent, class _Vp, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, subrange<_It>)
remove(_It __f, _Sent __l, const _Vp& __v, _Proj __pr = _Proj()) {
    _It __w = __f;
    for (_It __i = __f; __i != __l; ++__i) {
        if (!(ranges::__ccw_proj(__pr, *__i) == __v)) { *__w = ::_ccw::move(*__i); ++__w; }
    }
    return subrange<_It>(__w, __l);
}
_CCW_RNG_RANGE_A1(remove, subrange<_CCW_RNG_IT(_Rp)>)

template <class _It, class _Sent, class _Pred, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, subrange<_It>)
remove_if(_It __f, _Sent __l, _Pred __p, _Proj __pr = _Proj()) {
    _It __w = __f;
    for (_It __i = __f; __i != __l; ++__i) {
        if (!__p(ranges::__ccw_proj(__pr, *__i))) { *__w = ::_ccw::move(*__i); ++__w; }
    }
    return subrange<_It>(__w, __l);
}
_CCW_RNG_RANGE_A1(remove_if, subrange<_CCW_RNG_IT(_Rp)>)

template <class _It, class _Sent, class _Out, class _Vp, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, remove_copy_result<_It _CCW_RNG_C _Out>)
remove_copy(_It __f, _Sent __l, _Out __o, const _Vp& __v, _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f) {
        if (!(ranges::__ccw_proj(__pr, *__f) == __v)) { *__o = *__f; ++__o; }
    }
    remove_copy_result<_It, _Out> __r = { __f, __o };
    return __r;
}
template <class _Rp, class _Out, class _Vp, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, remove_copy_result<_CCW_RNG_IT(_Rp) _CCW_RNG_C _Out>)
remove_copy(_Rp&& __r, _Out __o, const _Vp& __v, _Proj __pr = _Proj()) {
    return ranges::remove_copy(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __o, __v, __pr);
}

template <class _It, class _Sent, class _Out, class _Pred, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, remove_copy_if_result<_It _CCW_RNG_C _Out>)
remove_copy_if(_It __f, _Sent __l, _Out __o, _Pred __p, _Proj __pr = _Proj()) {
    for (; __f != __l; ++__f) {
        if (!__p(ranges::__ccw_proj(__pr, *__f))) { *__o = *__f; ++__o; }
    }
    remove_copy_if_result<_It, _Out> __r = { __f, __o };
    return __r;
}
template <class _Rp, class _Out, class _Pred, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, remove_copy_if_result<_CCW_RNG_IT(_Rp) _CCW_RNG_C _Out>)
remove_copy_if(_Rp&& __r, _Out __o, _Pred __p, _Proj __pr = _Proj()) {
    return ranges::remove_copy_if(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __o, __p, __pr);
}

template <class _It, class _Sent, class _Pred = equal_to, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, subrange<_It>)
unique(_It __f, _Sent __l, _Pred __p = _Pred(), _Proj __pr = _Proj()) {
    if (__f == __l) return subrange<_It>(__f, __l);
    _It __w = __f;
    _It __i = __f;
    for (++__i; __i != __l; ++__i) {
        if (!__p(ranges::__ccw_proj(__pr, *__w), ranges::__ccw_proj(__pr, *__i))) {
            ++__w;
            if (__w != __i) *__w = ::_ccw::move(*__i);
        }
    }
    ++__w;
    return subrange<_It>(__w, __l);
}
_CCW_RNG_RANGE_A0(unique, equal_to, subrange<_CCW_RNG_IT(_Rp)>)

template <class _It, class _Sent, class _Out, class _Pred = equal_to, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, unique_copy_result<_It _CCW_RNG_C _Out>)
unique_copy(_It __f, _Sent __l, _Out __o, _Pred __p = _Pred(), _Proj __pr = _Proj()) {
    if (__f != __l) {
        typename _CCW_STD::iterator_traits<_It>::value_type __prev = *__f;
        *__o = __prev; ++__o;
        for (++__f; __f != __l; ++__f) {
            if (!__p(ranges::__ccw_proj(__pr, __prev), ranges::__ccw_proj(__pr, *__f))) {
                __prev = *__f;
                *__o = __prev; ++__o;
            }
        }
    }
    unique_copy_result<_It, _Out> __r = { __f, __o };
    return __r;
}
template <class _Rp, class _Out, class _Pred = equal_to, class _Proj = identity>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF1N(_Rp, _Out, unique_copy_result<_CCW_RNG_IT(_Rp) _CCW_RNG_C _Out>)
unique_copy(_Rp&& __r, _Out __o, _Pred __p = _Pred(), _Proj __pr = _Proj()) {
    return ranges::unique_copy(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __o, __p, __pr);
}

template <class _It, class _Sent>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, _It) reverse(_It __f, _Sent __l) {
    _It __e = __l;
    while (__f != __e && __f != --__e) {
        typename _CCW_STD::iterator_traits<_It>::value_type __t = *__f;
        *__f = *__e;
        *__e = __t;
        ++__f;
    }
    return __l;
}
template <class _Rp>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, _CCW_RNG_IT(_Rp)) reverse(_Rp&& __r) {
    return ranges::reverse(ranges::__ccw_begin(__r), ranges::__ccw_end(__r));
}

template <class _It, class _Sent, class _Out>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, reverse_copy_result<_It _CCW_RNG_C _Out>)
reverse_copy(_It __f, _Sent __l, _Out __o) {
    _It __e = __l;
    while (__e != __f) { --__e; *__o = *__e; ++__o; }
    reverse_copy_result<_It, _Out> __r = { __l, __o };
    return __r;
}
template <class _Rp, class _Out>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, reverse_copy_result<_CCW_RNG_IT(_Rp) _CCW_RNG_C _Out>)
reverse_copy(_Rp&& __r, _Out __o) {
    return ranges::reverse_copy(ranges::__ccw_begin(__r), ranges::__ccw_end(__r), __o);
}

template <class _It, class _Sent>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, subrange<_It>)
rotate(_It __f, _It __m, _Sent __l) {
    if (__f == __m) return subrange<_It>(__l, __l);
    if (__m == __l) return subrange<_It>(__f, __l);
    _It __ret = __f;
    for (_It __i = __m; __i != __l; ++__i) ++__ret;
    _It __nxt = __m;
    _It __w   = __f;
    while (__nxt != __l) {
        typename _CCW_STD::iterator_traits<_It>::value_type __t = *__w;
        *__w = *__nxt;
        *__nxt = __t;
        ++__w; ++__nxt;
        if (__w == __m) __m = __nxt;
        else if (__nxt == __l) __nxt = __m;
    }
    return subrange<_It>(__ret, __l);
}
template <class _Rp, class _It>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, subrange<_CCW_RNG_IT(_Rp)>)
rotate(_Rp&& __r, _It __m) {
    return ranges::rotate(ranges::__ccw_begin(__r), __m, ranges::__ccw_end(__r));
}

template <class _It, class _Sent, class _Out>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IFN(_It, rotate_copy_result<_It _CCW_RNG_C _Out>)
rotate_copy(_It __f, _It __m, _Sent __l, _Out __o) {
    for (_It __i = __m; __i != __l; ++__i, (void)++__o) *__o = *__i;
    for (_It __i = __f; __i != __m; ++__i, (void)++__o) *__o = *__i;
    rotate_copy_result<_It, _Out> __r = { __l, __o };
    return __r;
}
template <class _Rp, class _It, class _Out>
_CCW_LIBCPP_HIDE_FROM_ABI _CCW_RNG_IF(_Rp, rotate_copy_result<_CCW_RNG_IT(_Rp) _CCW_RNG_C _Out>)
rotate_copy(_Rp&& __r, _It __m, _Out __o) {
    return ranges::rotate_copy(ranges::__ccw_begin(__r), __m, ranges::__ccw_end(__r), __o);
}

} // namespace ranges
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_HAS_ALIAS_TEMPLATE && vc14+
#endif // _CCW_LIBCPP___ALGORITHM_RANGES_MODIFYING_H
