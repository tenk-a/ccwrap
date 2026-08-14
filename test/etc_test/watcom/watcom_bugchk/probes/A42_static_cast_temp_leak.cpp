// bugchk-id:     A42
// bugchk-kind:   run
// bugchk-expect: bug
// bugchk-desc:   a static_cast<Class>(x) temporary is never destroyed (leaks one per evaluation)
//
// static_cast to a CLASS type creates a temporary whose destructor is never run, so a
// loop doing it leaks one object per turn. The equivalent functional-cast spelling
// C(x) is fine, which is the control below -- so it is the static_cast form
// specifically, not temporaries in general.
//
// Found in libcxx03 deque: the fill insert did
//     for (k = 0; k < c; ++k) push_back(static_cast<_Tp>(__v));
// and leaked one element per inserted item. Rewritten to take one named copy first.
//
// Raw compiler only -- no ccwrap headers.
#include <stdio.h>

struct C {
    static int live;
    int v;
    C() : v(0)            { ++live; }
    explicit C(int x) : v(x) { ++live; }
    C(const C& o) : v(o.v) { ++live; }
    C& operator=(const C& o) { v = o.v; return *this; }
    ~C() { --live; }
};
int C::live = 0;

static void sink(const C&) {}

int main(void)
{
    int bad = 0;

    /* (1) static_cast<C>(x) as a call argument, in a loop */
    {
        C src(1);
        int before = C::live;
        for (int i = 0; i < 3; ++i)
            sink(static_cast<C>(src));
        printf("static_cast<C> in a loop : live delta = %d (want 0)\n", C::live - before);
        if (C::live != before) { puts("BROKEN: the cast temporaries were not destroyed"); bad = 1; }
    }

    /* (2) control: a plain copy temporary */
    {
        C src(1);
        int before = C::live;
        for (int i = 0; i < 3; ++i)
            sink(C(src));
        printf("C(src) in a loop         : live delta = %d (want 0)\n", C::live - before);
        if (C::live != before) { puts("BROKEN: even C(src) temporaries leak"); bad = 1; }
    }

    /* (3) control: assignment from a cast temporary */
    {
        C src(2), dst;
        int before = C::live;
        for (int i = 0; i < 3; ++i)
            dst = static_cast<C>(src);
        printf("dst = static_cast<C>(src): live delta = %d (want 0)\n", C::live - before);
        if (C::live != before) { puts("BROKEN: assignment-side cast temporaries leak"); bad = 1; }
    }

    if (!bad) puts("ok: temporaries are destroyed");
    return bad;
}
