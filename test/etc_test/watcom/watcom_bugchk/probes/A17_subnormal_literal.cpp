// bugchk-id:     A17
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   a subnormal literal is rejected (E173) -- FLT_TRUE_MIN / DBL_TRUE_MIN, denorm_min()
static const double dbl_true_min = 4.9406564584124654e-324;    // smallest positive subnormal double
static const float  flt_true_min = 1.40129846e-45f;            // smallest positive subnormal float

int main() { return (dbl_true_min > 0.0 && flt_true_min > 0.0f) ? 0 : 1; }
