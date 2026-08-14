/**
 *  @file   detail/iomanip.hpp
 *  @brief  <iomanip> gap-fills: get_time / put_time / get_money / put_money (C++11)
 *          and quoted (C++14).
 *  @license Boost Software License Version 1.0
 *  @note   The forwarder (vc/iomanip) includes native <iomanip> first, then this file.
 *          Self-guarded, so including it unconditionally is safe.
 */
#ifndef _CCW_DETAIL_IOMANIP_HPP
#define _CCW_DETAIL_IOMANIP_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

#if _CCW_STD_LIB_LT(1600, 201103L)

#include <istream>
#include <ostream>
#include <locale>
#include <string>
#include <ctime>
#include <cctype>
#include <cstring>

namespace std {

template <class _CharT>
struct __ccw_put_time_t { const tm* __tm_; const _CharT* __fmt_; };

inline __ccw_put_time_t<char> put_time(const tm* __t, const char* __fmt) {
    __ccw_put_time_t<char> __m; __m.__tm_ = __t; __m.__fmt_ = __fmt; return __m;
}
inline __ccw_put_time_t<wchar_t> put_time(const tm* __t, const wchar_t* __fmt) {
    __ccw_put_time_t<wchar_t> __m; __m.__tm_ = __t; __m.__fmt_ = __fmt; return __m;
}

template <class _CharT, class _Traits>
basic_ostream<_CharT, _Traits>& operator<<(basic_ostream<_CharT, _Traits>& __os,
                                           __ccw_put_time_t<_CharT> __m) {
    typename basic_ostream<_CharT, _Traits>::sentry __s(__os);
    if (__s) {
        const _CharT* __e = __m.__fmt_;
        while (*__e) ++__e;
        use_facet<time_put<_CharT, ostreambuf_iterator<_CharT, _Traits> > >(__os.getloc())
            .put(ostreambuf_iterator<_CharT, _Traits>(__os), __os, __os.fill(),
                 const_cast<tm*>(__m.__tm_), __m.__fmt_, __e);
    }
    return __os;
}

struct __ccw_get_time_t { tm* __tm_; const char* __fmt_; };

inline __ccw_get_time_t get_time(tm* __t, const char* __fmt) {
    __ccw_get_time_t __m; __m.__tm_ = __t; __m.__fmt_ = __fmt; return __m;
}

template <class _Traits>
inline bool __ccw_gt_num(basic_istream<char, _Traits>& __is, int __maxdigits, int& __val) {
    int __v = 0, __n = 0;
    typename _Traits::int_type __c = __is.peek();
    while (__n < __maxdigits && __c != _Traits::eof() &&
           _Traits::to_char_type(__c) >= '0' && _Traits::to_char_type(__c) <= '9') {
        __v = __v * 10 + (_Traits::to_char_type(__c) - '0');
        __is.get();
        ++__n;
        __c = __is.peek();
    }
    if (__n == 0) return false;
    __val = __v;
    return true;
}

template <class _Traits>
inline void __ccw_gt_skipws(basic_istream<char, _Traits>& __is) {
    typename _Traits::int_type __c = __is.peek();
    while (__c != _Traits::eof() && isspace((unsigned char)_Traits::to_char_type(__c))) {
        __is.get();
        __c = __is.peek();
    }
}

template <class _Traits>
basic_istream<char, _Traits>& operator>>(basic_istream<char, _Traits>& __is, __ccw_get_time_t __m) {
    typename basic_istream<char, _Traits>::sentry __s(__is, true);   // noskipws
    if (!__s) return __is;
    bool __ok = true;
    for (const char* __f = __m.__fmt_; __ok && *__f; ++__f) {
        if (*__f != '%') {
            if (*__f == ' ' || *__f == '\t') {          // whitespace matches any run
                __ccw_gt_skipws(__is);
            } else {
                typename _Traits::int_type __c = __is.get();
                if (__c == _Traits::eof() || _Traits::to_char_type(__c) != *__f) __ok = false;
            }
            continue;
        }
        int __v = 0;
        ++__f;
        switch (*__f) {
        case 'Y': __ok = __ccw_gt_num(__is, 4, __v); if (__ok) __m.__tm_->tm_year = __v - 1900; break;
        case 'y': __ok = __ccw_gt_num(__is, 2, __v); if (__ok) __m.__tm_->tm_year = __v < 69 ? __v + 100 : __v; break;
        case 'm': __ok = __ccw_gt_num(__is, 2, __v); if (__ok) __m.__tm_->tm_mon  = __v - 1; break;
        case 'd':
        case 'e': __ok = __ccw_gt_num(__is, 2, __v); if (__ok) __m.__tm_->tm_mday = __v; break;
        case 'H':
        case 'I': __ok = __ccw_gt_num(__is, 2, __v); if (__ok) __m.__tm_->tm_hour = __v; break;
        case 'M': __ok = __ccw_gt_num(__is, 2, __v); if (__ok) __m.__tm_->tm_min  = __v; break;
        case 'S': __ok = __ccw_gt_num(__is, 2, __v); if (__ok) __m.__tm_->tm_sec  = __v; break;
        case 'j': __ok = __ccw_gt_num(__is, 3, __v); if (__ok) __m.__tm_->tm_yday = __v - 1; break;
        case 'n':
        case 't': __ccw_gt_skipws(__is); break;
        case '%': {
            typename _Traits::int_type __c = __is.get();
            if (__c == _Traits::eof() || _Traits::to_char_type(__c) != '%') __ok = false;
            break;
        }
        case '\0': --__f; __ok = false; break;
        default:   __ok = false; break;      // a specifier this subset does not handle
        }
    }
    if (!__ok) __is.setstate(ios_base::failbit);
    return __is;
}

template <class _MoneyT>
struct __ccw_put_money_t { _MoneyT __v_; bool __intl_; };

template <class _MoneyT>
inline __ccw_put_money_t<_MoneyT> put_money(const _MoneyT& __v, bool __intl = false) {
    __ccw_put_money_t<_MoneyT> __m; __m.__v_ = __v; __m.__intl_ = __intl; return __m;
}

template <class _CharT, class _Traits, class _MoneyT>
basic_ostream<_CharT, _Traits>& operator<<(basic_ostream<_CharT, _Traits>& __os,
                                           __ccw_put_money_t<_MoneyT> __m) {
    typename basic_ostream<_CharT, _Traits>::sentry __s(__os);
    if (__s) {
        use_facet<money_put<_CharT, ostreambuf_iterator<_CharT, _Traits> > >(__os.getloc())
            .put(ostreambuf_iterator<_CharT, _Traits>(__os), __m.__intl_, __os, __os.fill(),
                 static_cast<long double>(__m.__v_));
    }
    return __os;
}

template <class _MoneyT>
struct __ccw_get_money_t { _MoneyT* __p_; bool __intl_; };

template <class _MoneyT>
inline __ccw_get_money_t<_MoneyT> get_money(_MoneyT& __v, bool __intl = false) {
    __ccw_get_money_t<_MoneyT> __m; __m.__p_ = &__v; __m.__intl_ = __intl; return __m;
}

template <class _CharT, class _Traits, class _MoneyT>
basic_istream<_CharT, _Traits>& operator>>(basic_istream<_CharT, _Traits>& __is,
                                           __ccw_get_money_t<_MoneyT> __m) {
    typename basic_istream<_CharT, _Traits>::sentry __s(__is);
    if (__s) {
        ios_base::iostate __err = ios_base::goodbit;
        long double __ld = 0;
        use_facet<money_get<_CharT, istreambuf_iterator<_CharT, _Traits> > >(__is.getloc())
            .get(istreambuf_iterator<_CharT, _Traits>(__is), istreambuf_iterator<_CharT, _Traits>(),
                 __m.__intl_, __is, __err, __ld);
        if (__err == ios_base::goodbit) *__m.__p_ = static_cast<_MoneyT>(__ld);
        __is.setstate(__err);
    }
    return __is;
}

}   // namespace std
#endif  // _CCW_STD_LIB_LT(1600, 201103L)

