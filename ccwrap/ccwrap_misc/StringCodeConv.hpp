/**
 *  @file StringCodeConv.hpp
 *  @brief  String encoding conversion
 *  @author Masashi Kitamura(tenka@6809.net)
 *  @date   2020
 *  @license Boost Software Lisence Version 1.0
 *  note
 *      base https://github.com/tenk-a/abx/blob/master/src/fks/ccwrap_mbc.c
 */
#ifndef __CCWRAP_StringCodeConv_hpp_INCLUDED
#define __CCWRAP_StringCodeConv_hpp_INCLUDED

#include <cstddef>

#include <cstdint>
#include <type_traits>
#include <string>
#include <iostream>

#include <string_view>

#if defined(_WIN32) && !defined(CCWRAP_CHAR_IS_UTF8)
#include <windows.h>
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1400
# pragma warning(push)
# pragma warning(disable:4244)
#endif

// ============================================================================
// ccwrap decl.

namespace ccwrap {
    using std::size_t;
    using std::ptrdiff_t;
    using std::max_align_t;
 #if __CCWRAP_CXX >= 201103L
    using std::nullptr_t;
 #endif

    using std::int8_t;
    using std::uint8_t;
    using std::int16_t;
    using std::uint16_t;
    using std::int32_t;
    using std::uint32_t;
    using std::int64_t;
    using std::uint64_t;
    using std::intptr_t;
    using std::uintptr_t;
    using std::intmax_t;
    using std::uintmax_t;

 #if !defined(__CCWRAP_HAS_CHAR8_T) && !defined(__cpp_char8_t)
   typedef unsigned char    char8_t;
 #endif

 #if 0
    typedef signed char         schar;
    typedef unsigned char       uchar;
    typedef unsigned short      ushort;
    typedef unsigned int        uint;
    typedef unsigned long       ulong;
    typedef unsigned long long  llong;
    typedef unsigned long long  ullong;
 #endif
}

namespace ccwrap {
    using std::basic_ostream;
    using std::basic_istream;
    using std::basic_string;
    using std::string;
    using std::wstring;
    using std::u16string;
    using std::u32string;
    using std::char_traits;
    using std::char_traits;
    using std::allocator;

    using std::basic_string_view;
    using std::string_view;
    using std::wstring_view;
    using std::u16string_view;
    using std::u32string_view;

 #if __CCWRAP_CXX >= 202002L
    using std::u8string;
    using std::u8string_view;
 #else
    typedef basic_string<char8_t>       u8string;
    typedef basic_string_view<char8_t>  u8string_view;
 #endif
}

namespace ccwrap {
    typedef int         wide_char_t;

    struct char16re_t { char16_t value; };      // reverse endian
    struct char32re_t { char32_t value; };      // reverse endian

 #ifdef __CCWRAP_BIG_ENDIAN
    typedef char16_t    char16be_t;             // big endian
    typedef char16re_t  char16le_t;             // little endian
    typedef char32_t    char32be_t;             // big endian
    typedef char32re_t  char32le_t;             // little endian
 #else
    typedef char16_t    char16le_t;             // little endian
    typedef char16re_t  char16be_t;             // big endian
    typedef char32_t    char32le_t;             // little endian
    typedef char32re_t  char32be_t;             // big endian
 #endif

}   // namespace ccwrap


// ============================================================================
// CharCodeConvTraits

