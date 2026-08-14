// bugchk-id:     B07
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   base-case partial specialisation on a 64-bit non-type argument is not selected (<ratio> cannot reduce)
// This is the exact reason std::ratio does not reduce: intmax_t is long long, gcd_<A,0> is
// never picked, so the recursion runs into A % 0 (E129 division by zero).
// NOTE: 32-bit and narrower non-type parameters DO work -- see B07b.
typedef long long intmax_t_;

template <intmax_t_ A, intmax_t_ B> struct gcd_ { static const intmax_t_ value = gcd_<B, A % B>::value; };
template <intmax_t_ A>              struct gcd_<A, 0> { static const intmax_t_ value = A; };

extern int check[ gcd_<12, 8>::value == 4 ? 1 : -1 ];

int main() { return 0; }
