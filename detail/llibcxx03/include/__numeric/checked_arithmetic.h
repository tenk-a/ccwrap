//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception). ccwrap: libc++ layout,
// classic C++03 body for Open Watcom.
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___NUMERIC_CHECKED_ARITHMETIC_H
#define _CCW_LIBCPP___NUMERIC_CHECKED_ARITHMETIC_H
#include "../__config"
#include <limits>
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

struct __ckd_val {
    bool __neg;
    unsigned long long __hi;
    unsigned long long __lo;
};

template <class _Up>
_CCW_LIBCPP_HIDE_FROM_ABI __ckd_val __ckd_load(_Up __a) {
    __ckd_val __v;
    __v.__hi = 0;
    __v.__neg = false;
    if (numeric_limits<_Up>::is_signed) {
        long long __s = (long long)__a;
        if (__s < 0) {
            __v.__neg = true;
            __v.__lo = 0ULL - (unsigned long long)__s;
        } else {
            __v.__lo = (unsigned long long)__s;
        }
    } else {
        __v.__lo = (unsigned long long)__a;
    }
    return __v;
}

_CCW_LIBCPP_HIDE_FROM_ABI inline int __ckd_cmp_mag(const __ckd_val& __x, const __ckd_val& __y) {
    if (__x.__hi != __y.__hi) return __x.__hi < __y.__hi ? -1 : 1;
    if (__x.__lo != __y.__lo) return __x.__lo < __y.__lo ? -1 : 1;
    return 0;
}

_CCW_LIBCPP_HIDE_FROM_ABI inline void __ckd_add_mag(const __ckd_val& __x, const __ckd_val& __y, __ckd_val& __r) {
    __r.__lo = __x.__lo + __y.__lo;
    __r.__hi = __x.__hi + __y.__hi + (__r.__lo < __x.__lo ? 1ULL : 0ULL);
}

_CCW_LIBCPP_HIDE_FROM_ABI inline void __ckd_sub_mag(const __ckd_val& __x, const __ckd_val& __y, __ckd_val& __r) {
    __r.__hi = __x.__hi - __y.__hi - (__x.__lo < __y.__lo ? 1ULL : 0ULL);
    __r.__lo = __x.__lo - __y.__lo;
}

_CCW_LIBCPP_HIDE_FROM_ABI inline void __ckd_mul_mag(unsigned long long __a, unsigned long long __b, __ckd_val& __r) {
    const unsigned long long __m = 0xFFFFFFFFULL;
    unsigned long long __a0 = __a & __m, __a1 = __a >> 32;
    unsigned long long __b0 = __b & __m, __b1 = __b >> 32;
    unsigned long long __p00 = __a0 * __b0;
    unsigned long long __p01 = __a0 * __b1;
    unsigned long long __p10 = __a1 * __b0;
    unsigned long long __p11 = __a1 * __b1;
    unsigned long long __mid = (__p00 >> 32) + (__p01 & __m) + (__p10 & __m);
    __r.__lo = (__p00 & __m) | (__mid << 32);
    __r.__hi = __p11 + (__p01 >> 32) + (__p10 >> 32) + (__mid >> 32);
}

_CCW_LIBCPP_HIDE_FROM_ABI inline void __ckd_norm(__ckd_val& __v) {
    if (__v.__hi == 0 && __v.__lo == 0) __v.__neg = false;   // no negative zero
}

_CCW_LIBCPP_HIDE_FROM_ABI inline __ckd_val __ckd_math_add(const __ckd_val& __x, const __ckd_val& __y) {
    __ckd_val __r;
    if (__x.__neg == __y.__neg) {
        __ckd_add_mag(__x, __y, __r);
        __r.__neg = __x.__neg;
    } else if (__ckd_cmp_mag(__x, __y) >= 0) {
        __ckd_sub_mag(__x, __y, __r);
        __r.__neg = __x.__neg;
    } else {
        __ckd_sub_mag(__y, __x, __r);
        __r.__neg = __y.__neg;
    }
    __ckd_norm(__r);
    return __r;
}

_CCW_LIBCPP_HIDE_FROM_ABI inline __ckd_val __ckd_math_mul(const __ckd_val& __x, const __ckd_val& __y) {
    __ckd_val __r;
    __ckd_mul_mag(__x.__lo, __y.__lo, __r);
    __r.__neg = (__x.__neg != __y.__neg);
    __ckd_norm(__r);
    return __r;
}

template <class _Tp>
_CCW_LIBCPP_HIDE_FROM_ABI bool __ckd_store(_Tp* __r, const __ckd_val& __v) {
    unsigned long long __bits = __v.__neg ? (0ULL - __v.__lo) : __v.__lo;
    *__r = (_Tp)__bits;

    if (__v.__hi != 0) return true;                       // needs > 64 bits
    if (__v.__neg) {
        if (!numeric_limits<_Tp>::is_signed) return true; // negative into unsigned
        long long __mn = (long long)numeric_limits<_Tp>::min();
        return __v.__lo > (0ULL - (unsigned long long)__mn);
    }
    return __v.__lo > (unsigned long long)numeric_limits<_Tp>::max();
}

template <class _Tp, class _Up, class _Vp>
_CCW_LIBCPP_HIDE_FROM_ABI bool ckd_add(_Tp* __result, _Up __a, _Vp __b) {
    return __ckd_store(__result, __ckd_math_add(__ckd_load(__a), __ckd_load(__b)));
}

template <class _Tp, class _Up, class _Vp>
_CCW_LIBCPP_HIDE_FROM_ABI bool ckd_sub(_Tp* __result, _Up __a, _Vp __b) {
    __ckd_val __y = __ckd_load(__b);
    __y.__neg = !__y.__neg;
    __ckd_norm(__y);
    return __ckd_store(__result, __ckd_math_add(__ckd_load(__a), __y));
}

template <class _Tp, class _Up, class _Vp>
_CCW_LIBCPP_HIDE_FROM_ABI bool ckd_mul(_Tp* __result, _Up __a, _Vp __b) {
    return __ckd_store(__result, __ckd_math_mul(__ckd_load(__a), __ckd_load(__b)));
}

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___NUMERIC_CHECKED_ARITHMETIC_H
