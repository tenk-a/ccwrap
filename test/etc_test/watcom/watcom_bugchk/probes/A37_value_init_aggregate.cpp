/* bugchk-id:     A37
 * bugchk-kind:   run
 * bugchk-expect: bug
 * bugchk-desc:   value-initialising an aggregate (T() / mem-init v_()) does not zero its members
 *
 * C++03 8.5/5: value-initialising a class with no user-declared constructor
 * value-initialises **every member**, i.e. the scalars become 0. Open Watcom leaves them
 * with whatever was on the stack -- for BOTH spellings:
 *
 *     Point p = Point();          // direct
 *     template<class T> struct H { T v_; H() : v_() {} };   // via a mem-initialiser
 *
 * A SCALAR T is handled correctly (`int` really does become 0), so the defect is
 * specific to class types. A8 already records the placement-new form
 * (`::new (buf) T()`); this is the same failure without placement new, which means the
 * A8 workaround ("copy-construct from a value-initialised temporary") does NOT help --
 * the temporary itself is not zeroed either.
 *
 * Found via std::atomic<Point>: libcxx03's `atomic() : __v_() {}` is correct C++, yet a
 * default-constructed atomic<Point> came out holding garbage.
 *
 * Raw compiler only -- no ccwrap headers.
 */
#include <stdio.h>

struct Point { int x; int y; };            /* no user-declared constructor: an aggregate */

template <class T> struct Holder {
    T v_;
    Holder() : v_() {}                     /* value-initialise through a mem-initialiser */
};

/* Dirty the stack so a missing initialisation shows up instead of reading an
   incidentally-zero frame. */
static void dirty_stack()
{
    volatile char buf[256];
    for (int i = 0; i < 256; ++i) buf[i] = (char)0xA5;
}

int main(void)
{
    int bad = 0;

    dirty_stack();
    {
        Holder<Point> h;
        Holder<int>   hi;
        printf("aggregate via mem-init : x=%d y=%d (want 0 0)\n", h.v_.x, h.v_.y);
        printf("scalar    via mem-init : %d (want 0)\n", hi.v_);
        if (h.v_.x != 0 || h.v_.y != 0) {
            puts("BROKEN: mem-init v_() left the aggregate's members uninitialised");
            bad = 1;
        }
        if (hi.v_ != 0) { puts("BROKEN: even a scalar was not value-initialised"); bad = 1; }
    }

    dirty_stack();
    {
        Point p = Point();
        printf("aggregate via T()      : x=%d y=%d (want 0 0)\n", p.x, p.y);
        if (p.x != 0 || p.y != 0) {
            puts("BROKEN: T() did not value-initialise the aggregate");
            bad = 1;
        }
    }

    if (!bad) puts("ok: value-initialisation zeroes aggregates");
    return bad;
}
