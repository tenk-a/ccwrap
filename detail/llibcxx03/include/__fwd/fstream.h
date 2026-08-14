//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FWD_FSTREAM_H
#define _CCW_LIBCPP___FWD_FSTREAM_H
#include "../__config"
#include "../__fwd/string.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _CharT, class _Traits = char_traits<_CharT> > class basic_filebuf;
template <class _CharT, class _Traits = char_traits<_CharT> > class basic_ifstream;
template <class _CharT, class _Traits = char_traits<_CharT> > class basic_ofstream;
template <class _CharT, class _Traits = char_traits<_CharT> > class basic_fstream;
typedef basic_filebuf<char>  filebuf;
typedef basic_ifstream<char> ifstream;
typedef basic_ofstream<char> ofstream;
typedef basic_fstream<char>  fstream;
typedef basic_filebuf<wchar_t>  wfilebuf;
typedef basic_ifstream<wchar_t> wifstream;
typedef basic_ofstream<wchar_t> wofstream;
typedef basic_fstream<wchar_t>  wfstream;
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
