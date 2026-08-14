// bugchk-id:     A14
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   int(T<a,b>::value) functional-cast is misparsed (E253)
template <class T, int V> struct integral_constant { static const int value = V; };

int main() {
    return int(integral_constant<int, 7>::value) - 7;
}
