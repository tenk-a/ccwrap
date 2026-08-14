//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//  ccwrap's stringstream family is 2-parameter (no separate Allocator param).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FWD_SSTREAM_H
#define _CCW_LIBCPP___FWD_SSTREAM_H
#include "../__config"
#include "../__fwd/string.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _CharT, class _Traits = char_traits<_CharT> > class basic_stringbuf;
template <class _CharT, class _Traits = char_traits<_CharT> > class basic_istringstream;
template <class _CharT, class _Traits = char_traits<_CharT> > class basic_ostringstream;
template <class _CharT, class _Traits = char_traits<_CharT> > class basic_stringstream;
typedef basic_stringbuf<char>     stringbuf;
typedef basic_istringstream<char> istringstream;
typedef basic_ostringstream<char> ostringstream;
typedef basic_stringstream<char>  stringstream;
typedef basic_stringbuf<wchar_t>     wstringbuf;
typedef basic_istringstream<wchar_t> wistringstream;
typedef basic_ostringstream<wchar_t> wostringstream;
typedef basic_stringstream<wchar_t>  wstringstream;
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
