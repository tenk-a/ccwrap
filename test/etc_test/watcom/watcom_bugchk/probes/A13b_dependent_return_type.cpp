// bugchk-id:     A13b
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   a member template whose RETURN TYPE is a dependent type built from its own non-type parameter breaks the whole class instantiation
//
// A member function template may not spell
//     typename alt<(unsigned)I, T0, ...>::type&
// as its return type: instantiating the enclosing class -- without ever calling that
// member -- reports E351 / E121 inside alt<>, pointing at the conditional, with no
// mention of the caller. The SAME typedef written in the member BODY is fine, which is
// what -DSKIP_RET shows: drop only the return type and the file compiles.
//
// bug: E351 "type parameter supplied for a non-type argument" + E121 at the alt<> line
// ok : compiles

#include <stdio.h>

template <bool B, class T, class F> struct cond              { typedef T type; };
template <class T, class F>         struct cond<false, T, F> { typedef F type; };

template <unsigned I, class T0, class T1, class T2, class T3> struct alt {
    typedef typename cond<I == 2, T2, T3>::type    a23;
    typedef typename cond<I == 1, T1, a23>::type   a123;
    typedef typename cond<I == 0, T0, a123>::type  type;
};

struct nil1 {}; struct nil2 {}; struct nil3 {};

template <class T0, class T1 = nil1, class T2 = nil2, class T3 = nil3>
class box {
    char buf_[16];
    int  idx_;
public:
    box() : idx_(0) {}

#ifndef SKIP_RET
    template <unsigned long I>
    typename alt<(unsigned)I, T0, T1, T2, T3>::type& put() {
        typedef typename alt<(unsigned)I, T0, T1, T2, T3>::type V;
        idx_ = (int)I;
        return *reinterpret_cast<V*>(buf_);
    }
#endif

    template <unsigned long I, class A0>
    void set(A0 a0) {
        typedef typename alt<(unsigned)I, T0, T1, T2, T3>::type V;
        *reinterpret_cast<V*>(buf_) = (V)a0;
        idx_ = (int)I;
    }
    int index() const { return idx_; }
};

int main(void) {
    box<int, char> b;
    b.set<1, char>('x');
    printf("%s\n", b.index() == 1 ? "ok" : "bug");
    return 0;
}
