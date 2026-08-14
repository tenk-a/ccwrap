/*
 * ccwrap: libc++ layout. classic C++03 body.
 */
#ifndef _CCW_LIBCPP___ALGORITHM_IS_PERMUTATION_H
#define _CCW_LIBCPP___ALGORITHM_IS_PERMUTATION_H
#include "../__config"

_CCW_LIBCPP_BEGIN_NAMESPACE_STD
template <class _It1, class _It2>
_CCW_LIBCPP_HIDE_FROM_ABI bool is_permutation(_It1 __f1, _It1 __l1, _It2 __f2) {
    _It2 __l2 = __f2; for (_It1 __t = __f1; __t != __l1; ++__t) ++__l2;
    for (_It1 __i = __f1; __i != __l1; ++__i) {
        bool __seen = false; for (_It1 __k = __f1; __k != __i; ++__k) if (*__k == *__i) { __seen = true; break; }
        if (__seen) continue;
        int __c1 = 0; for (_It1 __k = __i; __k != __l1; ++__k) if (*__k == *__i) ++__c1;
        int __c2 = 0; for (_It2 __k = __f2; __k != __l2; ++__k) if (*__k == *__i) ++__c2;
        if (__c1 != __c2) return false;
    }
    return true;
}

template <class _It1, class _It2, class _Pred>
_CCW_LIBCPP_HIDE_FROM_ABI bool is_permutation(_It1 __f1, _It1 __l1, _It2 __f2, _Pred __eq) {
    _It2 __l2 = __f2; for (_It1 __t = __f1; __t != __l1; ++__t) ++__l2;
    for (_It1 __i = __f1; __i != __l1; ++__i) {
        bool __seen = false; for (_It1 __k = __f1; __k != __i; ++__k) if (__eq(*__k, *__i)) { __seen = true; break; }
        if (__seen) continue;
        int __c1 = 0; for (_It1 __k = __i; __k != __l1; ++__k) if (__eq(*__k, *__i)) ++__c1;
        int __c2 = 0; for (_It2 __k = __f2; __k != __l2; ++__k) if (__eq(*__k, *__i)) ++__c2;
        if (__c1 != __c2) return false;
    }
    return true;
}
_CCW_LIBCPP_END_NAMESPACE_STD
#endif
