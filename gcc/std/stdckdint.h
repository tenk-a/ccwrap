// ccwrap <stdckdint.h>
#pragma once

#include <ccwrap_common.h>

#ifndef __STDC_VERSION_STDCKDINT_H__
# define __STDC_VERSION_STDCKDINT_H__ 202311L
#endif

#ifdef __cplusplus

# if !defined(_CCW_STD_CKD_DEFINED)
#  define _CCW_STD_CKD_DEFINED   /* shared with detail/cxx/stdckdint.hpp */
namespace std {
template <class _Rp, class _Tp, class _Up>
inline bool ckd_add(_Rp* __r, _Tp __a, _Up __b) { return __builtin_add_overflow(__a, __b, __r); }
template <class _Rp, class _Tp, class _Up>
inline bool ckd_sub(_Rp* __r, _Tp __a, _Up __b) { return __builtin_sub_overflow(__a, __b, __r); }
template <class _Rp, class _Tp, class _Up>
inline bool ckd_mul(_Rp* __r, _Tp __a, _Up __b) { return __builtin_mul_overflow(__a, __b, __r); }
}   // namespace std
# endif

#else

# ifndef ckd_add
#  define ckd_add(result, a, b) __builtin_add_overflow((a), (b), (result))
#  define ckd_sub(result, a, b) __builtin_sub_overflow((a), (b), (result))
#  define ckd_mul(result, a, b) __builtin_mul_overflow((a), (b), (result))
# endif

#endif // __cplusplus
