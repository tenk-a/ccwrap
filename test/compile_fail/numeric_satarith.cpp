#include <numeric>

int main() {
    bool t = true, f = false;
    (void)t; (void)f;

    int r = std::add_sat(1, 2) + std::sub_sat(3, 1) + std::mul_sat(2, 3)
          + std::div_sat(6, 2) + (int)std::saturate_cast<short>(5);
    (void)r;

#ifdef REJ_ADD_SAT
    std::add_sat(t, f);
#endif
#ifdef REJ_SUB_SAT
    std::sub_sat(t, f);
#endif
#ifdef REJ_MUL_SAT
    std::mul_sat(t, f);
#endif
#ifdef REJ_DIV_SAT
    std::div_sat(t, f);
#endif
#ifdef REJ_SAT_CAST
    std::saturate_cast<bool>(5);
#endif
    return 0;
}