namespace ccwrap {

#define __CCWRAP_REV_UC16(c)  ((uint8_t)((c) >> 8) | ((uint8_t)(c) << 8))
#define __CCWRAP_REV_UC32(c)  (uint8_t(uint32_t(c) >> 24) | (uint8_t(uint32_t(c) >> 16) << 8) | (uint8_t(uint32_t(c) >> 8)<<16) | (uint8_t(c) << 24))

template<class C, bool ErrCk=true>
class CharCodeConvTraits { };


namespace __det_CharConvTraits {

/** Get display size of character(for JP font)
 */
template<class DMY>
inline size_t unicode_chWidth(wide_char_t c) {
    if (c < 0x370)
        return 1;
    if (c >= 0xff60 && c <= 0xff9f)
        return 1;
    return 2;
}

template<class DMY>
inline bool unicode_checkCh(wide_char_t c) {
    return c < 0xD800 || (c >= 0xE000 && c <= 0x10FFFD && (uint16_t)c != 0xffff && (uint16_t)c != 0xfffe );
}

template<class DMY>
void* memClr(void* bgn, void const* end) {
    char* b = (char*)bgn;
    char* e = (char*)end;
    while (b < e)
        *b++ = 0;
    return e;
}

}   // __det_CharConvTraits

template<bool ErrCk>
class CharCodeConvTraits<char8_t, ErrCk> {
public:
    typedef char8_t                 value_type;
    typedef ccwrap::wide_char_t     wide_char_type;
    typedef size_t                  size_type;
    typedef value_type*             pointer;
    typedef value_type const*       const_pointer;
    static wide_char_type   getChr(const_pointer& s, const_pointer e);
    static wide_char_type   peekChr(const_pointer s, const_pointer e) { return getChr(s, e); }
    static pointer          setChr(pointer d, const_pointer e, wide_char_type c);
    static size_type        chrPtrBytes(const_pointer s);
    static size_type        chrBytes(wide_char_type c);
    static size_type        chrWidth(wide_char_type c) { return __det_CharConvTraits::unicode_chWidth<void>(c); }
    static bool             checkChr(wide_char_type c) { return __det_CharConvTraits::unicode_checkCh<void>(c); }
};

/** Get character.
 */
template<bool ErrCk> wide_char_t
CharCodeConvTraits<char8_t, ErrCk>::getChr(value_type const*& rStr, value_type const* e) {
    char8_t const*  s = (char8_t const*)rStr;
    char8_t         b;
    wide_char_type  c,d;
    if (s >= e) goto END_ERR_RET;
    b = *s++;
    c = b, d = c;
    if (c < 0xC0) {
        if (c < 0x80)
            goto RET;
        goto CODE_ERR_RET;
    }
    if (s >= e) goto END_ERR_RET;
    b = *s++;
    if (b < 0x80)
        goto CODE_ERR_RET;
    d = (d << 6) | (b & 0x3f);        // 11=5+6 0x80 .. 0x7ff
    if (c < 0xE0) {
        d &= (1 << 11) - 1;
        goto RET;
    }
    if (s >= e) goto END_ERR_RET;
    b = *s++;
    if (b < 0x80)
        goto CODE_ERR_RET;
    d = (d << 6) | (b & 0x3f);        // 16=4+6*2 0x8000 .. 0xffff
    if (c < 0xF0) {
        d &= (1 << 16) - 1;
        goto RET;
    }
    if (s >= e) goto END_ERR_RET;
    b = *s++;
    if (b < 0x80)
        goto CODE_ERR_RET;
    d = (d << 6) | (b & 0x3f);        // 21=3+6*3
    if (c < 0xF8) {
        d &= (1 << 21) - 1;
        goto RET;
    }
    if (s >= e) goto END_ERR_RET;
    b = *s++;
    if (b < 0x80)
        goto CODE_ERR_RET;
    d = (d << 6) | (b & 0x3f);        // 26=2+6*4
    if (c < 0xFC) {
        d &= (1 << 26) - 1;
        goto RET;
    }
    if (s >= e) goto END_ERR_RET;
    b = *s++;
    if (b < 0x80)
        goto CODE_ERR_RET;
    d = (d << 6) | (b & 0x3f);        // 31=1+6*5
    d &= (1U << 31) - 1;
 RET:
    rStr = (value_type const*)s;
    return d;

 END_ERR_RET:
    rStr = e;
    return (wide_char_type)-1;    // eos

 CODE_ERR_RET:
    rStr = (value_type const*)(s - 1);
    return (wide_char_type)-2;    // error char
}

/** Set character.
 */
template<bool ErrCk> typename CharCodeConvTraits<char8_t, ErrCk>::pointer
CharCodeConvTraits<char8_t, ErrCk>::setChr(value_type*  dst, value_type const* end, wide_char_type c) {
    char8_t*    d = (value_type*)dst;
    char8_t*    e = (value_type*)end;
    if (c < 0xC0/*0x80*/) { // 0x80-xBF bad code
        if (d >= e) goto ERR_RET;
        *d++ = c;
    } else if (c <= 0x7FF) {
        if (d+2 > e) goto ERR_RET;
        *d++ = 0xC0|(c>>6);
        *d++ = 0x80|(c&0x3f);
    } else if (c <= 0xFFFF) {
        if (d+3 > e) goto ERR_RET;
        *d++ = 0xE0|(c>>12);
        *d++ = 0x80|((c>>6)&0x3f);
        *d++ = 0x80|(c&0x3f);
        //if (c >= 0xff60 && c <= 0xff9f) {--(*adn); }  // hankaku-kana
    } else if (c <= 0x1fFFFF) {
        if (d+4 > e) goto ERR_RET;
        *d++ = 0xF0|(c>>18);
        *d++ = 0x80|((c>>12)&0x3f);
        *d++ = 0x80|((c>>6)&0x3f);
        *d++ = 0x80|(c&0x3f);
    } else if (c <= 0x3fffFFFF) {
        if (d+5 > e) goto ERR_RET;
        *d++ = 0xF8|(c>>24);
        *d++ = 0x80|((c>>18)&0x3f);
        *d++ = 0x80|((c>>12)&0x3f);
        *d++ = 0x80|((c>>6)&0x3f);
        *d++ = 0x80|(c&0x3f);
    } else {
        if (d+6 > e) goto ERR_RET;
        *d++ = 0xFC|(c>>30);
        *d++ = 0x80|((c>>24)&0x3f);
        *d++ = 0x80|((c>>18)&0x3f);
        *d++ = 0x80|((c>>12)&0x3f);
        *d++ = 0x80|((c>>6)&0x3f);
        *d++ = 0x80|(c&0x3f);
    }
    return d;

ERR_RET:
    __det_CharConvTraits::memClr<void>(d, e);
    return e;
}

template<bool ErrCk>
size_t CharCodeConvTraits<char8_t, ErrCk>::chrPtrBytes(const_pointer s) {
    unsigned c = *(unsigned char*)s;
    if (c < 0xC0)
        return 1;
    if (c < 0xE0)
        return 2;
    if (c < 0xF0)
        return 3;
    if (c < 0xF8)
        return 4;
    if (c < 0xFC)
        return 5;
    return 6;
}

/** Size of character.
 */
template<bool ErrCk>
size_t CharCodeConvTraits<char8_t, ErrCk>::chrBytes(wide_char_t c) {
    if (c < 0x80)
        return 1;
    if (c <= 0x7FF)
        return 2;
    if (c <= 0xFFFF)
        return 3;
    if (c <= 0x1fFFFF)
        return 4;
    if (c <= 0x3fffFFFF)
        return 5;
    return 6;
}

// ---------------------------------------------------------------------------

template<bool ErrCk>
class CharCodeConvTraits<char16_t, ErrCk> {
public:
    typedef char16_t            value_type;
    typedef wide_char_t         wide_char_type;
    typedef size_t              size_type;
    typedef value_type*         pointer;
    typedef value_type const*   const_pointer;

