// bugchk-id:     B03
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   no rvalue references / move semantics (unique_ptr uses a move-emulation proxy)
struct X { int v; };

static int take(X&& x) { return x.v; }

int main() {
    X a; a.v = 1;
    return take(static_cast<X&&>(a)) - 1;
}
