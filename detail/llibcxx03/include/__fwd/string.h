//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout.
//  char_traits + basic_string forward declarations. This is the ONE place the
//  default template arguments (traits = char_traits, alloc = allocator) are given.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___FWD_STRING_H
#define _CCW_LIBCPP___FWD_STRING_H
#include "../__config"
#include "../__fwd/memory.h"
#if _CCW_LIBCPP_NATIVE_STD
#  include <string>
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _CharT> struct char_traits;
template <>            struct char_traits<char>;
template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> > class basic_string;
typedef basic_string<char>     string;
typedef basic_string<wchar_t>  wstring;
typedef basic_string<char16_t> u16string;
typedef basic_string<char32_t> u32string;
#if defined(__cpp_char8_t) || defined(__CCW_HAS_CHAR8_T)
typedef basic_string<char8_t>  u8string;
#endif
_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_NATIVE_STD
#endif
