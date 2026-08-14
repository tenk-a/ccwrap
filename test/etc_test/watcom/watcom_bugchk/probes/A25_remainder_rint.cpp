// bugchk-id:     A25
// bugchk-kind:   run
// bugchk-expect: bug
// bugchk-desc:   runtime remainder/remquo are wrong; rint/nearbyint do not round half to even
#include <stdio.h>
#include <math.h>

/* 'near' is a reserved word on Watcom (A15), hence near_eq. */
static int near_eq(double a, double b) { double d = a - b; if (d < 0) d = -d; return d < 1e-9; }

int main() {
    /* remainder(x,y) = x - n*y with n = round-half-to-even(x/y). */
    static const double t[][3] = {
        { 7,   4, -1   }, { 11,  4, -1 }, { 3,   4, -1   }, { 6,   4, -2 },
        { 7.5, 4, -0.5 }, { -7,  4,  1 }, { 5,   4,  1   }, { 9,   4,  1 },
        { 13,  4,  1   }, { 1,   4,  1 }, { 2,   4,  2   }
    };
    int bad = 0, i;
    for (i = 0; i < 11; ++i) {
        double got = remainder(t[i][0], t[i][1]);
        if (!near_eq(got, t[i][2])) {
            ++bad;
            printf("A25 remainder(%g,%g) = %g want %g -> BROKEN\n", t[i][0], t[i][1], got, t[i][2]);
        }
    }

    int q = -12345;
    double r = remquo(7, 4, &q);
    int remquo_ok = near_eq(r, -1) && q == 2;
    if (!remquo_ok) printf("A25 remquo(7,4) = %g q=%d want -1 q=2 -> BROKEN\n", r, q);

    /* Default rounding is FE_TONEAREST: rint/nearbyint must round half to EVEN.
       (round() always goes away from zero -- do not confuse the two.) */
    int rint_ok = near_eq(rint(2.5), 2.0) && near_eq(nearbyint(2.5), 2.0)
               && near_eq(rint(3.5), 4.0) && near_eq(rint(-2.5), -2.0);
    if (!rint_ok)
        printf("A25 rint(2.5)=%g nearbyint(2.5)=%g rint(-2.5)=%g want 2/2/-2 -> BROKEN\n",
               rint(2.5), nearbyint(2.5), rint(-2.5));

    int ok = (bad == 0) && remquo_ok && rint_ok;
    printf("A25 remainder/remquo/rint -> %s\n", ok ? "OK" : "BROKEN");
    return ok ? 0 : 1;
}