    static wide_char_type   getChr(const_pointer& s, const_pointer e);
    static wide_char_type   peekChr(const_pointer s, const_pointer e) { return getChr(s, e); }
    static pointer          setChr(pointer d, const_pointer e, wide_char_type c);
    static size_type        chrPtrBytes(const_pointer s) {char16_t c=*s;return (size_type(0xD800<=c && c<=0xDBFF)+1)*2;}
    static size_type        chrBytes(wide_char_type c) { return c > 0xffff ? 4 : 2/*c ? 2 : 0*/; }
    static size_type        chrWidth(wide_char_type c) { return __det_CharConvTraits::unicode_chWidth<void>(c); }
    static bool             checkChr(wide_char_type c) { return __det_CharConvTraits::unicode_checkCh<void>(c); }
};

template<bool ErrCk> wide_char_t
CharCodeConvTraits<char16_t, ErrCk>::getChr(const_pointer& rStr, const_pointer end) {
    uint16_t const* p = (uint16_t const*)rStr;
    uint16_t const* e = (uint16_t const*)end;
    wide_char_t     c;
    if (p + 1 > e) {
        p = e;
        c = -1;
    } else {
        c = *p++;
        if (0xD800 <= c && c <= 0xDBFF) {
            if (p + 1 > e) {
                p = e;
                c = -1;
            } else {
                wide_char_t d = *p;
                if (0xDC00 <= d && d <= 0xDFFF) {
                    ++p;
                    c = ((c & 0x3ff) << 10) | (d & 0x3ff);
                    c += 0x10000;
                } else {
                    c = -2;
                }
            }
        }
    }
    rStr = (const_pointer)p;
    return c;
}

template<bool ErrCk> typename CharCodeConvTraits<char16_t, ErrCk>::pointer
CharCodeConvTraits<char16_t, ErrCk>::setChr(pointer d, const_pointer e, wide_char_type c) {
    if (c < 0x10000) {
        if (d+1 > e) goto ERR_RET;
        *d++ = char16_t(c);
    } else if (c <= 0x10FFFF) {
        uint32_t x = c - 0x10000;
        if (d+2 > e) goto ERR_RET;
        c = (x >> 10) | 0xD800;
        *d++ = char16_t(c);
        c = (x & 0x3ff) | 0xDC00;
        *d++ = char16_t(c);
    } else {    // error
    }
    return d;
ERR_RET:
    __det_CharConvTraits::memClr<void>(d, e);
    return (pointer)e;
}

template<bool ErrCk>
class CharCodeConvTraits<char16re_t, ErrCk> {
    typedef CharCodeConvTraits<char16_t, ErrCk>   Orig;
public:
    typedef char16re_t          value_type;
    typedef wide_char_t         wide_char_type;
    typedef size_t              size_type;
    typedef value_type*         pointer;
    typedef value_type const*   const_pointer;

