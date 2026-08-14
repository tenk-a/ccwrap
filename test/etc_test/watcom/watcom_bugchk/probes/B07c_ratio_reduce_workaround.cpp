// bugchk-id:     B07c
// bugchk-kind:   run
// bugchk-expect: ok
// bugchk-desc:   proves std::ratio CAN reduce on Watcom: dispatch on bool, carry 64-bit values in types, forward as V+0
//
// B07 says a base-case partial specialisation on a 64-bit non-type argument is never picked,
// and the catalogue concluded from that that <ratio> cannot reduce. It can. Two separate
// defects were being hit at once, and each has its own dodge:
//
//   1. matching       -- do not partially specialise on a 64-bit non-type argument at all.
//                        Discriminate on a bool and carry the values in wrapper *types*.
//   2. forwarding     -- never pass a 64-bit non-type parameter on verbatim (A20b): write V + 0.
//
// Neither alone is enough: with only (1) the values come out as garbage, with only (2) the
// base case is still not selected (E129 division by zero).
#include <stdio.h>

typedef long long im;

template <im V> struct llv { static const im value = V; };      // never partially specialised

template <bool BIsZero, class A, class B> struct gcd_h;
template <class A, class B> struct gcd_h<true,  A, B> { static const im value = A::value; };
template <class A, class B> struct gcd_h<false, A, B> {
    static const im value = gcd_h<(A::value % B::value) == 0, B, llv<A::value % B::value> >::value;
};

template <im A, im B> struct gcd_ {
    static const im value = gcd_h<(B + 0) == 0, llv<A + 0>, llv<B + 0> >::value;
};

template <im N, im D> struct ratio {
    static const im __ad = D < 0 ? -D : D;
    static const im __g  = gcd_<(N < 0 ? -N : N) + 0, __ad + 0>::value;
    static const im num  = (D < 0 ? -N : N) / __g;
    static const im den  = __ad / __g;
    typedef ratio<((D < 0 ? -N : N) / __g) + 0, (__ad / __g) + 0> type;
};

int main() {
    int bad = 0;
#define CHK(label, got, want) do { long long g_ = (long long)(got); \
    if (g_ != (long long)(want)) { ++bad; printf("B07c %-24s = %lld want %lld -> BROKEN\n", label, g_, (long long)(want)); } \
} while (0)

    typedef ratio<2, 4>  R24;
    typedef ratio<6, 9>  R69;
    typedef ratio<-2, 4> Rm24;
    typedef ratio<2, -4> R2m4;
    typedef gcd_<(im)1 << 40, (im)1 << 36> Gbig;

    CHK("gcd_<12,8>",          (gcd_<12, 8>::value), 4);
    CHK("gcd_<6,9>",           (gcd_<6, 9>::value),  3);
    CHK("gcd_<5,0>",           (gcd_<5, 0>::value),  5);   /* base case actually selected */
    CHK("gcd_ 64-bit operands", Gbig::value, (im)1 << 36);
    CHK("ratio<2,4>::num",     R24::num, 1);
    CHK("ratio<2,4>::den",     R24::den, 2);
    CHK("ratio<6,9>::num",     R69::num, 2);
    CHK("ratio<6,9>::den",     R69::den, 3);
    CHK("ratio<-2,4>::num",    Rm24::num, -1);            /* sign normalised onto num */
    CHK("ratio<-2,4>::den",    Rm24::den, 2);
    CHK("ratio<2,-4>::num",    R2m4::num, -1);
    CHK("ratio<2,-4>::den",    R2m4::den, 2);
    CHK("ratio<2,4>::type::num", R24::type::num, 1);       /* the A20 read path */
    CHK("ratio<2,4>::type::den", R24::type::den, 2);

    printf("B07c ratio reduction on Watcom -> %s\n", bad == 0 ? "OK" : "BROKEN");
    return bad == 0 ? 0 : 1;
}
