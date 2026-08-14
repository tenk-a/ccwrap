/*
 * ccwrap: libc++ layout. classic C++03 body.
 *
 * aligned_storage / aligned_union (C++11). No alignof and no alignment specifier are
 * needed: the alignment of a type is measured with the classic padding trick, and the
 * result is expressed as a union with a member of the wanted alignment -- which is how
 * C++03 code has always asked for aligned storage.
 *
 * LIMITS, both worth knowing before using this for anything exotic:
 *   * The strongest alignment obtainable is that of the widest fundamental type (8 on
 *     the targets here). A larger Align is accepted and produces suitably SIZED storage
 *     but only fundamental alignment -- over-alignment cannot be expressed at all on
 *     Open Watcom (no _Alignas, no __declspec(align)), and this
 *     header stays uniform across compilers rather than being right on one of them.
 *   * aligned_union takes a fixed 1..3 types (no variadics).
 */
#ifndef _CCW_LIBCPP___TYPE_TRAITS_ALIGNED_STORAGE_H
#define _CCW_LIBCPP___TYPE_TRAITS_ALIGNED_STORAGE_H
#include "../__config"
#include "__ccw_native_traits.h"
#include <cstddef>
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp>
struct __ccw_as_align_of {
    struct __probe { char __c_; _Tp __t_; };
    enum { value = sizeof(__probe) - sizeof(_Tp) };
};

template <_CCW_STD::size_t _Align> struct __ccw_as_aligner            { typedef double type; };
template <> struct __ccw_as_aligner<1>                                     { typedef char   type; };
template <> struct __ccw_as_aligner<2>                                     { typedef short  type; };
template <> struct __ccw_as_aligner<4>                                     { typedef int    type; };
template <> struct __ccw_as_aligner<8>                                     { typedef double type; };

template <_CCW_STD::size_t _Len, _CCW_STD::size_t _Align = 8>
struct aligned_storage {
    union type {
        typename __ccw_as_aligner<_Align>::type __align_;
        unsigned char __data_[_Len > 1 ? _Len : 1];
    };
};

template <_CCW_STD::size_t _Len, class _T0, class _T1 = char, class _T2 = char>
struct aligned_union {
    enum { __s01 = sizeof(_T0) > sizeof(_T1) ? sizeof(_T0) : sizeof(_T1) };
    enum { __s = (__s01 > (int)sizeof(_T2) ? __s01 : (int)sizeof(_T2)) };
    enum { __size = (__s > (int)_Len ? __s : (int)_Len) };
    enum { __a01 = (int)__ccw_as_align_of<_T0>::value > (int)__ccw_as_align_of<_T1>::value
                 ? (int)__ccw_as_align_of<_T0>::value : (int)__ccw_as_align_of<_T1>::value };
    enum { __a = (__a01 > (int)__ccw_as_align_of<_T2>::value ? __a01 : (int)__ccw_as_align_of<_T2>::value) };
    static const _CCW_STD::size_t alignment_value = (_CCW_STD::size_t)__a;
    typedef typename aligned_storage<(_CCW_STD::size_t)__size, (_CCW_STD::size_t)__a>::type type;
};

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