    static wide_char_type   getChr(const_pointer& s, const_pointer e);
    static wide_char_type   peekChr(const_pointer s, const_pointer e) { return getChr(s, e); }
    static pointer          setChr(pointer d, const_pointer e, wide_char_type c);
    static size_type        chrPtrBytes(const_pointer s) {
                                char16_t c = char16_t(__CCWRAP_REV_UC16(*(uint16_t const*)s));
                                return (size_type((0xD800<=c) & (c<=0xDBFF)) + 1) * 2;
                            }
    static size_type        chrBytes(wide_char_type c) { return Orig::chrBytes(c); }
    static size_type        chrWidth(wide_char_type c) { return __det_CharConvTraits::unicode_chWidth<void>(c); }
    static bool             checkChr(wide_char_type c) { return __det_CharConvTraits::unicode_checkCh<void>(c); }
};

template<bool ErrCk> wide_char_t
CharCodeConvTraits<char16re_t, ErrCk>::getChr(const_pointer& rStr, const_pointer end) {
    uint16_t const* p = (uint16_t const*)rStr;
    uint16_t const* e = (uint16_t const*)end;
    wide_char_t     c;
    if (p + 1 > e) {
        p = e;
        c = -1;
    } else {
        c = __CCWRAP_REV_UC16(*p);
        ++p;
        if (0xD800 <= c && c <= 0xDBFF) {
            if (p + 1 > e) {
                p = e;
                c = -1;
            } else {
                wide_char_t  d = __CCWRAP_REV_UC16(*p);
                if (0xDC00 <= d && d <= 0xDFFF) {
                    ++p;
                    c = ((c & 0x3ff) << 10) | (d & 0x3ff);
                    c += 0x10000;
                } else {
                    c = -2;
                }
            }
        }
    }
    rStr = (const_pointer)p;
    return c;
}

template<bool ErrCk> typename CharCodeConvTraits<char16re_t, ErrCk>::pointer
CharCodeConvTraits<char16re_t, ErrCk>::setChr(pointer d, const_pointer e, wide_char_type c) {
    if (c < 0x10000) {
        if (d+1 > e) goto ERR_RET;
        *(uint16_t*)d = __CCWRAP_REV_UC16(c);
        ++d;
    } else if (c <= 0x10FFFF) {
        uint32_t x = c - 0x10000;
        if (d+2 > e) goto ERR_RET;
        c = (x >> 10) | 0xD800;
        *(uint16_t*)d = __CCWRAP_REV_UC16(c);
        ++d;
        c = (x & 0x3ff) | 0xDC00;
        *(uint16_t*)d = __CCWRAP_REV_UC16(c);
        ++d;
    } else {    // error
    }
    return d;
ERR_RET:
    __det_CharConvTraits::memClr<void>(d, e);
    return (pointer)e;
}


// ---------------------------------------------------------------------------

template<bool ErrCk>
class CharCodeConvTraits<char32_t, ErrCk> {
public:
    typedef char32_t            value_type;
    typedef wide_char_t         wide_char_type;
    typedef size_t              size_type;
    typedef value_type*         pointer;
    typedef value_type const*   const_pointer;

    static wide_char_type   getChr(const_pointer& s, const_pointer e) { return (s+1 <= e) ? *s++ : ((s=e), -1); }
    static wide_char_type   peekChr(const_pointer s, const_pointer e) { return getChr(s, e); }
    static pointer          setChr(pointer d, const_pointer e, wide_char_type c) {
                                if (d+1 <= e) {
                                    *d++ = c;
                                    return d;
                                } else {
                                    __det_CharConvTraits::memClr<void>(d, e);
                                    return (pointer)e;
                                }
                            }
    static size_type        chrPtrBytes(const_pointer /*s*/) { return 4; }
    static size_type        chrBytes(wide_char_type /*c*/) { return 4; }
    static size_type        chrWidth(wide_char_type c) { return __det_CharConvTraits::unicode_chWidth<void>(c); }
    static bool             checkChr(wide_char_type c) { return __det_CharConvTraits::unicode_checkCh<void>(c); }
};


template<bool ErrCk>
class CharCodeConvTraits<char32re_t, ErrCk> {
public:
    typedef char32re_t          value_type;
    typedef wide_char_t         wide_char_type;
    typedef size_t              size_type;
    typedef value_type*         pointer;
    typedef value_type const*   const_pointer;

    static wide_char_type   getChr(const_pointer& s, const_pointer e) {
                                if (s+1 <= e) {
                                    wide_char_type c = __CCWRAP_REV_UC32(*(uint32_t const*)s);
                                    ++s;
                                    return c;
                                }
                                s = e;
                                return -1;
                            }
    static wide_char_type   peekChr(const_pointer s, const_pointer e) { return getChr(s, e); }
    static pointer          setChr(pointer d, const_pointer e, wide_char_type c) {
                                if (d+1 <= e) {
                                    *(uint32_t*)d = __CCWRAP_REV_UC32(c);
                                    return d+1;
                                } else {
                                    __det_CharConvTraits::memClr<void>(d, e);
                                    return (pointer)e;
                                }
                            }
    static size_type        chrPtrBytes(const_pointer /*s*/) { return 4; }
    static size_type        chrBytes(wide_char_type /*c*/) { return 4; }
    static size_type        chrWidth(wide_char_type c) { return __det_CharConvTraits::unicode_chWidth<void>(c); }
    static bool             checkChr(wide_char_type c) { return __det_CharConvTraits::unicode_checkCh<void>(c); }
};


// ---------------------------------------------------------------------------
template<bool ErrCk>
class CharCodeConvTraits<wchar_t, ErrCk> {
public:
    typedef typename std::conditional<(sizeof(wchar_t)==2), char16_t, char32_t>::type   orig_value_type;
    typedef wchar_t             value_type;
    typedef wide_char_t         wide_char_type;
    typedef size_t              size_type;
    typedef value_type*         pointer;
    typedef value_type const*   const_pointer;
private:
    typedef CharCodeConvTraits<orig_value_type,ErrCk> Orig;
public:
    static bool             isLead(value_type c) { return Orig::isLead( c); }
    static bool             isLead(const_pointer s) { return Orig::isLead((value_type const*)s); }
    static wide_char_type   getChr(const_pointer& s, const_pointer e) {
                                return Orig::getChr((typename Orig::const_pointer&)s, (typename Orig::const_pointer)e);
                            }
    static wide_char_type   peekChr(const_pointer s, const_pointer e) { return getChr(s, e); }
    static pointer          setChr(pointer d, const_pointer e, wide_char_type c) {
                                return (pointer)Orig::setChr((typename Orig::pointer)d
                                                            ,(typename Orig::const_pointer)e,c);
                            }
    static size_type        chrPtrBytes(const_pointer s) { return Orig::chrPtrBytes(s); }
    static size_type        chrBytes(wide_char_type c) { return Orig::chrBytes(c); }
    static size_type        chrWidth(wide_char_type c) { return __det_CharConvTraits::unicode_chWidth<void>(c); }
    static bool             checkChr(wide_char_type c) { return __det_CharConvTraits::unicode_checkCh<void>(c); }
};


// ---------------------------------------------------------------------------

#if defined(_WIN32)
struct char_dbc_t { unsigned char c; };

template<bool ErrCk>
class CharCodeConvTraits<char_dbc_t, ErrCk> {
public:
    typedef char_dbc_t          value_type;
    typedef wide_char_t         wide_char_type;
    typedef size_t              size_type;
    typedef value_type*         pointer;
    typedef value_type const*   const_pointer;

