// bugchk-id:     A01
// bugchk-kind:   run
// bugchk-expect: ok
// bugchk-desc:   vector built/destroyed in a loop -- GUARD: the catalogued crash does not reproduce standalone
#include <stdio.h>
#include <vector>

struct NonPod {
    int x;
    NonPod() : x(0) {}
    ~NonPod() { x = -1; }
};

static int loop_fn(int n) {
    int acc = 0;
    for (int i = 0; i < n; ++i) {
        std::vector<NonPod> v;              // constructed/destroyed every iteration
        v.push_back(NonPod());
        acc += (int)v.size();
    }
    return acc;
}

int main() {
    std::vector<NonPod> live;               // caller frame keeps a non-trivial local alive
    live.push_back(NonPod());
    int r = loop_fn(2000);
    printf("A01 loop vector: acc=%d (want 2000) -> %s\n", r, r == 2000 ? "OK" : "BROKEN");
    return r == 2000 ? 0 : 1;
}
