// bugchk-id:     A15
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   near / far / huge are still reserved words (DOS memory-model keywords)
static bool near(double a, double b) { return a == b; }
static int  far_count = 0;
static int  huge_count = 0;

int main() {
    (void)far_count; (void)huge_count;
    return near(1.0, 1.0) ? 0 : 1;
}
