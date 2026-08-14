// A32: combining a sizeof-of-a-SFINAE-call comparison with a dependent trait value in a
// SINGLE enum initializer via `&&` is miscompiled. The left operand (a substitution-
// based detection that wrongly answers "yes" for a non-class) is not gated by the right
// operand (is-class = false), so the whole thing yields 1 for int. Computing each operand
// in its OWN enum member first, then ANDing, is correct.
//
// bug: prints "bug" (combo_bad<int> == 1, the && did not gate)
// ok : prints "ok"  (combo_bad<int> == 0)

#include <stdio.h>

typedef char yes_t;
struct no_t { char pad[2]; };

// nested-type detection (on Open Watcom this slips to "yes" for non-class types)
template <class T> yes_t f(int, typename T::type* = 0);
template <class T> no_t  f(long);

// pointer-to-member = only a class can form one
template <class T> yes_t g(int, void (T::*)(void) = 0);
template <class T> no_t  g(long);
template <class T> struct is_cls { enum { value = sizeof(g<T>(0)) == sizeof(yes_t) }; };

// BAD: sizeof-of-call comparison && dependent trait value, all in one enum initializer
template <class T> struct combo_bad {
    enum { value = (sizeof(f<T>(0)) == sizeof(yes_t)) && is_cls<T>::value };
};

int main(void)
{
    printf(combo_bad<int>::value == 0 ? "ok\n" : "bug\n");
    return 0;
}
