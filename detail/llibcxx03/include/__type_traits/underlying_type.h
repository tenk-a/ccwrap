/*
 * ccwrap: libc++ layout. classic C++03 body.
 *
 *  underlying_type (C++11) needs a compiler intrinsic. Open Watcom has none (bug
 *) and no substitute is known, so it is simply absent there -- that is
 *  the one trait in this directory that a target can lack.
 *
 *  Everywhere else the intrinsic exists: __underlying_type on GCC/Clang and on MSVC
 *  from vc11 (_MSC_VER 1700). vc8/vc9 have neither the intrinsic nor a native
 *  <type_traits>, so they fall back to "a signed integer of the same size" -- right for
 *  the ordinary int-backed enum, and bit-preserving for the cast to_underlying performs.
 */
#ifndef _CCW_LIBCPP___TYPE_TRAITS_UNDERLYING_TYPE_H
#define _CCW_LIBCPP___TYPE_TRAITS_UNDERLYING_TYPE_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/conditional.h"

#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS && !defined(__WATCOMC__)

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

#if defined(_MSC_VER) && _MSC_VER < 1700
template <class _Ep>
struct underlying_type {
    typedef typename conditional<sizeof(_Ep) == 1, signed char,
            typename conditional<sizeof(_Ep) == 2, short,
            typename conditional<sizeof(_Ep) == 4, int, _ccw_llong>::type>::type>::type type;
};
#else
template <class _Ep>
struct underlying_type { typedef __underlying_type(_Ep) type; };
#endif

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS && !__WATCOMC__
#endif // _CCW_LIBCPP___TYPE_TRAITS_UNDERLYING_TYPE_H
