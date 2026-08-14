// bugchk-id:     A51
// bugchk-kind:   run
// bugchk-expect: bug
// bugchk-aux:    extra/A51_mangle64_other_tu.cpp
// bugchk-desc:   a 64-bit non-type template argument mangles differently in every TU, so the function cannot be linked
//
// A class template with a 64-bit (long long) non-type parameter gets a mangled name
// that is not a function of the ARGUMENT VALUES. Watcom encodes something TU-local
// there -- the two TUs of this probe produce
//
//     ...rat64$::01AEDOCOY01AEDODOY...      (one TU)
//     ...rat64$::09DBT18Z09DBT08Z...        (the other)
//
// for the very same rat64<1,1>. So an out-of-line function whose signature names such
// a type is unreachable: the caller emits one name, the definition another, and the
// linker reports E2028 for a function that is plainly there.
//
// The 32-bit version is the control. It links, which is why this shows up so rarely.
//
// Found through <filesystem>: file_time_type is chrono::time_point<file_clock,
// duration<long long, ratio<1,1> > >, so filesystem::last_write_time could never be
// linked out of libcxx03.lib. The fix is to keep such a type out of any signature that
// crosses a TU boundary -- last_write_time is inline now and hands time_t to the
// out-of-line worker.
#include <stdio.h>

template <long long _Np, long long _Dp> struct rat64 { };
template <int _Np, int _Dp>             struct rat32 { };

int takes_rat64(rat64<1, 1>);       // defined in extra/A51_mangle64_other_tu.cpp
int takes_rat32(rat32<1, 1>);

int main(void) {
    int bad = 0;
    rat64<1, 1> a;
    rat32<1, 1> b;
    if (takes_rat64(a) != 42) { printf("BROKEN: 64-bit non-type argument\n"); bad = 1; }
    if (takes_rat32(b) != 42) { printf("BROKEN: 32-bit non-type argument\n"); bad = 1; }
    if (!bad) printf("ok\n");
    return bad;
}