    static wide_char_type   getChr(const_pointer& s, const_pointer e);
    static wide_char_type   peekChr(const_pointer s, const_pointer e);
    static pointer          setChr(pointer d, const_pointer e, wide_char_type c);
    static size_type        chrPtrBytes(const_pointer s) { return size_type(isLead(s->c)) + 1; }
    static size_type        chrBytes(wide_char_type c) { return 1 + size_type(c > 0xff); }
    static size_type        chrWidth(wide_char_type c) { return 1 + size_type(c > 0xff); }
    static bool             checkChr(wide_char_type c) {
                                return (c > 0xff) ? (isLead(c >> 8) && isTrail((unsigned char)c)) : true;
                            }
private:
    static bool             isLead(unsigned char c) { return IsDBCSLeadByte(c) != 0; }
    static bool             isTrail(unsigned char c) { return ((c >= 0x30) & (c <= 0xFE)) && c != 0x7f; }
};

template<bool ErrCk> inline
wide_char_t CharCodeConvTraits<char_dbc_t, ErrCk>::getChr(const_pointer& rStr, const_pointer end) {
    unsigned char const* s = (unsigned char const*)rStr;
    unsigned char const* e = (unsigned char const*)end;
    if (s >= e) {
        rStr = end;
        return -1;
    }
    wide_char_t c = *s++;
    if (isLead(c)) {
        if (s < e && ((ErrCk && isTrail(*s)) || (!ErrCk && *s))) {
            c = (c << 8) | *s;
            ++s;
        } else if (ErrCk) {
            c = -2;
        }
    }
    rStr = (const_pointer)s;
    return c;
}

template<bool ErrCk> inline
wide_char_t CharCodeConvTraits<char_dbc_t, ErrCk>::peekChr(const_pointer str, const_pointer end) {
    unsigned char const* s = (unsigned char const*)str;
    unsigned char const* e = (unsigned char const*)end;
    if (s >= e) return -1;
    wide_char_t c = *s++;
    if (isLead(c)) {
        if (s < e && ((ErrCk && isTrail(*s)) || (!ErrCk && *s))) {
            c = (c << 8) | *s;
        } else if (ErrCk) {
            c = -2;
        }
    }
    return c;
}

template<bool ErrCk> typename CharCodeConvTraits<char_dbc_t, ErrCk>::pointer
CharCodeConvTraits<char_dbc_t, ErrCk>::setChr(pointer str, const_pointer end, wide_char_type c) {
    unsigned char* d = (unsigned char*)str;
    unsigned char* e = (unsigned char*)end;
    if (c > 0xff) {
        if (d+2 > e) goto ERR_RET;
        *d++ = c >> 8;
        *d++ = c;
    } else {
        if (d >= e) goto ERR_RET;
        *d++ = c;
    }
    return (pointer)d;
ERR_RET:
    if (d < e)
        *d = 0;
    return (pointer)e;
}
#endif  // _WIN32


#if defined(CCWRAP_CHAR_IS_UTF8)   // utf8

template<bool ErrCk>
class CharCodeConvTraits<char, ErrCk> {
    typedef CharCodeConvTraits<char8_t,ErrCk> Orig;
public:
    typedef char                value_type;
    typedef wide_char_t         wide_char_type;
    typedef size_t              size_type;
    typedef value_type*         pointer;
    typedef value_type const*   const_pointer;
    static bool             isLead(value_type c) { return Orig::isLead( c); }
    static bool             isLead(const_pointer s) { return Orig::isLead((value_type const*)s); }
    static wide_char_type   getChr(const_pointer& s, const_pointer e) {
                                return Orig::getChr((typename Orig::const_pointer&)s, (typename Orig::const_pointer)e);
                            }
    static wide_char_type   peekChr(const_pointer s, const_pointer e) { return getChr(s, e); }
    static pointer          setChr(pointer d, const_pointer e, wide_char_type c) {
                                return Orig::setChr((typename Orig::pointer)d, (typename Orig::const_pointer)e, c);
                            }
    static size_type        chrPtrBytes(const_pointer s) { return Orig::chrPtrBytes(s); }
    static size_type        chrBytes(wide_char_type c) { return Orig::chrBytes(c); }
    static size_type        chrWidth(wide_char_type c) { return __det_CharConvTraits::unicode_chWidth<void>(c); }
    static bool             checkChr(wide_char_type c) { return __det_CharConvTraits::unicode_checkCh<void>(c); }
};

#elif defined(_WIN32) && !defined(CCWRAP_CHAR_IS_UTF8)
template<bool ErrCk>
class CharCodeConvTraits<char, ErrCk> {
    typedef CharCodeConvTraits<char_dbc_t, ErrCk>   BASE;
public:
    typedef char                value_type;
    typedef wide_char_t         wide_char_type;
    typedef size_t              size_type;
    typedef value_type*         pointer;
    typedef value_type const*   const_pointer;
    static wide_char_type       getChr(const_pointer& s, const_pointer e)
                                { return BASE::getChr((char_dbc_t const*&)s, (char_dbc_t const*)e); }
    static wide_char_type       peekChr(const_pointer s, const_pointer e)
                                { return BASE::peekChr((char_dbc_t const*)s, (char_dbc_t const*)e); }
    static pointer              setChr(pointer d, const_pointer e, wide_char_type c)
                                { return (pointer)BASE::setChr((char_dbc_t*)d,(char_dbc_t const*)e,c); }
    static size_type            chrPtrBytes(const_pointer s) { return BASE::chrPtrBytes((char_dbc_t const*)s); }
    static size_type            chrBytes(wide_char_type c) { return BASE::chrBytes(c); }
    static size_type            chrWidth(wide_char_type c) { return BASE::chrWidth(c); }
    static bool                 checkChr(wide_char_type c) { return BASE::checkChr(c); }
};
#endif  // CCWRAP_CHAR_IS_UTF8

}   // namespace __ccwrap


