/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ALGORITHM_SHUFFLE_H
#define _CCW_LIBCPP___ALGORITHM_SHUFFLE_H
#include "../__config"
#include "../__iterator/iterator_traits.h"
#include "../__utility/swap.h"
#include <cstdlib>
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _RandIt, class _URNG>
_CCW_LIBCPP_HIDE_FROM_ABI void shuffle(_RandIt __first, _RandIt __last, _URNG& __g) {
    typedef typename iterator_traits<_RandIt>::difference_type _Dp;
    _Dp __n = __last - __first;
    for (_Dp __i = __n - 1; __i > 0; --__i) {
        _Dp __j = (_Dp)(__g() % (typename _URNG::result_type)(__i + 1));
        if (__j != __i) _CCW_STD::swap(__first[__i], __first[__j]);
    }
}

template <class _RandIt>
_CCW_LIBCPP_HIDE_FROM_ABI void random_shuffle(_RandIt __first, _RandIt __last) {
    typedef typename iterator_traits<_RandIt>::difference_type _Dp;
    _Dp __n = __last - __first;
    for (_Dp __i = __n - 1; __i > 0; --__i) {
        _Dp __j = (_Dp)(_CCW_STD::rand() % (int)(__i + 1));
        if (__j != __i) _CCW_STD::swap(__first[__i], __first[__j]);
    }
}
template <class _RandIt, class _Rand>
_CCW_LIBCPP_HIDE_FROM_ABI void random_shuffle(_RandIt __first, _RandIt __last, _Rand& __r) {
    typedef typename iterator_traits<_RandIt>::difference_type _Dp;
    _Dp __n = __last - __first;
    for (_Dp __i = __n - 1; __i > 0; --__i) {
        _Dp __j = (_Dp)__r((int)(__i + 1));
        if (__j != __i) _CCW_STD::swap(__first[__i], __first[__j]);
    }
}

_CCW_LIBCPP_END_NAMESPACE_STD
#endif
