/**
 *  @file   detail/c/win/localeinfo.h
 *  @brief  GetLocaleInfoEx, resolved at run time, for the named-locale facets.
 *  @license Boost Software License Version 1.0
 */
#ifndef CCW_DETAIL_WIN_LOCALEINFO_H_INCLUDED__
#define CCW_DETAIL_WIN_LOCALEINFO_H_INCLUDED__

#include "win32_fwd.h"

#ifdef __cplusplus
namespace __ccw { extern "C" {
#endif
_CCW_WINAPI(__ccw_hmodule) GetModuleHandleA(const char*);
_CCW_WINAPI(__ccw_farproc_t) GetProcAddress(__ccw_hmodule, const char*);
#ifdef __cplusplus
} }
#endif

#if defined(__cplusplus)
#  define _CCW_LI_NS  ::__ccw::
#  define _CCW_LI_FN  inline
#else
#  define _CCW_LI_NS
#  define _CCW_LI_FN  static _ccw_forceinline
#endif

#define _CCW_LI_ILANGUAGE           0x00000001uL
#define _CCW_LI_RETURN_NUMBER       0x20000000uL
#define _CCW_LI_CUSTOM_UNSPECIFIED  0x00001000uL
#define _CCW_LI_SDECIMAL            0x0000000EuL
#define _CCW_LI_STHOUSAND           0x0000000FuL
#define _CCW_LI_SGROUPING           0x00000010uL
#define _CCW_LI_SCURRENCY           0x00000014uL
#define _CCW_LI_SINTLSYMBOL         0x00000015uL
#define _CCW_LI_SMONDECIMALSEP      0x00000016uL
#define _CCW_LI_SMONTHOUSANDSEP     0x00000017uL
#define _CCW_LI_SMONGROUPING        0x00000018uL
#define _CCW_LI_ICURRDIGITS         0x00000019uL
#define _CCW_LI_IINTLCURRDIGITS     0x0000001AuL
#define _CCW_LI_ICURRENCY           0x0000001BuL
#define _CCW_LI_INEGCURR            0x0000001CuL
#define _CCW_LI_S1159               0x00000028uL
#define _CCW_LI_S2359               0x00000029uL
#define _CCW_LI_SDAYNAME1           0x0000002AuL
#define _CCW_LI_SABBREVDAYNAME1     0x00000031uL
#define _CCW_LI_SMONTHNAME1         0x00000038uL
#define _CCW_LI_SABBREVMONTHNAME1   0x00000044uL
#define _CCW_LI_SPOSITIVESIGN       0x00000050uL
#define _CCW_LI_SNEGATIVESIGN       0x00000051uL

typedef int (_ccw_stdcall* __ccw_getlocaleinfoex_t)(const wchar_t*, unsigned long, wchar_t*, int);
typedef int (_ccw_stdcall* __ccw_isvalidlocalename_t)(const wchar_t*);

_CCW_LI_FN __ccw_getlocaleinfoex_t __ccw_li_fn(void)
{
    static __ccw_getlocaleinfoex_t __fn;
    static int __resolved;
    if (!__resolved) {
        __ccw_hmodule __k = _CCW_LI_NS GetModuleHandleA("kernel32.dll");
        if (__k)
            __fn = (__ccw_getlocaleinfoex_t)
                   _CCW_LI_NS GetProcAddress(__k, "GetLocaleInfoEx");
        __resolved = 1;
    }
    return __fn;
}

_CCW_LI_FN int __ccw_li_name(const char* __name, wchar_t* __w, int __max)
{
    int __i = 0;
    if (!__name) return 0;
    for (; __name[__i] != 0 && __i + 1 < __max; ++__i) {
        char __c = __name[__i];
        if (__c == '.' || __c == '@') break;
        __w[__i] = (wchar_t)(unsigned char)(__c == '_' ? '-' : __c);
    }
    __w[__i] = 0;
    return __i;
}

