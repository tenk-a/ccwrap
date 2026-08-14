//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FWD_OSTREAM_H
#define _CCW_LIBCPP___FWD_OSTREAM_H
#include "../__config"
#include "../__fwd/string.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _CharT, class _Traits = char_traits<_CharT> > class basic_ostream;
typedef basic_ostream<char> ostream;
typedef basic_ostream<wchar_t> wostream;
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
