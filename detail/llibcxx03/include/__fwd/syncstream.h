// -*- C++ -*-
//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//  Forward declarations for <syncstream> (C++20). Default template arguments live
//  here only, so <syncstream> and <iosfwd> never repeat them.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FWD_SYNCSTREAM_H
#define _CCW_LIBCPP___FWD_SYNCSTREAM_H
#include "../__config"
#include "../__fwd/memory.h"
#include "../__fwd/string.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> > class basic_syncbuf;
template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> > class basic_osyncstream;
typedef basic_syncbuf<char>        syncbuf;
typedef basic_osyncstream<char>    osyncstream;
typedef basic_syncbuf<wchar_t>     wsyncbuf;
typedef basic_osyncstream<wchar_t> wosyncstream;
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
