// bugchk-id:     A13
// bugchk-kind:   compile
// bugchk-expect: ok
// bugchk-desc:   nested dependent typedef used as an integer -- GUARD: catalogued E157 does not reproduce
template <class T> struct remove_cv             { typedef T type; };
template <class T> struct remove_cv<const T>    { typedef T type; };
template <class T> struct remove_cv<volatile T> { typedef T type; };

template <class T> struct make_unsigned_helper;
template <> struct make_unsigned_helper<int>          { typedef unsigned int type; };
template <> struct make_unsigned_helper<unsigned int> { typedef unsigned int type; };

template <class T> struct make_unsigned {
    typedef typename make_unsigned_helper<typename remove_cv<T>::type>::type type;
};

template <class T>
int digit_sum(T value, int base) {
    typedef typename make_unsigned<T>::type U;       // U must be usable as an integer
    U mag = (U)value;
    int n = 0;
    while (mag) { n += (int)(mag % U(base)); mag /= U(base); }
    return n;
}

int main() { return digit_sum<int>(123, 10) == 6 ? 0 : 1; }
