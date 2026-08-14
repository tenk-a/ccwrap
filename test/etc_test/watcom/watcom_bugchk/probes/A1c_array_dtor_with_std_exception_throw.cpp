// A1'': an array of objects with a non-trivial destructor gets its caller's frame
// corrupted when one of the class's member functions contains a `throw` of a type
// derived from std::exception.  The objects themselves stay healthy (the live
// counter and the heap are both intact right up to the fault) -- what breaks is
// the stack frame, which is why the runtime reports "stack data has been corrupted".
//
// Three ingredients are all required (measured by removing each):
//   1. the objects live in an ARRAY (two separate variables do not reproduce)
//   2. a member function throws a type DERIVED FROM std::exception
//      (throwing an int, or a class with virtuals but no std::exception base, is fine)
//   3. the class has a non-trivial destructor
// The throw is never executed; only its presence in the state table matters.
//
// bug: dies with "stack data has been corrupted!" (or an access violation)
// ok : prints "ok"
#include <stdio.h>
#include <exception>

struct E : public std::exception { E() {} virtual ~E() {} };

static int g_live = 0;

struct Payload {
    char v_;
    Payload(char c) : v_(c) { ++g_live; }
    Payload(const Payload& o) : v_(o.v_) { ++g_live; }
    Payload& operator=(const Payload& o) { v_ = o.v_; return *this; }
    ~Payload() { --g_live; }
    int get() const { return (int)v_; }
};

class Holder {
    Payload* p_;
public:
    Holder() : p_(0) {}
    Holder(const Payload& v) : p_(new Payload(v)) {}
    Holder(const Holder& o) : p_(o.p_ ? new Payload(*o.p_) : 0) {}
    Holder& operator=(const Holder& o) {
        if (this != &o) { delete p_; p_ = o.p_ ? new Payload(*o.p_) : 0; }
        return *this;
    }
    ~Holder() { delete p_; }
    int call(int x) const { if (!p_) throw E(); return x + p_->get(); }
};

int main(void) {
    int r0 = 0, r1 = 0;
    {
        Payload src(1);
        Holder f = src;
        Holder c(f);
        Payload other(2);
        c = other;
        {
            Holder hv[2];
            hv[0] = f;
            hv[1] = c;
            r0 = hv[0].call(10);
            r1 = hv[1].call(10);
        }
    }
    if (r0 == 11 && r1 == 12 && g_live == 0) {
        printf("ok\n");
        return 0;
    }
    printf("bug: r0=%d r1=%d live=%d\n", r0, r1, g_live);
    return 1;
}
