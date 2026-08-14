/*
 * ccwrap: libc++ layout. classic C++03 body.
 *
 * common_type. The standard defines it as the type of
 * `true ? declval<_Tp>() : declval<_Up>()`, which needs decltype -- not available on a
 * C++03 front end. SUBSET: the answer is computed from the usual arithmetic
 * conversions, so it is right for
 *   - any type with itself (class types included), and
 *   - any mix of arithmetic types, which is what chrono's Rep and the numeric
 *     algorithms ask about.
 * For two DIFFERENT class types it yields the first, since detecting a conversion
 * between them is exactly what needs decltype -- do not rely on it there.
 *
 * One deliberate flattening: equal-width types are not told apart, so
 * common_type<long, int>::type is `int` where the standard says `long`. The two are
 * layout-identical on every target here, and common_type<long, long> still answers
 * `long` through the same-type case.
 */
#ifndef _CCW_LIBCPP___TYPE_TRAITS_COMMON_TYPE_H
#define _CCW_LIBCPP___TYPE_TRAITS_COMMON_TYPE_H
#include "../__config"
#include "__ccw_native_traits.h"
#include "decay.h"
#include "is_same.h"
#include "make_unsigned.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp> struct __ccw_ct_promote                { typedef _Tp type; };
template <> struct __ccw_ct_promote<bool>                   { typedef int type; };
template <> struct __ccw_ct_promote<char>                   { typedef int type; };
template <> struct __ccw_ct_promote<signed char>            { typedef int type; };
template <> struct __ccw_ct_promote<unsigned char>          { typedef int type; };
template <> struct __ccw_ct_promote<short>                  { typedef int type; };
template <> struct __ccw_ct_promote<unsigned short>         { typedef int type; };
#if !defined(_MSC_VER) || defined(_NATIVE_WCHAR_T_DEFINED)
template <> struct __ccw_ct_promote<wchar_t>                { typedef int type; };
#endif

template <class _Tp> struct __ccw_ct_rank { enum { value = 0 }; };
template <> struct __ccw_ct_rank<int>                { enum { value = 1 }; };
template <> struct __ccw_ct_rank<unsigned int>       { enum { value = 2 }; };
template <> struct __ccw_ct_rank<long>               { enum { value = 3 }; };
template <> struct __ccw_ct_rank<unsigned long>      { enum { value = 4 }; };
template <> struct __ccw_ct_rank<long long>          { enum { value = 5 }; };
template <> struct __ccw_ct_rank<unsigned long long> { enum { value = 6 }; };
template <> struct __ccw_ct_rank<float>              { enum { value = 7 }; };
template <> struct __ccw_ct_rank<double>             { enum { value = 8 }; };
template <> struct __ccw_ct_rank<long double>        { enum { value = 9 }; };

template <class _A, class _B, bool _AFirst> struct __ccw_ct_pick { typedef _A __hi; typedef _B __lo; };
template <class _A, class _B> struct __ccw_ct_pick<_A, _B, false> { typedef _B __hi; typedef _A __lo; };

template <bool _ToUnsigned, class _Hi> struct __ccw_ct_fix       { typedef _Hi type; };
template <class _Hi> struct __ccw_ct_fix<true, _Hi> { typedef typename make_unsigned<_Hi>::type type; };

template <class _A, class _B>
struct __ccw_ct_arith {
    typedef typename __ccw_ct_promote<_A>::type _PA;
    typedef typename __ccw_ct_promote<_B>::type _PB;
    enum { __ra = __ccw_ct_rank<_PA>::value };
    enum { __rb = __ccw_ct_rank<_PB>::value };
    typedef __ccw_ct_pick<_PA, _PB, (__ra >= __rb)> _P;
    typedef typename _P::__hi _Hi;
    typedef typename _P::__lo _Lo;
    enum { __rhi = __ccw_ct_rank<_Hi>::value };
    enum { __rlo = __ccw_ct_rank<_Lo>::value };
    enum { __int_pair = __rhi <= 6 && __rlo != 0 };
    enum { __mixed_sign = __rhi % 2 == 1 && __rlo % 2 == 0 };
    enum { __same_width = sizeof(_Hi) == sizeof(_Lo) };
    enum { __to_unsigned = __int_pair && __mixed_sign && __same_width };
    typedef typename __ccw_ct_fix<(bool)__to_unsigned, _Hi>::type type;
};

template <class _Tp, class _Up, bool _Same> struct __ccw_ct2 {
    typedef typename __ccw_ct_arith<_Tp, _Up>::type type;
};
template <class _Tp, class _Up> struct __ccw_ct2<_Tp, _Up, true> { typedef _Tp type; };

template <class _Tp, class _Up = void, class _Vp = void, class _Wp = void>
struct common_type {
    typedef typename common_type<typename common_type<_Tp, _Up>::type, _Vp, _Wp>::type type;
};
template <class _Tp>
struct common_type<_Tp, void, void, void> { typedef typename decay<_Tp>::type type; };
template <class _Tp, class _Up>
struct common_type<_Tp, _Up, void, void> {
    typedef typename decay<_Tp>::type _DT;
    typedef typename decay<_Up>::type _DU;
    enum { __same = is_same<_DT, _DU>::value };
    typedef typename __ccw_ct2<_DT, _DU, (bool)__same>::type type;
};
template <class _Tp, class _Up, class _Vp>
struct common_type<_Tp, _Up, _Vp, void> {
    typedef typename common_type<typename common_type<_Tp, _Up>::type, _Vp>::type type;
};

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
