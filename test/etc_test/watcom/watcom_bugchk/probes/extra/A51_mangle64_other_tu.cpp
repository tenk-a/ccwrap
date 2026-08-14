// The SECOND translation unit of probe A51 (not a probe of its own -- the runner only
// globs probes/*.c and probes/*.cpp, and this lives one level down in extra/).
//
// It defines the two functions the probe declares. The declarations are character for
// character the same, so a conforming compiler produces the same mangled name here and
// there and the program links.

template <long long _Np, long long _Dp> struct rat64 { };
template <int _Np, int _Dp>             struct rat32 { };

int takes_rat64(rat64<1, 1>) { return 42; }
int takes_rat32(rat32<1, 1>) { return 42; }
