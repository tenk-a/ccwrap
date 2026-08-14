// bugchk-id:     A11
// bugchk-kind:   compile
// bugchk-expect: ok
// bugchk-desc:   type-specific free swap vs generic std::swap -- GUARD: catalogued E227 does not reproduce
namespace std {
template <class T>
void swap(T& a, T& b) { T t = a; a = b; b = t; }
}

namespace N {
struct X { int v; };
inline void swap(X& a, X& b) { int t = a.v; a.v = b.v; b.v = t; }   // should win by overload resolution
}

int main() {
    using std::swap;
    N::X a, b;
    a.v = 1; b.v = 2;
    swap(a, b);                      // unqualified: ADL finds N::swap, std::swap<T> also a candidate
    return a.v == 2 ? 0 : 1;
}
