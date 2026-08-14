/**
 *  @file   detail/string_stox.hpp
 *  @brief  <string> gap-fill: the sto* family (needs a COMPLETE <stdexcept>).
 *  @license Boost Software License Version 1.0
 *  @note   Emitted by the <stdexcept> forwarder, not by string.hpp: the native
 *          <stdexcept> includes <string> while its own classes are incomplete.
 */
#ifndef _CCW_DETAIL_STRING_STOX_HPP
#define _CCW_DETAIL_STRING_STOX_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include "eh_fail.hpp"

#if _CCW_STD_LIB_LT(1600, 201103L)
#include <cstdlib>
#include <cerrno>
#include <string>
#include <stdexcept>

namespace std {

inline void __ccw_stox_check(const char* s, const char* end) {
    if (end == s) _CCW_THROW(std::invalid_argument("stox"));
    if (errno == ERANGE) _CCW_THROW(std::out_of_range("stox"));
}

inline int stoi(const string& s, size_t* pos = 0, int base = 10) {
    const char* p = s.c_str(); char* e; errno = 0;
    long v = ::strtol(p, &e, base);
    __ccw_stox_check(p, e);
    if (v > INT_MAX || v < INT_MIN) _CCW_THROW(std::out_of_range("stoi"));
    if (pos) *pos = (size_t)(e - p);
    return (int)v;
}
inline long stol(const string& s, size_t* pos = 0, int base = 10) {
    const char* p = s.c_str(); char* e; errno = 0;
    long v = ::strtol(p, &e, base); __ccw_stox_check(p, e);
    if (pos) *pos = (size_t)(e - p); return v;
}
inline unsigned long stoul(const string& s, size_t* pos = 0, int base = 10) {
    const char* p = s.c_str(); char* e; errno = 0;
    unsigned long v = ::strtoul(p, &e, base); __ccw_stox_check(p, e);
    if (pos) *pos = (size_t)(e - p); return v;
}
inline _ccw_llong stoll(const string& s, size_t* pos = 0, int base = 10) {
    const char* p = s.c_str(); char* e; errno = 0;
    _ccw_llong v = ::strtoll(p, &e, base); __ccw_stox_check(p, e);
    if (pos) *pos = (size_t)(e - p); return v;
}
inline _ccw_ullong stoull(const string& s, size_t* pos = 0, int base = 10) {
    const char* p = s.c_str(); char* e; errno = 0;
    _ccw_ullong v = ::strtoull(p, &e, base); __ccw_stox_check(p, e);
    if (pos) *pos = (size_t)(e - p); return v;
}
inline double stod(const string& s, size_t* pos = 0) {
    const char* p = s.c_str(); char* e; errno = 0;
    double v = ::strtod(p, &e); __ccw_stox_check(p, e);
    if (pos) *pos = (size_t)(e - p); return v;
}
inline float  stof(const string& s, size_t* pos = 0) { return (float)stod(s, pos); }
inline long double stold(const string& s, size_t* pos = 0) { return (long double)stod(s, pos); }

}   // namespace std
#endif

#endif  // _CCW_DETAIL_STRING_STOX_HPP
