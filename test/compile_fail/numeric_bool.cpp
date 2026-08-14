#include <numeric>

int main() {
    bool t = true, f = false;
    (void)t; (void)f;

    int r = std::gcd(12, 18) + std::lcm(4, 6) + std::midpoint(1, 4);
    (void)r;

#ifdef REJ_GCD
    std::gcd(t, f);
#endif
#ifdef REJ_LCM
    std::lcm(t, f);
#endif
#ifdef REJ_MIDPOINT
    std::midpoint(t, f);
#endif
    return 0;
}
