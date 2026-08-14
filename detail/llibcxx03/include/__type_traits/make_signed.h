//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___TYPE_TRAITS_MAKE_SIGNED_H
#define _CCW_LIBCPP___TYPE_TRAITS_MAKE_SIGNED_H
#include "../__config"
#include "__ccw_native_traits.h"
#if !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _Tp> struct _CCW_LIBCPP_TEMPLATE_VIS make_signed { typedef _Tp type; };

#define _CCW_MAKE_SIGNED(_From, _To)                                                         \
    template <> struct _CCW_LIBCPP_TEMPLATE_VIS make_signed<_From>                { typedef _To type; };                \
    template <> struct _CCW_LIBCPP_TEMPLATE_VIS make_signed<const _From>          { typedef const _To type; };          \
    template <> struct _CCW_LIBCPP_TEMPLATE_VIS make_signed<volatile _From>       { typedef volatile _To type; };       \
    template <> struct _CCW_LIBCPP_TEMPLATE_VIS make_signed<const volatile _From> { typedef const volatile _To type; };

_CCW_MAKE_SIGNED(signed char,        signed char)
_CCW_MAKE_SIGNED(unsigned char,      signed char)
_CCW_MAKE_SIGNED(char,               signed char)
_CCW_MAKE_SIGNED(short,              short)
_CCW_MAKE_SIGNED(unsigned short,     short)
_CCW_MAKE_SIGNED(int,                int)
_CCW_MAKE_SIGNED(unsigned int,       int)
_CCW_MAKE_SIGNED(long,               long)
_CCW_MAKE_SIGNED(unsigned long,      long)
_CCW_MAKE_SIGNED(long long,          long long)
_CCW_MAKE_SIGNED(unsigned long long, long long)

#undef _CCW_MAKE_SIGNED

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // !_CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#endif
