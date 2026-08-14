// bugchk-id:     A10
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   native <cmath> gives the C99 classifiers as macros only, so std::isnan(x) will not parse
#include <cmath>

// Deliberately NOT including ccwrap: this probes the *native* header. The standard says
// <cmath> must provide these as std:: functions and undefine the macros.
int main() {
    double x = 1.0;
    int a = std::isnan(x);
    int b = std::isinf(x);
    int c = std::isfinite(x);
    int d = std::signbit(x);
    int e = std::fpclassify(x);
    return (a || b || !c || d || e == 0) ? 1 : 0;
}