// ============================================================================
// stringCodeConv

namespace ccwrap {

template<class DC, class SC>
size_t  stringCodeCalcCapacity(SC const* src, std::size_t size) {
    SC const* s = src;
    SC const* e = src + size;
    size_t l = 0;
    while (s < e) {
        wide_char_t c = CharCodeConvTraits<SC>::getChr(s, e);
        if (c < 0)
            break;
        l += CharCodeConvTraits<DC>::chrBytes(c);
    }
    return l / sizeof(DC);
}
template<class DC, class SC> size_t  stringCodeCalcCapacity(basic_string_view<SC> src) {
    return stringCodeCalcCapacity<DC,SC>(src.data(), src.size());
}
//template<class DC, class SC> size_t  stringCodeCalcCapacity(basic_string<SC> const& src) {
//  return stringCodeCalcCapacity<DC,SC>(src.data(), src.size());
//}

template<class SC>
bool  stringCodeCheck(SC const* src, std::size_t size) {
    SC const* s = src;
    SC const* e = src + size;
    while (s < e) {
        wide_char_t c = CharCodeConvTraits<SC>::getChr(s, e);
        if (c < 0) {
            if (c == -2)
                return false;
            break;
        }
    }
    return true;
}
template<class SC> bool  stringCodeCheck(basic_string_view<SC> src) {
    return stringCodeCheck<SC>(src.data(), src.size());
}
//template<class SC> bool  stringCodeCheck(basic_string<SC> const& src) {
//  return stringCodeCheck<SC>(src.data(), src.size());
//}

template<class DS, class SC>
DS  stringCodeConv(SC const* src, size_t srcSize) {
    typedef typename DS::value_type DC;
    size_t  sz = stringCodeCalcCapacity<DC,SC>(src, srcSize);
    DS      dst;
    dst.resize(sz+1);
    DC*       db = (DC*)dst.data();
    DC*       dp = db;
    DC*       de = db + sz;
    SC const* sb = src;
    SC const* sp = sb;
    SC const* se = sb + srcSize;
    while (dp < de) {
        wide_char_t c = CharCodeConvTraits<SC>::getChr(sp, se);
        if (c < 0) {
            if (sp < se)
                continue;
            break;
        }
        dp = CharCodeConvTraits<DC>::setChr(dp, de, c);
    }
    *dp = 0;
    dst.resize(de - db);
    return dst;
}

template<class DS, class SC>
DS  stringCodeConv(basic_string_view<SC> src) {
    return stringCodeConv<DS,SC>(src.data(), src.size());
}

template<class DS, class SS>
inline DS stringCodeConv(SS src, typename SS::value_type const* = nullptr){
    return stringCodeConv<DS,typename SS::value_type>(src);
}

template<class DS, class SC>
inline DS stringCodeConv(SC const*  src) {
    return stringCodeConv<DS,SC>(basic_string_view<SC>(src));
}

#if 1
#define __CCWRAP_SAME_STRING_CODE_CONV_DEF(S)    \
    template<> inline S stringCodeConv<S>(basic_string_view<S::value_type> src) { return S(src.data(), src.size()); }
__CCWRAP_SAME_STRING_CODE_CONV_DEF(string)
__CCWRAP_SAME_STRING_CODE_CONV_DEF(wstring)
__CCWRAP_SAME_STRING_CODE_CONV_DEF(u8string)
__CCWRAP_SAME_STRING_CODE_CONV_DEF(u16string)
__CCWRAP_SAME_STRING_CODE_CONV_DEF(u32string)
#undef __CCWRAP_SAME_STRING_CODE_CONV_DEF
#endif


#if !defined(CCWRAP_CHAR_IS_UTF8)

#if defined(_WIN32)

namespace __det_StringCodeConv {
    template<class DMY>
    wstring  conv_to_wstring(char const* src, size_t size) {
        enum { cp = 0 };
        size_t   wl = MultiByteToWideChar(cp,0,src,int(size),nullptr,0);
        wstring wbuf;
        wbuf.resize(wl + 1);
        if (wl > 0) {
            MultiByteToWideChar(cp,0,src,int(size),&wbuf[0], int(wbuf.size()));
            wbuf[wl] = 0;
        }
        wbuf.resize(wl);
        return wbuf;
    }

