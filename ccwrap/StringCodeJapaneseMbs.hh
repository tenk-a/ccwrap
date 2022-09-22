/**
 *  @file StringCodeJapaneseMbs.hh
 *  @brief  Japanese multi-byte-char Stinrg encoding conversion
 *  @author Masashi Kitamura(tenka@6809.net)
 *  @date   2020
 */
#ifndef __CCWRAP_StringCodeJapaneseMbs_hh_INCLUDED
#define __CCWRAP_StringCodeJapaneseMbs_hh_INCLUDED

namespace ccwrap {

struct char_sjis_t { typedef unsigned char type; };

template<bool ErrCk>
class CharCodeConvTraits<char_sjis_t, ErrCk> {
public:
    typedef char_sjis_t         value_type;
    typedef wide_char_t         wide_char_type;
    typedef size_t              size_type;
    typedef value_type*         pointer;
    typedef value_type const*   const_pointer;

    static wide_char_type   getChr(const_pointer& s, const_pointer e);
    static wide_char_type   peekChr(const_pointer s, const_pointer e);
    static pointer          setChr(pointer d, const_pointer e, wide_char_type c);
    static size_type        chrPtrBytes(const_pointer s) { return size_type(isLead(*(unsigned char const*)s)) + 1; }
    static size_type        chrBytes(wide_char_type c) { return 1 + size_type(c > 0xff); }
    static size_type        chrWidth(wide_char_type c) { return 1 + size_type(c > 0xff); }
    static bool             checkChr(wide_char_type c) {
                                return (c > 0xff) ? (isLead((unsigned char)(c >> 8)) && isTrail((unsigned char)c))
                                                  : true;
                            }
private:
    static bool             isLead(unsigned c) { return (c >= 0x81) && (c <= 0xFC) && ((c <= 0x9F) || (c >= 0xE0)); }
    static bool             isTrail(unsigned c) { return (c >= 0x40) && (c <= 0xFC) && ((c <= 0x7e) || (c >= 0x81)); }
};

template<bool ErrCk> inline
wide_char_t CharCodeConvTraits<char_sjis_t, ErrCk>::getChr(const_pointer& rStr, const_pointer end) {
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
wide_char_t CharCodeConvTraits<char_sjis_t, ErrCk>::peekChr(const_pointer str, const_pointer end) {
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

template<bool ErrCk> typename CharCodeConvTraits<char_sjis_t, ErrCk>::pointer
CharCodeConvTraits<char_sjis_t, ErrCk>::setChr(pointer str, const_pointer end, wide_char_type c) {
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


//  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -

struct char_eucjp_t { typedef unsigned char type; };

template<bool ErrCk>
class CharCodeConvTraits<char_eucjp_t, ErrCk> {
public:
    typedef char_eucjp_t        value_type;
    typedef wide_char_t         wide_char_type;
    typedef size_t              size_type;
    typedef value_type*         pointer;
    typedef value_type const*   const_pointer;

    static wide_char_type   getChr(const_pointer& s, const_pointer e);
    static wide_char_type   peekChr(const_pointer s, const_pointer e) { return getChr(s, e); }
    static pointer          setChr(pointer d, const_pointer e, wide_char_type c);
    static size_type        chrPtrBytes(const_pointer s) { return size_type(isLead(*(unsigned char const*)s)) + 1; }
    static size_type        chrBytes(wide_char_type c) { return 1 + size_type(c > 0xff) + size_type(c > 0xffff); }
    static size_type        chrWidth(wide_char_type c);
    static bool             checkChr(wide_char_type c);
private:
    static bool             isLead(unsigned c) { return (c >= 0xA1 && c <= 0xFE); }
};

template<bool ErrCk>
wide_char_t CharCodeConvTraits<char_eucjp_t, ErrCk>::getChr(const_pointer& rStr, const_pointer end) {
    unsigned char const* s = (unsigned char const*)rStr;
    unsigned char const* e = (unsigned char const*)end;
    wide_char_t c, k;
    if (s >= e) goto END_ERR;
    c = *s++;
    k = c;
    if (c == 0x8f) {
        if (s+1 >= e) goto END_ERR;
        k <<= 16;
        c = *s++;
        if (!isLead(c)) goto CODE_ERR;
    }
    if (isLead(c)) {
        if (s >= e) goto END_ERR;
        if ((ErrCk && isLead(*s)) || (!ErrCk && *s)) {
            k |= (c << 8) | *s;
            ++s;
        } else if (ErrCk) {
            goto CODE_ERR;
        }
    }
    rStr = (const_pointer)s;
    return k;
 END_ERR:
    rStr = end;
    return -1;
 CODE_ERR:
    rStr = (const_pointer)s;
    return -2;
}

template<bool ErrCk> typename CharCodeConvTraits<char_eucjp_t, ErrCk>::pointer
CharCodeConvTraits<char_eucjp_t, ErrCk>::setChr(pointer str, const_pointer end, wide_char_type c) {
    unsigned char* d = (unsigned char*)str;
    unsigned char* e = (unsigned char*)end;
    if (c > 0xff) {
        if (c > 0xffff) {
            if (d+3 > e) goto ERR_RET;
            *d++ = c >> 16;
        } else if (d+2 > e) {
            goto ERR_RET;
        }
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

template<bool ErrCk>
bool    CharCodeConvTraits<char_eucjp_t, ErrCk>::checkChr(wide_char_type c) {
    if (c <= 0xff)
        return true;
    if (c > 0xffff) {
        if ((c >> 16) != 0x8f)
            return false;
    }
    return isLead((unsigned char)(c >> 8)) && isLead((unsigned char)c);
}

template<bool ErrCk>
size_t CharCodeConvTraits<char_eucjp_t, ErrCk>::chrWidth(wide_char_type c) {
    unsigned h = c >> 8;
    if (h == 0 || h == 0x8E) {
        return 1;
    }
    return 2;
}

}   // ccwrap


#endif  // __CCWRAP_StringCodeJapaneseMbs_hh_INCLUDED
