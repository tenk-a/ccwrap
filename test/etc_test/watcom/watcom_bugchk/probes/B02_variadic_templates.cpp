// bugchk-id:     B02
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   no variadic templates (workaround: detail/fnctmpl.hpp / __fnctmpl.h 0..N fixed arities)
template <class... Ts> struct tuple {};
template <class... Ts> int count(Ts...) { return sizeof...(Ts); }

int main() { return count(1, 2, 3) - 3; }
