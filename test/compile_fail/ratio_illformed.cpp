#include <ratio>

int main() {
    typedef std::ratio<2, 4>  half;
    typedef std::ratio<-2, 4> minus_half;
    typedef std::ratio<1, 3>  third;
    typedef std::ratio_divide<half, third> ok_div;
    int r = (int)half::num + (int)half::den + (int)minus_half::num + (int)ok_div::num;
    (void)r;

#ifdef REJ_DEN_ZERO
    typedef std::ratio<1, 0> bad;
    (void)sizeof(bad);
#endif
#ifdef REJ_ZERO_OVER_ZERO
    typedef std::ratio<0, 0> bad0;
    (void)sizeof(bad0);
#endif
#ifdef REJ_NUM_MIN
    typedef std::ratio<(-9223372036854775807LL - 1), 1> bad_num;
    (void)sizeof(bad_num);
#endif
#ifdef REJ_DEN_MIN
    typedef std::ratio<1, (-9223372036854775807LL - 1)> bad_den;
    (void)sizeof(bad_den);
#endif
#ifdef REJ_DIVIDE_BY_ZERO
    typedef std::ratio_divide<half, std::ratio<0, 1> > bad_div;
    (void)sizeof(bad_div);
#endif
    return 0;
}
