/*
 * ccwrap: libc++ layout. classic C++03 body.
 *
 *  ccwrap-only header (libc++ has no counterpart). When llibcxx03 is used as a
 *  SUPPLEMENT to a native C++ library (vc10+, gcc in C++11 mode), the standard-named
 *  traits come from that library and the __type_traits headers stay switched off;
 *  see _CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS in <__config>.
 *
 *  The include below is deliberately at namespace scope -- every __type_traits header
 *  includes this file BEFORE opening _CCW_LIBCPP_BEGIN_NAMESPACE_STD.
 */
#ifndef _CCW_LIBCPP___TYPE_TRAITS_CCW_NATIVE_TRAITS_H
#define _CCW_LIBCPP___TYPE_TRAITS_CCW_NATIVE_TRAITS_H

#include "../__config"

#if _CCW_LIBCPP_HAS_NATIVE_TYPE_TRAITS
#  include <type_traits>
#endif

#endif // _CCW_LIBCPP___TYPE_TRAITS_CCW_NATIVE_TRAITS_H