#if _CCW_STD_LIB_LT(1900, 201402L)
#include <istream>
#include <ostream>
#include <string>
#include <cstring>

namespace std {

struct __ccw_quoted_out { const char* __p_; size_t __n_; char __delim_; char __esc_; };
struct __ccw_quoted_in  { string* __s_; char __delim_; char __esc_; };

inline __ccw_quoted_out quoted(const char* __s, char __delim = '"', char __esc = '\\') {
    __ccw_quoted_out __q;
    __q.__p_ = __s; __q.__n_ = strlen(__s); __q.__delim_ = __delim; __q.__esc_ = __esc;
    return __q;
}
inline __ccw_quoted_out quoted(const string& __s, char __delim = '"', char __esc = '\\') {
    __ccw_quoted_out __q;
    __q.__p_ = __s.c_str(); __q.__n_ = __s.size(); __q.__delim_ = __delim; __q.__esc_ = __esc;
    return __q;
}
inline __ccw_quoted_in quoted(string& __s, char __delim = '"', char __esc = '\\') {
    __ccw_quoted_in __q;
    __q.__s_ = &__s; __q.__delim_ = __delim; __q.__esc_ = __esc;
    return __q;
}

template <class _Traits>
basic_ostream<char, _Traits>& operator<<(basic_ostream<char, _Traits>& __os, __ccw_quoted_out __q) {
    string __r(1, __q.__delim_);
    for (size_t __i = 0; __i < __q.__n_; ++__i) {
        char __c = __q.__p_[__i];
        if (__c == __q.__delim_ || __c == __q.__esc_) __r += __q.__esc_;
        __r += __c;
    }
    __r += __q.__delim_;
    return __os << __r;
}

template <class _Traits>
basic_ostream<char, _Traits>& operator<<(basic_ostream<char, _Traits>& __os, __ccw_quoted_in __q) {
    __ccw_quoted_out __o;
    __o.__p_ = __q.__s_->c_str(); __o.__n_ = __q.__s_->size();
    __o.__delim_ = __q.__delim_;  __o.__esc_ = __q.__esc_;
    return __os << __o;
}

template <class _Traits>
basic_istream<char, _Traits>& operator>>(basic_istream<char, _Traits>& __is, __ccw_quoted_in __q) {
    typename basic_istream<char, _Traits>::sentry __s(__is);
    if (!__s) return __is;
    typename _Traits::int_type __c = __is.get();
    if (__c == _Traits::eof() || _Traits::to_char_type(__c) != __q.__delim_) {
        if (__c != _Traits::eof()) __is.putback(_Traits::to_char_type(__c));
        return __is >> *__q.__s_;
    }
    __q.__s_->erase();
    for (;;) {
        __c = __is.get();
        if (__c == _Traits::eof()) { __is.setstate(ios_base::failbit); break; }
        char __ch = _Traits::to_char_type(__c);
        if (__ch == __q.__esc_) {
            __c = __is.get();
            if (__c == _Traits::eof()) { __is.setstate(ios_base::failbit); break; }
            *__q.__s_ += _Traits::to_char_type(__c);
        } else if (__ch == __q.__delim_) {
            break;
        } else {
            *__q.__s_ += __ch;
        }
    }
    return __is;
}

}   // namespace std

#endif  // _CCW_STD_LIB_LT(1900, 201402L)
#endif  // _CCW_DETAIL_IOMANIP_HPP
