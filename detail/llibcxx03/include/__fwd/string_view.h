//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FWD_STRING_VIEW_H
#define _CCW_LIBCPP___FWD_STRING_VIEW_H
#include "../__config"
#include "../__fwd/string.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _CharT, class _Traits = char_traits<_CharT> > class basic_string_view;
typedef basic_string_view<char> string_view;
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
