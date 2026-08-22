//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: <regex> collating element names ([re.grammar] "[[.name.]]").
//  The table is the ASCII one from libc++ src/regex.cpp.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___REGEX_COLLATION_NAMES_H
#define _CCW_LIBCPP___REGEX_COLLATION_NAMES_H

#include "../__config"

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

struct __ccw_re_collname { const char* __elem_; char __char_; };

_CCW_LIBCPP_HIDE_FROM_ABI inline const __ccw_re_collname* __ccw_re_collnames(unsigned& __n) {
    static const __ccw_re_collname __t[] = {
        { "A", (char)0x41 },
        { "B", (char)0x42 },
        { "C", (char)0x43 },
        { "D", (char)0x44 },
        { "E", (char)0x45 },
        { "F", (char)0x46 },
        { "G", (char)0x47 },
        { "H", (char)0x48 },
        { "I", (char)0x49 },
        { "J", (char)0x4a },
        { "K", (char)0x4b },
        { "L", (char)0x4c },
        { "M", (char)0x4d },
        { "N", (char)0x4e },
        { "NUL", (char)0x00 },
        { "O", (char)0x4f },
        { "P", (char)0x50 },
        { "Q", (char)0x51 },
        { "R", (char)0x52 },
        { "S", (char)0x53 },
        { "T", (char)0x54 },
        { "U", (char)0x55 },
        { "V", (char)0x56 },
        { "W", (char)0x57 },
        { "X", (char)0x58 },
        { "Y", (char)0x59 },
        { "Z", (char)0x5a },
        { "a", (char)0x61 },
        { "alert", (char)0x07 },
        { "ampersand", (char)0x26 },
        { "apostrophe", (char)0x27 },
        { "asterisk", (char)0x2a },
        { "b", (char)0x62 },
        { "backslash", (char)0x5c },
        { "backspace", (char)0x08 },
        { "c", (char)0x63 },
        { "carriage-return", (char)0x0d },
        { "circumflex", (char)0x5e },
        { "circumflex-accent", (char)0x5e },
        { "colon", (char)0x3a },
        { "comma", (char)0x2c },
        { "commercial-at", (char)0x40 },
        { "d", (char)0x64 },
        { "dollar-sign", (char)0x24 },
        { "e", (char)0x65 },
        { "eight", (char)0x38 },
        { "equals-sign", (char)0x3d },
        { "exclamation-mark", (char)0x21 },
        { "f", (char)0x66 },
        { "five", (char)0x35 },
        { "form-feed", (char)0x0c },
        { "four", (char)0x34 },
        { "full-stop", (char)0x2e },
        { "g", (char)0x67 },
        { "grave-accent", (char)0x60 },
        { "greater-than-sign", (char)0x3e },
        { "h", (char)0x68 },
        { "hyphen", (char)0x2d },
        { "hyphen-minus", (char)0x2d },
        { "i", (char)0x69 },
        { "j", (char)0x6a },
        { "k", (char)0x6b },
        { "l", (char)0x6c },
        { "left-brace", (char)0x7b },
        { "left-curly-bracket", (char)0x7b },
        { "left-parenthesis", (char)0x28 },
        { "left-square-bracket", (char)0x5b },
        { "less-than-sign", (char)0x3c },
        { "low-line", (char)0x5f },
        { "m", (char)0x6d },
        { "n", (char)0x6e },
        { "newline", (char)0x0a },
        { "nine", (char)0x39 },
        { "number-sign", (char)0x23 },
        { "o", (char)0x6f },
        { "one", (char)0x31 },
        { "p", (char)0x70 },
        { "percent-sign", (char)0x25 },
        { "period", (char)0x2e },
        { "plus-sign", (char)0x2b },
        { "q", (char)0x71 },
        { "question-mark", (char)0x3f },
        { "quotation-mark", (char)0x22 },
        { "r", (char)0x72 },
        { "reverse-solidus", (char)0x5c },
        { "right-brace", (char)0x7d },
        { "right-curly-bracket", (char)0x7d },
        { "right-parenthesis", (char)0x29 },
        { "right-square-bracket", (char)0x5d },
        { "s", (char)0x73 },
        { "semicolon", (char)0x3b },
        { "seven", (char)0x37 },
        { "six", (char)0x36 },
        { "slash", (char)0x2f },
        { "solidus", (char)0x2f },
        { "space", (char)0x20 },
        { "t", (char)0x74 },
        { "tab", (char)0x09 },
        { "three", (char)0x33 },
        { "tilde", (char)0x7e },
        { "two", (char)0x32 },
        { "u", (char)0x75 },
        { "underscore", (char)0x5f },
        { "v", (char)0x76 },
        { "vertical-line", (char)0x7c },
        { "vertical-tab", (char)0x0b },
        { "w", (char)0x77 },
        { "x", (char)0x78 },
        { "y", (char)0x79 },
        { "z", (char)0x7a },
        { "zero", (char)0x30 }
    };
    __n = (unsigned)(sizeof(__t) / sizeof(__t[0]));
    return __t;
}

_CCW_LIBCPP_HIDE_FROM_ABI inline int __ccw_re_collation_char(const char* __s) {
    unsigned                   __n = 0;
    const __ccw_re_collname*   __t = __ccw_re_collnames(__n);
    unsigned                   __lo = 0, __hi = __n;
    while (__lo < __hi) {
        unsigned __mid = __lo + (__hi - __lo) / 2;
        int      __c   = 0;
        const char* __a = __t[__mid].__elem_;
        const char* __b = __s;
        for (; *__a != 0 && *__a == *__b; ++__a, ++__b) {}
        __c = (int)(unsigned char)*__a - (int)(unsigned char)*__b;
        if (__c == 0) return (int)(unsigned char)__t[__mid].__char_;
        if (__c < 0) __lo = __mid + 1; else __hi = __mid;
    }
    return -1;
}

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP___REGEX_COLLATION_NAMES_H
