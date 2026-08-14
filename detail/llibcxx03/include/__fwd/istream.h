//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FWD_ISTREAM_H
#define _CCW_LIBCPP___FWD_ISTREAM_H
#include "../__config"
#include "../__fwd/string.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _CharT, class _Traits = char_traits<_CharT> > class basic_istream;
template <class _CharT, class _Traits = char_traits<_CharT> > class basic_iostream;
typedef basic_istream<char>  istream;
typedef basic_iostream<char> iostream;
typedef basic_istream<wchar_t>  wistream;
typedef basic_iostream<wchar_t> wiostream;
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
