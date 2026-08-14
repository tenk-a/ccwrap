// bugchk-id:     A04
// bugchk-kind:   compile
// bugchk-expect: ok
// bugchk-desc:   global init from a template's static member -- GUARD: catalogued E121 does not reproduce
template <class charT>
struct basic_ostream { int x; };

template <class charT>
struct StdioSync { static basic_ostream<charT> member; };

template <class charT>
basic_ostream<charT> StdioSync<charT>::member;

// This is the shape libcxx03's <iostream> wanted for cout/cerr/clog/cin.
static basic_ostream<char>& cout = StdioSync<char>::member;

int main() { return cout.x; }
