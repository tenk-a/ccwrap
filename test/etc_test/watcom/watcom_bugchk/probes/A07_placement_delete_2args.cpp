// bugchk-id:     A07
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   two-argument placement operator delete is rejected (E267) -> nothrow new leaks
#include <stddef.h>

struct nothrow_t {};

void* operator new(size_t, const nothrow_t&) throw();
void  operator delete(void*, const nothrow_t&) throw();   // E267 on Watcom

int main() { return 0; }
