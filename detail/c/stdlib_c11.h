/* stdlib_c11.h - ccwrap detail: the C11 <stdlib.h> additions a Microsoft-family CRT
 * does not have.
 *
 * quick_exit / at_quick_exit are missing from the legacy MSVC CRT, Open Watcom, and
 * mingw-w64 configurations targeting msvcrt.  UCRT provides both and must not include
 * this supplement.  The fallback needs nothing beyond _exit(), so ccwrap can supply
 * it outright.
 *
 * Everything here is defined at GLOBAL scope and is language-neutral, so <stdlib.h>
 * (C and C++) and <cstdlib> can both use it; <cstdlib> then re-exports the names into
 * namespace std with a using-declaration. Writing the supplement only in <cstdlib>
 * would leave C without it (doc/ccwrap_impl_rule.md).
 *
 * SUBSET (documented, cf. doc/ccwrap_impl_rule.md): the registry is a fixed 32 slots,
 * which is the minimum C11 requires an implementation to accept. Registering beyond
 * that reports failure rather than growing, so nothing allocates on an exit path.
 * It is also per-translation-unit (`static`), so a handler registered in one TU is not
 * seen by a quick_exit() called from another. Handlers run in reverse order of
 * registration, then _exit() ends the process without flushing -- which is
 * quick_exit's whole point.
 */
#ifndef CCW_DETAIL_STDLIB_C11_H_INCLUDED__
#define CCW_DETAIL_STDLIB_C11_H_INCLUDED__

#ifndef _CCW_QUICK_EXIT_MAX
#define _CCW_QUICK_EXIT_MAX 32
#endif

#if defined(__cplusplus)
#  define _CCW_S11_FN  inline
#else
#  define _CCW_S11_FN  static _ccw_forceinline
#endif

#ifdef __cplusplus
extern "C" {
#endif

static void (*__ccw_qe_fn[_CCW_QUICK_EXIT_MAX])(void);
static int   __ccw_qe_n = 0;

_CCW_S11_FN int _ccw_cdecl at_quick_exit(void (*__f)(void)) {
    if (__f == 0 || __ccw_qe_n >= _CCW_QUICK_EXIT_MAX) return -1;
    __ccw_qe_fn[__ccw_qe_n++] = __f;
    return 0;
}
_CCW_S11_FN void _ccw_cdecl quick_exit(int __status) {
    while (__ccw_qe_n > 0) __ccw_qe_fn[--__ccw_qe_n]();
    _exit(__status);
}

#ifdef __cplusplus
}
#endif

#undef _CCW_S11_FN

#endif /* CCW_DETAIL_STDLIB_C11_H_INCLUDED__ */
