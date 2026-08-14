// bugchk-id:     A08
// bugchk-kind:   run
// bugchk-expect: bug
// bugchk-desc:   placement new T() does not value-initialise (variant's default ctor)
#include <stdio.h>
#include <new>

int main() {
    char buf[sizeof(int) * 4];
    for (unsigned i = 0; i < sizeof(buf); ++i) buf[i] = (char)0xCD;

    int* p = ::new (static_cast<void*>(buf)) int();     // () => value-initialised => 0
    int ok = (*p == 0);
    printf("A08 placement new int(): %d (want 0) -> %s\n", *p, ok ? "OK" : "BROKEN");
    return ok ? 0 : 1;
}
