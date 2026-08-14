// bugchk-id:     A12
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   partial specialisation on a bounded array T[N] never matches (is_array/rank/extent, begin(T(&)[N]))
template <class T>                     struct IsArr        { enum { value = 0 }; };
template <class T>                     struct IsArr<T[]>   { enum { value = 1 }; };
template <class T, unsigned long N>    struct IsArr<T[N]>  { enum { value = 2 }; };

// A negative array bound is a hard error, so this is a compile-time assertion.
extern int check_bounded  [ IsArr<int[3]>::value == 2 ? 1 : -1 ];
extern int check_unbounded[ IsArr<int[] >::value == 1 ? 1 : -1 ];

int main() { return 0; }
