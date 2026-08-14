/**
 *  @file   detail/stdckdint_c.h
 *  @brief  The C form of <stdckdint.h> (C23): ckd_add / ckd_sub / ckd_mul.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_STDCKDINT_C_H
#define _CCW_DETAIL_STDCKDINT_C_H

#include <ccwrap_common.h>
#include <stdbool.h>

#ifndef __STDC_VERSION_STDCKDINT_H__
#define __STDC_VERSION_STDCKDINT_H__ 202311L
#endif

#ifndef _CCW_CKD_C_DEFINED
#define _CCW_CKD_C_DEFINED

_CCW_F_INL(FUNC, _ccw_int64,  __ccw_ckd_min_for_size,(unsigned int size),(size)) {
    switch (size) {
    case 1: return -128;
    case 2: return -32768;
    case 4: return (-2147483647 - 1);
    default: return (-9223372036854775807LL - 1LL);
    }
}

_CCW_F_INL(FUNC, _ccw_int64,  __ccw_ckd_max_for_size,(unsigned int size),(size)) {
    switch (size) {
    case 1: return 127;
    case 2: return 32767;
    case 4: return 2147483647;
    default: return 9223372036854775807LL;
    }
}

_CCW_F_DEF(PROC, void, __ccw_ckd_store,(void *result, unsigned int size, _ccw_ullong value),(result,size,value)) {
    unsigned int i;
    unsigned char *p = (unsigned char *)result;

    for (i = 0; i < size; ++i) {
        p[i] = (unsigned char)(value & 0xff);
        value >>= 8;
    }
}

#define _CCW_CKD_WRAP2(op, a, b) \
    ( __ccw_ckd_va = (a), __ccw_ckd_vb = (b), \
      (_ccw_ullong)(_ccw_int64)__ccw_ckd_va op (_ccw_ullong)(_ccw_int64)__ccw_ckd_vb )

_CCW_F_DEF(FUNC, bool,  __ccw_ckd_add,(void *result, unsigned int size, _ccw_int64 a, _ccw_int64 b), (result,size, a, b)) {
    _ccw_int64 minv = __ccw_ckd_min_for_size(size);
    _ccw_int64 maxv = __ccw_ckd_max_for_size(size);
    bool overflow = (b > 0 && a > maxv - b) || (b < 0 && a < minv - b);
    volatile _ccw_int64 __ccw_ckd_va, __ccw_ckd_vb;
    _ccw_ullong wrapped = _CCW_CKD_WRAP2(+, a, b);      /* volatile: Watcom folds a 64-bit wrap wrongly */

    __ccw_ckd_store(result, size, wrapped);
    return overflow;
}

_CCW_F_DEF(FUNC, bool, __ccw_ckd_sub,(void *result, unsigned int size, _ccw_int64 a, _ccw_int64 b),(result,size,a,b)) {
    _ccw_int64 minv = __ccw_ckd_min_for_size(size);
    _ccw_int64 maxv = __ccw_ckd_max_for_size(size);
    bool overflow = (b < 0 && a > maxv + b) || (b > 0 && a < minv + b);
    volatile _ccw_int64 __ccw_ckd_va, __ccw_ckd_vb;
    _ccw_ullong wrapped = _CCW_CKD_WRAP2(-, a, b);      /* volatile: Watcom folds a 64-bit wrap wrongly */

    __ccw_ckd_store(result, size, wrapped);
    return overflow;
}

_CCW_F_DEF(FUNC, bool, __ccw_ckd_mul,(void *result, unsigned int size, _ccw_int64 a, _ccw_int64 b), (result,size,a,b)) {
    _ccw_int64 minv = __ccw_ckd_min_for_size(size);
    _ccw_int64 maxv = __ccw_ckd_max_for_size(size);
    bool neg = (a < 0) != (b < 0);
    _ccw_ullong ua = a < 0 ? (_ccw_ullong)(0 - (_ccw_ullong)a) : (_ccw_ullong)a;
    _ccw_ullong ub = b < 0 ? (_ccw_ullong)(0 - (_ccw_ullong)b) : (_ccw_ullong)b;
    _ccw_ullong limit = neg ? (_ccw_ullong)(0 - (_ccw_ullong)minv) : (_ccw_ullong)maxv;
    bool overflow = ub != 0 && ua > limit / ub;
    volatile _ccw_int64 __ccw_ckd_va, __ccw_ckd_vb;
    _ccw_ullong wrapped = _CCW_CKD_WRAP2(*, a, b);      /* volatile: Watcom folds a 64-bit wrap wrongly */

    __ccw_ckd_store(result, size, wrapped);
    return overflow;
}

#endif  /* _CCW_CKD_C_DEFINED */

#ifndef __cplusplus
#ifndef ckd_add
#define ckd_add(result, a, b) __ccw_ckd_add((result), (unsigned int)sizeof(*(result)), (_ccw_int64)(a), (_ccw_int64)(b))
#define ckd_sub(result, a, b) __ccw_ckd_sub((result), (unsigned int)sizeof(*(result)), (_ccw_int64)(a), (_ccw_int64)(b))
#define ckd_mul(result, a, b) __ccw_ckd_mul((result), (unsigned int)sizeof(*(result)), (_ccw_int64)(a), (_ccw_int64)(b))
#endif
#endif

#endif  /* _CCW_DETAIL_STDCKDINT_C_H */
