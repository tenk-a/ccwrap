// bugchk-id:     A06
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   MAX*MAX in a constant expression is diagnosed as overflow (numeric_limits infinity)
#include <float.h>

// The classic way to spell "infinity" without a literal. Watcom constant-folds it and
// rejects the overflow instead of producing inf.
static const double inf_like = DBL_MAX * DBL_MAX;

int main() { return inf_like > DBL_MAX ? 0 : 1; }
