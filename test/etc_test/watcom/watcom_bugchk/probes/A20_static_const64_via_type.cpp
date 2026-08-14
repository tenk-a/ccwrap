// bugchk-id:     A20
// bugchk-kind:   run
// bugchk-expect: bug
// bugchk-desc:   a 64-bit static const read through a nested ::type typedef gives garbage (<ratio>)
#include <stdio.h>

template <long long N, long long D>
struct ratio {
    static const long long num = N;
    static const long long den = D;
    typedef ratio<N, D> type;
};

// NB: do not add out-of-class definitions for num/den here -- combined with the
// self-referential `typedef ratio<N,D> type` they send Watcom into E640 (possible
// non-terminating template instantiation), which would mask the bug under test.

int main() {
    long long direct = ratio<2, 4>::num;          // known good
    long long via    = ratio<2, 4>::type::num;    // same entity, reached through ::type
    int ok = (direct == 2 && via == 2);
    printf("A20 num direct=%lld via ::type=%lld (want 2/2) -> %s\n", direct, via, ok ? "OK" : "BROKEN");
    return ok ? 0 : 1;
}