    template<class DMY>
    string   conv_to_string(wchar_t const* src, size_t size) {
        enum { cp = 0 };
        size_t bl = WideCharToMultiByte(cp,0,src,int(size),nullptr,0,0,0);
        string  buf;
        buf.resize(bl + 1);
        if (bl > 0) {
            WideCharToMultiByte(cp,0,src,int(size),&buf[0],int(buf.size()),0,0);
            buf[bl] = 0;
        }
        buf.resize(bl);
        return buf;
    }
}

#else   // std

namespace __det_StringCodeConv {
    template<class DMY>
    wstring  conv_to_wstring(char const* src, size_t size) {
        enum { cp = 0 };
        size_t   wl = std::mbstowcs(nullptr, src, size);
        wstring wbuf;
        if (wl > 0 && wl != size_t(-1)) {
            wbuf.resize(wl + 1);
            std::mbstowcs(&wbuf[0], src, size);
            wbuf[wl] = 0;
        }
        wbuf.resize(wl);
        return wbuf;
    }

    template<class DMY>
    string   conv_to_string(wchar_t const* src, size_t size) {
        size_t bl = std::wcstombs(nullptr, src, size);
        string  buf;
        if (bl > 0 && bl != size_t(-1)) {
            buf.resize(bl + 1);
            std::wcstombs(&buf[0], src, size);
            buf[bl] = 0;
        }
        buf.resize(bl);
        return buf;
    }
}

#endif  // _WIN32

template<>
inline wstring  stringCodeConv<wstring>(char const* src, size_t size) {
    return __det_StringCodeConv::conv_to_wstring<void>(src, size);
}

template<>
inline string   stringCodeConv<string>(wchar_t const* src, size_t size) {
    return __det_StringCodeConv::conv_to_string<void>(src, size);
}

template<>
inline u8string stringCodeConv<u8string>(char const* src, size_t size) {
 #if !defined(CCWRAP_CHAR_IS_UTF8)
    wstring ws = stringCodeConv<wstring>(src, size);
    return stringCodeConv<u8string>(ws);
 #else
    return u8string((char8_t const*)src, size);
 #endif
}

template<>
inline string   stringCodeConv<string>(char8_t const* src, size_t size) {
 #if !defined(CCWRAP_CHAR_IS_UTF8)
    wstring ws = stringCodeConv<wstring>(src, size);
    return stringCodeConv<string>(ws);
 #else
    return string((char const*)src, size);
 #endif
}

#if defined(_WIN32) || (CCWRAP_WCHAR_BIT == 16)

#if 1 //!defined(CCWRAP_WCHAR_IS_NOT_UNICODE)
template<>
inline wstring   stringCodeConv<wstring>(char16_t const* src, size_t size) {
    return wstring((wchar_t const*)src, size);
}

template<>
inline u16string   stringCodeConv<u16string>(wchar_t const* src, size_t size) {
    return u16string((char16_t const*)src, size);
}

template<>
inline u16string    stringCodeConv<u16string>(char const* src, size_t size) {
    wstring ws = stringCodeConv<wstring>(src, size);
    return (u16string&)(ws);
}

template<>
inline string   stringCodeConv<string>(char16_t const* src, size_t size) {
    return stringCodeConv<string>((wchar_t const*)src, size);
}
#endif

template<>
inline u32string    stringCodeConv<u32string>(char const* src, size_t size) {
    wstring ws = stringCodeConv<wstring>(src, size);
    return stringCodeConv<u32string>(ws);
}

template<>
inline string   stringCodeConv<string>(char32_t const* src, size_t size) {
    wstring ws = stringCodeConv<wstring>(src, size);
    return stringCodeConv<string>(ws);
}

#else   // defined(CCWRAP_WCHAR_T_IS_2BYTES) || defined(_WIN32)

#if 1 //!defined(CCWRAP_WCHAR_IS_NOT_UNICODE)
template<>
inline wstring   stringCodeConv<wstring>(char32_t const* src, size_t size) {
    return wstring((wchar_t const*)src, size);
}

template<>
inline u32string   stringCodeConv<u32string>(wchar_t const* src, size_t size) {
    return u16string((char32_t const*)src, size);
}

template<>
inline u32string    stringCodeConv<u32string>(char const* src, size_t size) {
    return reinterpret_cast<u32string const&>(stringCodeConv<wstring>(src, size));
}

template<>
inline string   stringCodeConv<string>(char32_t const* src, size_t size) {
    return stringCodeConv<string>((wchar_t const*)src, size);
}

#endif

template<>
inline u16string    stringCodeConv<u16string>(char const* src, size_t size) {
    wstring ws = stringCodeConv<wstring>(src, size);
    return stringCodeConv<u16string>(ws);
}

template<>
inline string   stringCodeConv<string>(char16_t const* src, size_t size) {
    wstring ws = stringCodeConv<wstring>(src, size);
    return stringCodeConv<string>(ws);
}

#endif  // defined(CCWRAP_WCHAR_T_IS_2BYTES) || defined(_WIN32)

#endif  // CCWRAP_CHAR_IS_UTF8

}   // namespace ccwrap


