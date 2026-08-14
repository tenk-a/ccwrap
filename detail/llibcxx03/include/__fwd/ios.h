//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FWD_IOS_H
#define _CCW_LIBCPP___FWD_IOS_H
#include "../__config"
#include "../__fwd/string.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _CharT, class _Traits = char_traits<_CharT> > class basic_ios;
typedef basic_ios<char> ios;
typedef basic_ios<wchar_t> wios;
template <class _StateT> class fpos;
typedef fpos<int> streampos;
typedef fpos<int> wstreampos;
typedef fpos<int> u16streampos;
typedef fpos<int> u32streampos;
#if defined(__cpp_char8_t) || defined(__CCW_HAS_CHAR8_T)
typedef fpos<int> u8streampos;
#endif
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
