/**
 *  @file   detail/string.hpp
 *  @brief  <string> gap-fills: to_string / to_wstring / stoi family (C++11).
 *  @author tenka@6809.net (Masashi Kitamura)
 *  @license Boost Software License Version 1.0
 *  @note   Compiler-agnostic bodies (CRT sprintf/strtol/...); the forwarder
 *          (vc/string) includes native <string> first, then this file.
 */
#ifndef _CCW_DETAIL_STRING_HPP
#define _CCW_DETAIL_STRING_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

#if defined(_MSC_VER) && _MSC_VER == 1600
# define _CCW_STR_HAVE_TO_STRING_64 1
#else
# define _CCW_STR_HAVE_TO_STRING_64 0
#endif
#if _CCW_STD_LIB_LT(1700, 201103L)
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <stdexcept>
#include <string>

namespace std {

inline string to_string(int v)                { char b[24]; std::sprintf(b, "%d", v);   return string(b); }
inline string to_string(unsigned v)           { char b[24]; std::sprintf(b, "%u", v);   return string(b); }
inline string to_string(long v)               { char b[24]; std::sprintf(b, "%ld", v);  return string(b); }
inline string to_string(unsigned long v)      { char b[24]; std::sprintf(b, "%lu", v);  return string(b); }
#if !_CCW_STR_HAVE_TO_STRING_64
inline string to_string(_ccw_llong v)         { char b[32]; std::sprintf(b, "%" _CCW_PFMT_LL "d", v); return string(b); }
inline string to_string(_ccw_ullong v)        { char b[32]; std::sprintf(b, "%" _CCW_PFMT_LL "u", v); return string(b); }
inline string to_string(long double v)        { char b[64]; std::sprintf(b, "%Lf", (long double)v); return string(b); }
#endif
inline string to_string(double v)             { char b[64]; std::sprintf(b, "%f", v);   return string(b); }
inline string to_string(float v)              { return to_string((double)v); }

#if defined(_MSC_VER) || defined(__MINGW32__)
 #define _CCW_SWPRINTF(buf, n, fmt, val)  std::swprintf((buf), (fmt), (val))
#else
 #define _CCW_SWPRINTF(buf, n, fmt, val)  std::swprintf((buf), (n), (fmt), (val))
#endif
#if defined(_MSC_VER) && _MSC_VER < 1600
 #define _CCW_WLL   L"I64"
#else
 #define _CCW_WLL   L"ll"
#endif

inline wstring to_wstring(int v)              { wchar_t b[24]; _CCW_SWPRINTF(b, 24, L"%d", v);  return wstring(b); }
inline wstring to_wstring(long v)            { wchar_t b[24]; _CCW_SWPRINTF(b, 24, L"%ld", v); return wstring(b); }
inline wstring to_wstring(unsigned v)        { wchar_t b[24]; _CCW_SWPRINTF(b, 24, L"%u", v);  return wstring(b); }
inline wstring to_wstring(unsigned long v)   { wchar_t b[24]; _CCW_SWPRINTF(b, 24, L"%lu", v); return wstring(b); }
#if !_CCW_STR_HAVE_TO_STRING_64
inline wstring to_wstring(_ccw_llong v)      { wchar_t b[32]; _CCW_SWPRINTF(b, 32, L"%" _CCW_WLL L"d", v); return wstring(b); }
inline wstring to_wstring(_ccw_ullong v)     { wchar_t b[32]; _CCW_SWPRINTF(b, 32, L"%" _CCW_WLL L"u", v); return wstring(b); }
#endif
#if defined(__MINGW32__)
 #define _CCW_WLD_FMT      L"%f"
 #define _CCW_WLD_VAL(v)   ((double)(v))
#else
 #define _CCW_WLD_FMT      L"%Lf"
 #define _CCW_WLD_VAL(v)   ((long double)(v))
#endif
#if !_CCW_STR_HAVE_TO_STRING_64
inline wstring to_wstring(long double v)     { wchar_t b[64]; _CCW_SWPRINTF(b, 64, _CCW_WLD_FMT, _CCW_WLD_VAL(v)); return wstring(b); }
#endif
#undef _CCW_WLD_FMT
#undef _CCW_WLD_VAL
inline wstring to_wstring(double v)          { wchar_t b[64]; _CCW_SWPRINTF(b, 64, L"%f", v);  return wstring(b); }
inline wstring to_wstring(float v)           { return to_wstring((double)v); }
#undef _CCW_SWPRINTF
#undef _CCW_WLL
#undef _CCW_STR_HAVE_TO_STRING_64


}   // namespace std
#endif

#endif  // _CCW_DETAIL_STRING_HPP