_CCW_LI_FN int __ccw_win_locale_info(const char* __name, unsigned long __lct, wchar_t* __buf, int __n)
{
    __ccw_getlocaleinfoex_t __fn = __ccw_li_fn();
    wchar_t __w[96];
    if (!__fn) return 0;
    if (__ccw_li_name(__name, __w, 96) == 0) return 0;
    return __fn(__w, __lct, __buf, __n);
}

_CCW_LI_FN __ccw_isvalidlocalename_t __ccw_li_valid_fn(void)
{
    static __ccw_isvalidlocalename_t __fn;
    static int __resolved;
    if (!__resolved) {
        __ccw_hmodule __k = _CCW_LI_NS GetModuleHandleA("kernel32.dll");
        if (__k)
            __fn = (__ccw_isvalidlocalename_t)
                   _CCW_LI_NS GetProcAddress(__k, "IsValidLocaleName");
        __resolved = 1;
    }
    return __fn;
}

_CCW_LI_FN int __ccw_win_locale_valid(const char* __name)
{
    __ccw_getlocaleinfoex_t   __g = __ccw_li_fn();
    __ccw_isvalidlocalename_t __v = __ccw_li_valid_fn();
    wchar_t                   __w[96];
    unsigned long             __lang = 0;
    if (__ccw_li_name(__name, __w, 96) == 0) return 0;
    if (__v && !__v(__w)) return 0;
    if (!__g) return 0;
    if (__g(__w, _CCW_LI_ILANGUAGE | _CCW_LI_RETURN_NUMBER,
            (wchar_t*)(void*)&__lang, (int)(sizeof(__lang) / sizeof(wchar_t))) <= 0) return 0;
    return __lang != 0 && __lang != _CCW_LI_CUSTOM_UNSPECIFIED;
}

#define _CCW_LI_LCMAP_SORTKEY   0x00000400uL

typedef int (_ccw_stdcall* __ccw_comparestringex_t)(const wchar_t*, unsigned long,
                                                    const wchar_t*, int, const wchar_t*, int,
                                                    void*, void*, long);
typedef int (_ccw_stdcall* __ccw_lcmapstringex_t)(const wchar_t*, unsigned long,
                                                  const wchar_t*, int, void*, int,
                                                  void*, void*, long);

_CCW_LI_FN void* __ccw_li_k32(const char* __sym)
{
    __ccw_hmodule __k = _CCW_LI_NS GetModuleHandleA("kernel32.dll");
    return __k ? (void*)_CCW_LI_NS GetProcAddress(__k, __sym) : (void*)0;
}

_CCW_LI_FN int __ccw_win_compare_str(const char* __n, const wchar_t* __a, int __na,
                                     const wchar_t* __b, int __nb)
{
    static __ccw_comparestringex_t __fn;
    static int                     __resolved;
    wchar_t                        __w[96];
    int                            __r;
    if (!__resolved) { __fn = (__ccw_comparestringex_t)__ccw_li_k32("CompareStringEx"); __resolved = 1; }
    if (!__fn) return -2;
    if (__ccw_li_name(__n, __w, 96) == 0) return -2;
    __r = __fn(__w, 0uL, __a, __na, __b, __nb, (void*)0, (void*)0, 0L);
    return (__r == 0) ? -2 : (__r - 2);
}

_CCW_LI_FN int __ccw_win_sort_key(const char* __n, const wchar_t* __s, int __ns,
                                  void* __dst, int __cb)
{
    static __ccw_lcmapstringex_t __fn;
    static int                   __resolved;
    wchar_t                      __w[96];
    if (!__resolved) { __fn = (__ccw_lcmapstringex_t)__ccw_li_k32("LCMapStringEx"); __resolved = 1; }
    if (!__fn) return 0;
    if (__ccw_li_name(__n, __w, 96) == 0) return 0;
    return __fn(__w, _CCW_LI_LCMAP_SORTKEY, __s, __ns, __dst, __cb, (void*)0, (void*)0, 0L);
}

#define _CCW_LI_LCMAP_LOWERCASE 0x00000100uL
#define _CCW_LI_LCMAP_UPPERCASE 0x00000200uL
#define _CCW_LI_IDEFAULTANSICP  0x00001004uL
#define _CCW_LI_CT_CTYPE1       1uL

