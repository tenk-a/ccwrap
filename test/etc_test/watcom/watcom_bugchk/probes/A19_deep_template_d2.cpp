// bugchk-id:     A19
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-flags:  -d2
// bugchk-desc:   -d2 (full debug info) cannot compile a deeply recursive template (<tuple>); -d1 is fine
// Debug-info generation for detail/tuple.hpp's recursion blew up with no diagnostic at all
// (bad status, exit 1). This is a stripped-down cons-list of the same shape.
struct nil {};
template <class H, class T> struct cons {
    H head;
    T tail;
    cons() : head(H()), tail(T()) {}
};

template <int N> struct build { typedef cons<int, typename build<N - 1>::type> type; };
template <>      struct build<0> { typedef nil type; };

template <class L> struct depth { enum { value = 1 + depth<typename L::tail>::value }; };
template <>        struct depth<nil> { enum { value = 0 }; };

typedef build<40>::type L40;

int main() {
    L40 l;
    (void)l;
    return depth<L40>::value == 40 ? 0 : 1;
}