// ============================================================================

#if defined(_WIN32) && !defined(CCWRAP_CHAR_IS_UTF8)
 #include "StringCodeJapaneseMbs.hh"
#endif

namespace ccwrap {

namespace __det_CharConvTraits {
    template<class DMY>
    int checkUnicodeBOM(char const* src, size_t len) {
        unsigned char const* b = (unsigned char const*)src;
        if (len < 2)
            return 0;
        // Check Unicode BOM
        if (len >= 3 && b[0] == 0xEF && b[1] == 0xBB && b[2] == 0xBF) {
            return 1;       // UTF8:BOM
        } else if (len >= 2 && b[0] == 0xFF && b[1] == 0xFE) {
            if (len >= 4 && b[2] == 0x00 && b[3] == 0x00) {
                return 4; // UTF32LE:BOM
            } else {
                return 2;   // UTF16LE:BOM
            }
        } else if (len >= 2 && b[0] == 0xFE && b[1] == 0xFF) {
            return 3;       // UTF16BE:BOM
        } else if (len >= 4 && b[0] == 0x00 && b[1] == 0x00 && b[2] == 0xFE && b[3] == 0xFF) {
            return 5;       // UTF32BE:BOM
        }
        return 0;
    }
}   // __det_CharConvTraits

inline int checkUnicodeBOM(char const* src, size_t len) {
    return __det_CharConvTraits::checkUnicodeBOM<void>(src, len);
}
inline int checkUnicodeBOM(string_view str) {
    return __det_CharConvTraits::checkUnicodeBOM<void>(str.data(), str.size());
}

template<class STR>
STR   autoStringCodeConv(char const* src, size_t len) {
    int rc = checkUnicodeBOM(src, len);
    switch (rc) {
    case 1: return stringCodeConv<STR>((char8_t const*)src, len);
    case 2: return stringCodeConv<STR>((char16le_t const*)src, len/sizeof(char16le_t));
    case 3: return stringCodeConv<STR>((char16be_t const*)src, len/sizeof(char16be_t));
    case 4: return stringCodeConv<STR>((char32le_t const*)src, len/sizeof(char32le_t));
    case 5: return stringCodeConv<STR>((char32be_t const*)src, len/sizeof(char32be_t));
    default:
     #if !defined(CCWRAP_CHAR_IS_UTF8)
        if (stringCodeCheck((char8_t const*)src, len))
            return stringCodeConv<STR>((char8_t const*)src, len);
      #if 0 //defined(_WIN32)
        if (stringCodeCheck((char const*)src, len))
            return stringCodeConv<STR>((char const*)src, len);
      #endif
     #else
      #if defined(_WIN32)
        if (stringCodeCheck((char const*)src, len))
            return stringCodeConv<STR>((char const*)src, len);
        if (stringCodeCheck((char_dbc_t const*)src, len))
            return stringCodeConv<STR>((char_dbc_t const*)src, len);
      #endif
     #endif
        return stringCodeConv<STR>(src, len);
    }
}
template<class STR>
STR   autoStringCodeConv(string_view sv) {
    return autoStringCodeConv<STR>(sv.data(), sv.size());
}

}   // namespace ccwrap

#if defined(_MSC_VER) && _MSC_VER >= 1400
# pragma warning(pop)
#endif

// ============================================================================

#endif  // __CCWRAP_StringCodeConv_hpp_INCLUDED
