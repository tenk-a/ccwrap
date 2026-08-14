//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___ALGORITHM_PARTITION_COPY_H
#define _CCW_LIBCPP___ALGORITHM_PARTITION_COPY_H
#include "../__config"
#include "../__utility/pair.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _InputIt, class _OutTrue, class _OutFalse, class _Pred>
_CCW_LIBCPP_HIDE_FROM_ABI pair<_OutTrue, _OutFalse> partition_copy(_InputIt __f, _InputIt __l, _OutTrue __ot, _OutFalse __of, _Pred __p) {
    for (; __f != __l; ++__f) { if (__p(*__f)) { *__ot = *__f; ++__ot; } else { *__of = *__f; ++__of; } }
    return pair<_OutTrue, _OutFalse>(__ot, __of);
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
