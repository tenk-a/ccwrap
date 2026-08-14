// bugchk-id:     B08
// bugchk-kind:   compile
// bugchk-expect: ok
// bugchk-desc:   template<class T, T v> integral_constant -- GUARD: catalogued failure does not reproduce
template <class T, T v>
struct integral_constant {
    static const T value = v;
    typedef T value_type;
    typedef integral_constant<T, v> type;
    operator value_type() const { return value; }
};

typedef integral_constant<bool, true>  true_type;
typedef integral_constant<bool, false> false_type;

template <class T> struct is_int : false_type {};
template <> struct is_int<int> : true_type {};

template <class T> struct size_of : integral_constant<int, sizeof(T)> {};

int main() {
    return (is_int<int>::value && !is_int<char>::value && size_of<int>::value == 4) ? 0 : 1;
}