#define _CCW_LI_C1_UPPER   0x0001u
#define _CCW_LI_C1_LOWER   0x0002u
#define _CCW_LI_C1_DIGIT   0x0004u
#define _CCW_LI_C1_SPACE   0x0008u
#define _CCW_LI_C1_PUNCT   0x0010u
#define _CCW_LI_C1_CNTRL   0x0020u
#define _CCW_LI_C1_BLANK   0x0040u
#define _CCW_LI_C1_XDIGIT  0x0080u
#define _CCW_LI_C1_ALPHA   0x0100u

typedef int (_ccw_stdcall* __ccw_getstringtypew_t)(unsigned long, const wchar_t*, int, unsigned short*);
typedef int (_ccw_stdcall* __ccw_mb2wc_t)(unsigned, unsigned long, const char*, int, wchar_t*, int);

_CCW_LI_FN int __ccw_win_str_type(const wchar_t* __s, int __n, unsigned short* __out_)
{
    static __ccw_getstringtypew_t __fn;
    static int                    __resolved;
    if (!__resolved) { __fn = (__ccw_getstringtypew_t)__ccw_li_k32("GetStringTypeW"); __resolved = 1; }
    return __fn ? __fn(_CCW_LI_CT_CTYPE1, __s, __n, __out_) : 0;
}

_CCW_LI_FN int __ccw_win_byte_to_wide(unsigned __cp, const char* __s, int __n, wchar_t* __d, int __dn)
{
    static __ccw_mb2wc_t __fn;
    static int           __resolved;
    if (!__resolved) { __fn = (__ccw_mb2wc_t)__ccw_li_k32("MultiByteToWideChar"); __resolved = 1; }
    return __fn ? __fn(__cp, 0uL, __s, __n, __d, __dn) : 0;
}

typedef int (_ccw_stdcall* __ccw_wc2mb_t)(unsigned, unsigned long, const wchar_t*, int, char*, int, const char*, int*);

_CCW_LI_FN int __ccw_win_wide_to_byte(unsigned __cp, const wchar_t* __s, int __n, char* __d, int __dn)
{
    static __ccw_wc2mb_t __fn;
    static int           __resolved;
    if (!__resolved) { __fn = (__ccw_wc2mb_t)__ccw_li_k32("WideCharToMultiByte"); __resolved = 1; }
    return __fn ? __fn(__cp, 0uL, __s, __n, __d, __dn, (const char*)0, (int*)0) : 0;
}

_CCW_LI_FN unsigned __ccw_win_ansi_cp(const char* __n)
{
    __ccw_getlocaleinfoex_t __g = __ccw_li_fn();
    wchar_t                 __w[96];
    unsigned long           __cp = 0;
    if (!__g) return 0u;
    if (__ccw_li_name(__n, __w, 96) == 0) return 0u;
    if (__g(__w, _CCW_LI_IDEFAULTANSICP | _CCW_LI_RETURN_NUMBER,
            (wchar_t*)(void*)&__cp, (int)(sizeof(__cp) / sizeof(wchar_t))) <= 0) return 0u;
    return (unsigned)__cp;
}

_CCW_LI_FN int __ccw_win_map_case(const char* __n, const wchar_t* __s, int __ns, wchar_t* __d, int __nd, int __up)
{
    static __ccw_lcmapstringex_t __fn;
    static int                   __resolved;
    wchar_t                      __w[96];
    if (!__resolved) { __fn = (__ccw_lcmapstringex_t)__ccw_li_k32("LCMapStringEx"); __resolved = 1; }
    if (!__fn) return 0;
    if (__ccw_li_name(__n, __w, 96) == 0) return 0;
    return __fn(__w, __up ? _CCW_LI_LCMAP_UPPERCASE : _CCW_LI_LCMAP_LOWERCASE,
                __s, __ns, (void*)__d, __nd, (void*)0, (void*)0, 0L);
}

#undef _CCW_LI_FN
#undef _CCW_LI_NS

#endif /* CCW_DETAIL_WIN_LOCALEINFO_H_INCLUDED__ */
