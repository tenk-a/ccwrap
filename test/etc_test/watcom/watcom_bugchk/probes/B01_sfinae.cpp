// bugchk-id:     B01
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   no SFINAE: enable_if cannot select between overloads (tag dispatch is the workaround)
template <bool B, class T = void> struct enable_if {};
template <class T> struct enable_if<true, T> { typedef T type; };

template <class T> struct is_int { enum { value = 0 }; };
template <> struct is_int<int>  { enum { value = 1 }; };

template <class T> typename enable_if< is_int<T>::value, int>::type f(T) { return 1; }
template <class T> typename enable_if<!is_int<T>::value, int>::type f(T) { return 2; }

int main() { return (f(1) == 1 && f(1.0) == 2) ? 0 : 1; }
