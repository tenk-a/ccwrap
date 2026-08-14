/* bugchk-id:     C02c
 * bugchk-kind:   compile
 * bugchk-expect: bug
 * bugchk-desc:   native <complex.h> is a facade: 437 lines of declarations, but no _Complex type and no runtime
 *
 * Even with -za99, so that the header's C99 gate opens, the compiler has no _Complex type
 * (E1060: Invalid type) and clib3r.lib has no cabs/creal/... bodies. So there is nothing to
 * forward to, and a library-level substitute is the only option. Probed WITH -za99 to make
 * the point that the C99 dialect is not what is missing.
 */
/* bugchk-flags:  -za99 */
#include <complex.h>

int main(void) {
    double _Complex z = 1.0;
    double r = creal(z);
    return r == 1.0 ? 0 : 1;
}
