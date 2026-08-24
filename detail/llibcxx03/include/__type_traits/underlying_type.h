/*
 * ccwrap: libc++ layout. classic C++03 body.
 *
 *  underlying_type (C++11) is normally a compiler intrinsic: __underlying_type on
 *  GCC/Clang and on MSVC from vc11 (_MSC_VER 1700). vc8/vc9 have neither the intrinsic
 *  nor a native <type_traits>, so they fall back to "a signed integer of the same size"
 *  -- right for the ordinary int-backed enum, and bit-preserving for the cast
 *  to_underlying performs.
 *
 *  Open Watcom has no intrinsic either, and its enums are sized to fit, so the size
 *  alone is not enough: the signedness is recovered from the enum itself.
 */
#ifndef _CCW_LIBCPP___TYPE_TRAITS_UNDERLYING_TYPE_H
#define _CCW_LIBCPP___TYPE_TRAITS_UNDERLYING_TYPE_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "../__type_traits/conditional.h"

#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

#if defined(__WATCOMC__)
template <class _Ep> struct __ccw_ut_signed { enum { value = ((_Ep)(-1) < (_Ep)0) }; };

template <bool _Signed> struct __ccw_ut_pick8  { typedef unsigned char type; };
template <> struct __ccw_ut_pick8<true>        { typedef signed char type; };
template <bool _Signed> struct __ccw_ut_pick16 { typedef unsigned short type; };
template <> struct __ccw_ut_pick16<true>       { typedef short type; };
template <bool _Signed> struct __ccw_ut_pick32 { typedef unsigned int type; };
template <> struct __ccw_ut_pick32<true>       { typedef int type; };
template <bool _Signed> struct __ccw_ut_pick64 { typedef _ccw_ullong type; };
template <> struct __ccw_ut_pick64<true>       { typedef _ccw_llong type; };

template <class _Ep>
struct underlying_type {
    enum { __sgn = __ccw_ut_signed<_Ep>::value };
    typedef typename conditional<sizeof(_Ep) == 1, typename __ccw_ut_pick8 <(bool)__sgn>::type,
            typename conditional<sizeof(_Ep) == 2, typename __ccw_ut_pick16<(bool)__sgn>::type,
            typename conditional<sizeof(_Ep) == 4, typename __ccw_ut_pick32<(bool)__sgn>::type,
                                                   typename __ccw_ut_pick64<(bool)__sgn>::type
            >::type>::type>::type type;
};
#elif defined(_MSC_VER) && _MSC_VER < 1700
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

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif // _CCW_LIBCPP___TYPE_TRAITS_UNDERLYING_TYPE_H
