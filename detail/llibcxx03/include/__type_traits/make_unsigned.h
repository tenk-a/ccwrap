//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_MAKE_UNSIGNED_H
#define _CCW_LIBCPP___TYPE_TRAITS_MAKE_UNSIGNED_H
#include "../__config"
#include "__ccw_native_traits.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS make_unsigned { typedef _Tp type; };

#define _CCW_MAKE_UNSIGNED(_From, _To)                                                       \
    template <> struct _CCW_LIBCPP_TEMPLATE_VIS make_unsigned<_From>                { typedef _To type; };                \
    template <> struct _CCW_LIBCPP_TEMPLATE_VIS make_unsigned<const _From>          { typedef const _To type; };          \
    template <> struct _CCW_LIBCPP_TEMPLATE_VIS make_unsigned<volatile _From>       { typedef volatile _To type; };       \
    template <> struct _CCW_LIBCPP_TEMPLATE_VIS make_unsigned<const volatile _From> { typedef const volatile _To type; };

_CCW_MAKE_UNSIGNED(signed char,        unsigned char)
_CCW_MAKE_UNSIGNED(unsigned char,      unsigned char)
_CCW_MAKE_UNSIGNED(char,               unsigned char)
_CCW_MAKE_UNSIGNED(short,              unsigned short)
_CCW_MAKE_UNSIGNED(unsigned short,     unsigned short)
_CCW_MAKE_UNSIGNED(int,                unsigned int)
_CCW_MAKE_UNSIGNED(unsigned int,       unsigned int)
_CCW_MAKE_UNSIGNED(long,               unsigned long)
_CCW_MAKE_UNSIGNED(unsigned long,      unsigned long)
_CCW_MAKE_UNSIGNED(long long,          unsigned long long)
_CCW_MAKE_UNSIGNED(unsigned long long, unsigned long long)

#undef _CCW_MAKE_UNSIGNED

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
