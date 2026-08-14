//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//  _CCW_STD::sample via selection sampling (population must be forward-or-better; the
//  output iterator can be any output iterator). URBG taken by lvalue reference.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_SAMPLE_H
#define _CCW_LIBCPP___ALGORITHM_SAMPLE_H
#include "../__config"
#include "../__iterator/distance.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _PopIt, class _SampleIt, class _Distance, class _URBG>
_CCW_LIBCPP_HIDE_FROM_ABI _SampleIt sample(_PopIt __first, _PopIt __last, _SampleIt __out_, _Distance __n, _URBG& __g) {
    _Distance __unsampled = static_cast<_Distance>(_CCW_STD::distance(__first, __last));
    if (__n > __unsampled) __n = __unsampled;
    for (; __n > 0 && __first != __last; ++__first) {
        if (static_cast<_Distance>(__g() % static_cast<typename _URBG::result_type>(__unsampled)) < __n) {
            *__out_ = *__first; ++__out_; --__n;
        }
        --__unsampled;
    }
    return __out_;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
