// bugchk-id:     B09
// bugchk-kind:   compile
// bugchk-expect: ok
// bugchk-desc:   C++03 pair holding a reference member -- GUARD: catalogued failure does not reproduce
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(T1 a, T2 b) : first(a), second(b) {}
};

int main() {
    int x = 7;
    pair<int&, int> p(x, 1);      // T1 = int&, so first is a reference bound to x
    p.first = 9;
    return x - 9;                  // writing through p.first must change x
}
