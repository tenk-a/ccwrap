// A30: assigning to an EBO'd empty base subobject clobbers the derived class's first
// member's low byte. EBO gives the base 0 bytes in the layout, but the base's
// operator= still copies sizeof(base)==1 byte at offset 0.
//
// bug: prints "bug" (a.p was corrupted by the base assignment)
// ok : prints "ok"  (base assignment wrote nothing into the derived object)
#include <cstdio>

struct E {};                        // empty base
struct D : E { void* p; };          // EBO candidate: sizeof(D) == sizeof(void*)

int main() {
    D a; a.p = (void*)0x11223344;
    D b; b.p = (void*)0x55667788;
    *(E*)&a = *(E*)&b;              // assign only the empty base subobject
    std::printf(a.p == (void*)0x11223344 ? "ok\n" : "bug\n");
    return 0;
}
