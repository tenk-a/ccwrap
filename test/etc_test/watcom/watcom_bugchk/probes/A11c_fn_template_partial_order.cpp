// bugchk-id:     A11c
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   function template partial ordering is broken: the more specialised overload is still ambiguous (E227)
// The shape actually hit: <stdckdint.h>'s ckd_add(T*,T,T) vs <numeric>'s ckd_add(T*,U,V).
template <class T, class U, class V> int ckd_add(T*, U, V) { return 1; }   // generic
template <class T>                   int ckd_add(T*, T, T) { return 2; }   // more specialised

int main() {
    int r = 0;
    return ckd_add(&r, 1, 1) == 2 ? 0 : 1;     // partial ordering must pick the T,T,T one
}
