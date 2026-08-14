// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//  Forward declarations for <spanstream> (C++23). Default template arguments live
//  here only, so <spanstream> and <iosfwd> never repeat them.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FWD_SPANSTREAM_H
#define _CCW_LIBCPP___FWD_SPANSTREAM_H
#include "../__config"
#include "../__fwd/string.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _CharT, class _Traits = char_traits<_CharT> > class basic_spanbuf;
template <class _CharT, class _Traits = char_traits<_CharT> > class basic_ispanstream;
template <class _CharT, class _Traits = char_traits<_CharT> > class basic_ospanstream;
template <class _CharT, class _Traits = char_traits<_CharT> > class basic_spanstream;
typedef basic_spanbuf<char>      spanbuf;
typedef basic_ispanstream<char>  ispanstream;
typedef basic_ospanstream<char>  ospanstream;
typedef basic_spanstream<char>   spanstream;
typedef basic_spanbuf<wchar_t>     wspanbuf;
typedef basic_ispanstream<wchar_t> wispanstream;
typedef basic_ospanstream<wchar_t> wospanstream;
typedef basic_spanstream<wchar_t>  wspanstream;
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
