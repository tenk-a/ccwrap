/* bugchk-id:     B12
 * bugchk-kind:   compile
 * bugchk-expect: bug
 * bugchk-desc:   the `.template` disambiguator is rejected outright
 *
 * C++03 14.2/4 requires `obj.template f<T>(...)` when `obj` has a dependent type and `f`
 * is a member template; without it the `<` is parsed as less-than. Open Watcom does not
 * accept the keyword in that position at all, so this file fails to compile:
 *
 *     probe.cpp(N): Error! E929: syntax error: token 'template', but expected: 'id-expression'
 *
 * It does accept the undisambiguated `obj.f<T>(...)` inside a template (build with
 * -DUSE_PLAIN to see it pass) -- and that is exactly the spelling conforming compilers
 * reject ("use 'template' keyword to treat 'f' as a dependent template name" -- clang,
 * measured). So for a call written in a macro that is used from both template and
 * non-template contexts, NO single spelling works everywhere.
 *
 * Consequence: do not give a class template a member template that callers must invoke
 * with explicit template arguments. Use a plain (non-template) member, or a free function
 * template taking the object -- both avoid the disambiguator entirely. This is why the
 * test harness comparison holder (test/src/test.hpp, __TesT_cc_lr) uses plain members.
 *
 * Raw compiler only -- no ccwrap headers, no library.
 */
#include <stdio.h>

template<typename T, typename U>
struct H {
    T const& l_;
    U const& r_;
    H(T const& l, U const& r) : l_(l), r_(r) {}
    template<class DMY> void eq(char const* s) const { if (!(l_ == r_)) printf("ne %s\n", s); }
};

template<typename T, typename U>
inline H<T,U> mk(T const& l, U const& r) { return H<T,U>(l, r); }

#if defined(USE_PLAIN)
#  define t_eq(a,b)   mk(a,b).eq<void>(#a)              /* conforming compilers reject this */
#else
#  define t_eq(a,b)   mk(a,b).template eq<void>(#a)     /* Watcom rejects this (E929) */
#endif

/* object type is not dependent here */
static void plain() { int a = 1, b = 1; t_eq(a, b); }

/* object type IS dependent here -- 14.2/4 asks for `.template` */
template<class T>
static void in_template(T const& a, T const& b) { t_eq(a, b); }

int main() {
    plain();
    int x = 2, y = 2;
    in_template(x, y);
    printf("OK\n");
    return 0;
}
