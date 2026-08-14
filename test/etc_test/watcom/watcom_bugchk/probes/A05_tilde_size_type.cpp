// bugchk-id:     A05
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   ~size_type(0) is misparsed as a destructor call (string::npos)
typedef unsigned long size_type;

struct S {
    static const size_type npos = ~size_type(0);
};

int main() { return S::npos == (size_type)-1 ? 0 : 1; }
