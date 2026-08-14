/*
 * ccwrap: libc++ layout. classic C++03 body.
 *  Classic char traits used by basic_string.
 *  Generic template plus a char specialization;
 *  operations are written as plain loops (no <cstring> dependency).
 */
#ifndef _CCW_LIBCPP___STRING_CHAR_TRAITS_H
#define _CCW_LIBCPP___STRING_CHAR_TRAITS_H
#include "../__config"
#include <cstddef>
#if _CCW_LIBCPP_HAS_NATIVE_CHAR_TRAITS
#  include <string>
#else

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

template <class _CharT>
struct char_traits {
    typedef _CharT    char_type;
    typedef int       int_type;
    typedef _CCW_STD::size_t size_type;
    typedef _CCW_STD::ptrdiff_t off_type;    // streamoff (simplified)
    typedef _CCW_STD::ptrdiff_t pos_type;    // streampos (simplified; real is fpos<state_type>)
    typedef int            state_type;  // mbstate_t (placeholder; single-byte target)

    _CCW_LIBCPP_HIDE_FROM_ABI static void assign(char_type& __a, const char_type& __b) { __a = __b; }
    _CCW_LIBCPP_HIDE_FROM_ABI static bool eq(const char_type& __a, const char_type& __b) { return __a == __b; }
    _CCW_LIBCPP_HIDE_FROM_ABI static bool lt(const char_type& __a, const char_type& __b) { return __a < __b; }

    _CCW_LIBCPP_HIDE_FROM_ABI static size_type length(const char_type* __s) {
        size_type __n = 0; while (!eq(__s[__n], char_type())) ++__n; return __n;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI static int compare(const char_type* __a, const char_type* __b, size_type __n) {
        for (size_type __i = 0; __i < __n; ++__i) {
            if (lt(__a[__i], __b[__i])) return -1;
            if (lt(__b[__i], __a[__i])) return 1;
        }
        return 0;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI static const char_type* find(const char_type* __s, size_type __n, const char_type& __c) {
        for (size_type __i = 0; __i < __n; ++__i) if (eq(__s[__i], __c)) return __s + __i;
        return 0;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI static char_type* copy(char_type* __d, const char_type* __s, size_type __n) {
        for (size_type __i = 0; __i < __n; ++__i) __d[__i] = __s[__i]; return __d;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI static char_type* move(char_type* __d, const char_type* __s, size_type __n) {
        if (__d < __s) { for (size_type __i = 0; __i < __n; ++__i) __d[__i] = __s[__i]; }
        else if (__d > __s) { for (size_type __i = __n; __i > 0; --__i) __d[__i - 1] = __s[__i - 1]; }
        return __d;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI static char_type* assign(char_type* __d, size_type __n, char_type __c) {
        for (size_type __i = 0; __i < __n; ++__i) __d[__i] = __c; return __d;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI static int_type eof() { return -1; }
    _CCW_LIBCPP_HIDE_FROM_ABI static int_type to_int_type(const char_type& __c) { return int_type(__c); }
    _CCW_LIBCPP_HIDE_FROM_ABI static char_type to_char_type(const int_type& __i) { return char_type(__i); }
    _CCW_LIBCPP_HIDE_FROM_ABI static bool eq_int_type(const int_type& __a, const int_type& __b) { return __a == __b; }
    _CCW_LIBCPP_HIDE_FROM_ABI static int_type not_eof(const int_type& __i) { return eq_int_type(__i, eof()) ? 0 : __i; }
};

template <>
struct char_traits<char> {
    typedef char        char_type;
    typedef int         int_type;
    typedef _CCW_STD::size_t size_type;
    typedef _CCW_STD::ptrdiff_t off_type;    // streamoff (simplified)
    typedef _CCW_STD::ptrdiff_t pos_type;    // streampos (simplified; real is fpos<state_type>)
    typedef int            state_type;  // mbstate_t (placeholder; single-byte target)

    _CCW_LIBCPP_HIDE_FROM_ABI static void assign(char_type& __a, const char_type& __b) { __a = __b; }
    _CCW_LIBCPP_HIDE_FROM_ABI static bool eq(const char_type& __a, const char_type& __b) { return __a == __b; }
    _CCW_LIBCPP_HIDE_FROM_ABI static bool lt(const char_type& __a, const char_type& __b) {
        return (unsigned char)__a < (unsigned char)__b;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI static size_type length(const char_type* __s) {
        size_type __n = 0; while (__s[__n]) ++__n; return __n;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI static int compare(const char_type* __a, const char_type* __b, size_type __n) {
        for (size_type __i = 0; __i < __n; ++__i) {
            unsigned char __ca = (unsigned char)__a[__i], __cb = (unsigned char)__b[__i];
            if (__ca < __cb) return -1;
            if (__cb < __ca) return 1;
        }
        return 0;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI static const char_type* find(const char_type* __s, size_type __n, const char_type& __c) {
        for (size_type __i = 0; __i < __n; ++__i) if (__s[__i] == __c) return __s + __i;
        return 0;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI static char_type* copy(char_type* __d, const char_type* __s, size_type __n) {
        for (size_type __i = 0; __i < __n; ++__i) __d[__i] = __s[__i]; return __d;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI static char_type* move(char_type* __d, const char_type* __s, size_type __n) {
        if (__d < __s) { for (size_type __i = 0; __i < __n; ++__i) __d[__i] = __s[__i]; }
        else if (__d > __s) { for (size_type __i = __n; __i > 0; --__i) __d[__i - 1] = __s[__i - 1]; }
        return __d;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI static char_type* assign(char_type* __d, size_type __n, char_type __c) {
        for (size_type __i = 0; __i < __n; ++__i) __d[__i] = __c; return __d;
    }
    _CCW_LIBCPP_HIDE_FROM_ABI static int_type eof() { return -1; }
    _CCW_LIBCPP_HIDE_FROM_ABI static int_type to_int_type(const char_type& __c) { return int_type((unsigned char)__c); }
    _CCW_LIBCPP_HIDE_FROM_ABI static char_type to_char_type(const int_type& __i) { return char_type(__i); }
    _CCW_LIBCPP_HIDE_FROM_ABI static bool eq_int_type(const int_type& __a, const int_type& __b) { return __a == __b; }
    _CCW_LIBCPP_HIDE_FROM_ABI static int_type not_eof(const int_type& __i) { return __i == eof() ? 0 : __i; }
};

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP_HAS_NATIVE_CHAR_TRAITS
#endif // _CCW_LIBCPP___STRING_CHAR_TRAITS_H
