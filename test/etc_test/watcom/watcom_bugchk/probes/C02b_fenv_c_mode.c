/* bugchk-id:     C02b
 * bugchk-kind:   compile
 * bugchk-expect: bug
 * bugchk-desc:   native <fenv.h> is empty in the default C dialect (its body is gated on __STDC_VERSION__ >= 199901L)
 *
 * The header includes cleanly -- with no error and no warning -- but every name in it
 * disappears, because wcc386 defaults to __STDC_VERSION__ == 199409L. In C++ the macro
 * is undefined, so the first term of the guard is true and <cfenv> works; only C breaks.
 * ccwrap does not fake __STDC_VERSION__ (Watcom refuses to #undef it, E1101); the rule is
 * that the user passes -za99. This probe compiles WITHOUT -za99 on purpose: it is `ok`
 * only if a future Watcom stops gating the header.
 */
#include <fenv.h>

int main(void) {
    fenv_t env;
    int x = FE_DIVBYZERO;
    feclearexcept(FE_ALL_EXCEPT);
    fegetenv(&env);
    return x ? 0 : 1;
}
