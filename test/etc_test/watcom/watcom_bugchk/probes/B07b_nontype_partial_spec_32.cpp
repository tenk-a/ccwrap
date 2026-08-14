// bugchk-id:     B07b
// bugchk-kind:   compile
// bugchk-expect: ok
// bugchk-desc:   the same base-case specialisation DOES work for int/long -- B07 is specific to 64-bit arguments
// Kept as a control for B07: it pins down that the defect is the *width* of the non-type
// argument, not partial specialisation on non-type arguments as such.
template <int A, int B>  struct gcd_i { static const int value = gcd_i<B, A % B>::value; };
template <int A>         struct gcd_i<A, 0> { static const int value = A; };

template <long A, long B> struct gcd_l { static const long value = gcd_l<B, A % B>::value; };
template <long A>         struct gcd_l<A, 0> { static const long value = A; };

template <unsigned long A, unsigned long B> struct gcd_ul { static const unsigned long value = gcd_ul<B, A % B>::value; };
template <unsigned long A>                  struct gcd_ul<A, 0> { static const unsigned long value = A; };

extern int check_i [ gcd_i <12, 8>::value == 4 ? 1 : -1 ];
extern int check_l [ gcd_l <12, 8>::value == 4 ? 1 : -1 ];
extern int check_ul[ gcd_ul<12, 8>::value == 4 ? 1 : -1 ];

int main() { return 0; }
