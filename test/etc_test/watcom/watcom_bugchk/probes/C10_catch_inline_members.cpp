// bugchk-id:     C10
// bugchk-kind:   run
// bugchk-expect: bug
// bugchk-aux:    extra/C10_catch_other_tu.cpp
// bugchk-desc:   a catch handler's type table names the destructor but no body is emitted
//
// C10: a translation unit that only CATCHES a type whose members are all defined
// in-class cannot be linked.  The catch handler's type table emits EXTRN references
// to the default constructor, the copy constructor and the destructor; Open Watcom
// emits bodies for the two constructors but never for the destructor.
//
// Two TUs are needed: the throw side odr-uses the constructor and drags the whole
// class out, which hides the bug.  Here nothing throws, so main.obj is the only
// module naming the type and the link fails with
//     E2028: __near ns::my_exc::~my_exc() is an undefined reference
//
// Raw compiler only, no ccwrap headers.  The cure ccwrap uses is the anchor in
// detail/llibcxx03/include/__exception/eh_anchor.h -- see the bottom of this file
// for the shape of it (compile with -DCCW_C10_ANCHOR to check that it still works).
#include <stdio.h>

namespace ns {
class my_exc {
public:
    my_exc() {}
    virtual ~my_exc() {}
    virtual const char* what() const { return "my_exc"; }
};
class my_derived : public my_exc {
public:
    my_derived() {}
    virtual ~my_derived() {}
    virtual const char* what() const { return "my_derived"; }
};
}

#ifdef CCW_C10_ANCHOR
namespace {
void ccw_dtor_exc(ns::my_exc* p) { p->ns::my_exc::~my_exc(); }
void ccw_dtor_der(ns::my_derived* p) { p->ns::my_derived::~my_derived(); }
void* const ccw_anchor_exc = (void*)&ccw_dtor_exc;
void* const ccw_anchor_der = (void*)&ccw_dtor_der;
}
#endif

void g();

int main()
{
    try {
        g();
    } catch (ns::my_derived&) {
        printf("BROKEN: unreachable\n");
        return 1;
    } catch (ns::my_exc&) {
        printf("BROKEN: unreachable\n");
        return 1;
    }
    printf("ok: a catch-only TU linked\n");
    return 0;
}
