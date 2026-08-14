// ccwrap <wctype.h>
#pragma once

#include <ccwrap_common.h>

#include _CCW_NATIVE_UC_HEADER_PATH(wctype.h)

#if defined(_MSC_VER) && _MSC_VER < 1800

#include <string.h>     /* strcmp */

#ifdef __cplusplus
# define _CCW_WCT_FN inline
#else
# define _CCW_WCT_FN static __inline
#endif

_CCW_WCT_FN int _ccw_iswblank(wint_t __c) { return __c == L' ' || __c == L'\t'; }

_CCW_WCT_FN wctrans_t _ccw_wctrans(const char* __p) {
    if (strcmp(__p, "tolower") == 0) return (wctrans_t)1;
    if (strcmp(__p, "toupper") == 0) return (wctrans_t)2;
    return (wctrans_t)0;
}
_CCW_WCT_FN wint_t _ccw_towctrans(wint_t __c, wctrans_t __t) {
    if (__t == (wctrans_t)1) return towlower(__c);
    if (__t == (wctrans_t)2) return towupper(__c);
    return __c;
}

_CCW_WCT_FN wctype_t _ccw_wctype(const char* __p) {
    if (strcmp(__p, "alnum")  == 0) return (wctype_t)(_ALPHA | _DIGIT);
    if (strcmp(__p, "alpha")  == 0) return (wctype_t)(_ALPHA);
    if (strcmp(__p, "blank")  == 0) return (wctype_t)(_BLANK);
    if (strcmp(__p, "cntrl")  == 0) return (wctype_t)(_CONTROL);
    if (strcmp(__p, "digit")  == 0) return (wctype_t)(_DIGIT);
    if (strcmp(__p, "graph")  == 0) return (wctype_t)(_PUNCT | _ALPHA | _DIGIT);
    if (strcmp(__p, "lower")  == 0) return (wctype_t)(_LOWER);
    if (strcmp(__p, "print")  == 0) return (wctype_t)(_BLANK | _PUNCT | _ALPHA | _DIGIT);
    if (strcmp(__p, "punct")  == 0) return (wctype_t)(_PUNCT);
    if (strcmp(__p, "space")  == 0) return (wctype_t)(_SPACE);
    if (strcmp(__p, "upper")  == 0) return (wctype_t)(_UPPER);
    if (strcmp(__p, "xdigit") == 0) return (wctype_t)(_HEX);
    return (wctype_t)0;
}

#ifdef __cplusplus
#ifndef _CCW_WCTYPE_CXX_DEFINED
#define _CCW_WCTYPE_CXX_DEFINED
inline int       iswblank(wint_t __c)                 { return _ccw_iswblank(__c); }
inline wctrans_t wctrans(const char* __p)             { return _ccw_wctrans(__p); }
inline wint_t    towctrans(wint_t __c, wctrans_t __t) { return _ccw_towctrans(__c, __t); }
inline wctype_t  wctype(const char* __p)              { return _ccw_wctype(__p); }
#endif  /* _CCW_WCTYPE_CXX_DEFINED */
#else
#define iswblank(c)     _ccw_iswblank(c)
#define wctrans(p)      _ccw_wctrans(p)
#define towctrans(c, t) _ccw_towctrans((c), (t))
#define wctype(p)       _ccw_wctype(p)

#if defined(_CCW_C_CONFORM)
#undef iswprint
#define iswprint(c) (iswgraph(c) || (wint_t)(c) == L' ')
#endif
#endif  /* __cplusplus */

#undef _CCW_WCT_FN

#endif
