// bugchk-id:     A11b
// bugchk-kind:   compile
// bugchk-expect: ok
// bugchk-desc:   recursive partial specialisation with a base case -- GUARD: catalogued E904 does not reproduce
struct nil {};
template <class H, class T> struct cons { typedef H head; typedef T tail; };

template <class L, int I> struct tfind { typedef typename tfind<typename L::tail, I - 1>::type type; };
template <class L>        struct tfind<L, 0> { typedef typename L::head type; };

typedef cons<int, cons<char, nil> > L2;

int main() {
    tfind<L2, 0>::type a = 1;
    tfind<L2, 1>::type b = 'x';
    return (a == 1 && b == 'x') ? 0 : 1;
}
