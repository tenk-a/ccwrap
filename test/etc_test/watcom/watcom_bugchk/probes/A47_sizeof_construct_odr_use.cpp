// bugchk-id: A47
// bugchk-title: sizeof of a construction ODR-uses the constructor
// bugchk-expect: bug
// A construction inside sizeof is an unevaluated operand, so nothing may be odr-used.
// Open Watcom emits a reference to the constructor anyway, so a type that only DECLARES
// a private copy constructor fails to LINK. Needs an object of the type in the same TU.
#include <stdio.h>

typedef char yes_t;
struct no_t { char pad[2]; };
template <class T> const T& clval();

template <class T> struct is_cc {
    template <class U> static yes_t __t(char(*)[sizeof( (U(clval<U>()), 0) )]);
    template <class U> static no_t  __t(...);
    enum { value = sizeof(__t<T>(0)) == sizeof(yes_t) };
};

struct NonCopyable {
    NonCopyable() {}
    ~NonCopyable() {}
private:
    NonCopyable(const NonCopyable&);   // declared, never defined
};

NonCopyable g_obj;                     // an object of the type must exist in the TU

int main() {
    // Linking this program is the test: if the constructor is referenced, the link fails
    // with "NonCopyable::NonCopyable( NonCopyable const & ) is an undefined reference".
    printf("%s\n", is_cc<NonCopyable>::value ? "bug: links only because the ctor was not referenced"
                                             : "ok");
    return 0;
}
