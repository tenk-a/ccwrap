/*
 * @file bind.h
 * @brief bind for c++03
 */
#ifndef _CCW_LIBCPP___FUNCTIONAL_BIND_H
#define _CCW_LIBCPP___FUNCTIONAL_BIND_H
#include "../__config"
#include "../__type_traits/integral_constant.h"
#include "../__functional/invoke.h"
_CCW_LIBCPP_BEGIN_NAMESPACE_STD

namespace placeholders {
template <int _Np> struct __ph {};
_ccw_inline_const __ph<1> _1 = __ph<1>();
_ccw_inline_const __ph<2> _2 = __ph<2>();
_ccw_inline_const __ph<3> _3 = __ph<3>();
_ccw_inline_const __ph<4> _4 = __ph<4>();
_ccw_inline_const __ph<5> _5 = __ph<5>();
_ccw_inline_const __ph<6> _6 = __ph<6>();
_ccw_inline_const __ph<7> _7 = __ph<7>();
_ccw_inline_const __ph<8> _8 = __ph<8>();
_ccw_inline_const __ph<9> _9 = __ph<9>();
} // namespace placeholders

template <class _Tp> struct is_placeholder : public integral_constant<int, 0> {};
template <int _Np> struct is_placeholder<placeholders::__ph<_Np> > : public integral_constant<int, _Np> {};

template <class _Tp> struct is_bind_expression : public false_type {};

template <class _Ap> struct __bind_mu {
    template <class _Ca1, class _Ca2, class _Ca3>
    _CCW_LIBCPP_HIDE_FROM_ABI static const _Ap& __go(const _Ap& __a, _Ca1&, _Ca2&, _Ca3&) { return __a; }
};
template <> struct __bind_mu<placeholders::__ph<1> > {
    template <class _Ca1, class _Ca2, class _Ca3>
    _CCW_LIBCPP_HIDE_FROM_ABI static _Ca1& __go(const placeholders::__ph<1>&, _Ca1& __c1, _Ca2&, _Ca3&) { return __c1; }
};
template <> struct __bind_mu<placeholders::__ph<2> > {
    template <class _Ca1, class _Ca2, class _Ca3>
    _CCW_LIBCPP_HIDE_FROM_ABI static _Ca2& __go(const placeholders::__ph<2>&, _Ca1&, _Ca2& __c2, _Ca3&) { return __c2; }
};
template <> struct __bind_mu<placeholders::__ph<3> > {
    template <class _Ca1, class _Ca2, class _Ca3>
    _CCW_LIBCPP_HIDE_FROM_ABI static _Ca3& __go(const placeholders::__ph<3>&, _Ca1&, _Ca2&, _Ca3& __c3) { return __c3; }
};

template <class _Fp>
class __bind0 {
    _Fp __f_;
public:
    typedef typename __bind_rc<_Fp>::type result_type;
    _CCW_LIBCPP_HIDE_FROM_ABI explicit __bind0(_Fp __f) : __f_(__f) {}
    _CCW_LIBCPP_HIDE_FROM_ABI result_type operator()() { return invoke(__f_); }
    template <class _Ca1> _CCW_LIBCPP_HIDE_FROM_ABI result_type operator()(_Ca1) { return invoke(__f_); }
};

template <class _Fp, class _A1>
class __bind1 {
    _Fp __f_; _A1 __a1_;
    int __d_;                                        // dummy slot for missing late args
public:
    typedef typename __bind_rc<_Fp>::type result_type;
    _CCW_LIBCPP_HIDE_FROM_ABI __bind1(_Fp __f, _A1 __a1) : __f_(__f), __a1_(__a1), __d_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI result_type operator()() {
        return invoke(__f_, __bind_mu<_A1>::__go(__a1_, __d_, __d_, __d_));
    }
    template <class _Ca1> _CCW_LIBCPP_HIDE_FROM_ABI result_type operator()(_Ca1 __c1) {
        return invoke(__f_, __bind_mu<_A1>::__go(__a1_, __c1, __d_, __d_));
    }
    template <class _Ca1, class _Ca2> _CCW_LIBCPP_HIDE_FROM_ABI result_type operator()(_Ca1 __c1, _Ca2 __c2) {
        return invoke(__f_, __bind_mu<_A1>::__go(__a1_, __c1, __c2, __d_));
    }
    template <class _Ca1, class _Ca2, class _Ca3> _CCW_LIBCPP_HIDE_FROM_ABI result_type operator()(_Ca1 __c1, _Ca2 __c2, _Ca3 __c3) {
        return invoke(__f_, __bind_mu<_A1>::__go(__a1_, __c1, __c2, __c3));
    }
};

template <class _Fp, class _A1, class _A2>
class __bind2 {
    _Fp __f_; _A1 __a1_; _A2 __a2_;
    int __d_;
public:
    typedef typename __bind_rc<_Fp>::type result_type;
    _CCW_LIBCPP_HIDE_FROM_ABI __bind2(_Fp __f, _A1 __a1, _A2 __a2) : __f_(__f), __a1_(__a1), __a2_(__a2), __d_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI result_type operator()() {
        return invoke(__f_, __bind_mu<_A1>::__go(__a1_, __d_, __d_, __d_),
                            __bind_mu<_A2>::__go(__a2_, __d_, __d_, __d_));
    }
    template <class _Ca1> _CCW_LIBCPP_HIDE_FROM_ABI result_type operator()(_Ca1 __c1) {
        return invoke(__f_, __bind_mu<_A1>::__go(__a1_, __c1, __d_, __d_),
                            __bind_mu<_A2>::__go(__a2_, __c1, __d_, __d_));
    }
    template <class _Ca1, class _Ca2> _CCW_LIBCPP_HIDE_FROM_ABI result_type operator()(_Ca1 __c1, _Ca2 __c2) {
        return invoke(__f_, __bind_mu<_A1>::__go(__a1_, __c1, __c2, __d_),
                            __bind_mu<_A2>::__go(__a2_, __c1, __c2, __d_));
    }
    template <class _Ca1, class _Ca2, class _Ca3> _CCW_LIBCPP_HIDE_FROM_ABI result_type operator()(_Ca1 __c1, _Ca2 __c2, _Ca3 __c3) {
        return invoke(__f_, __bind_mu<_A1>::__go(__a1_, __c1, __c2, __c3),
                            __bind_mu<_A2>::__go(__a2_, __c1, __c2, __c3));
    }
};

template <class _Fp, class _A1, class _A2, class _A3>
class __bind3 {
    _Fp __f_; _A1 __a1_; _A2 __a2_; _A3 __a3_;
    int __d_;
public:
    typedef typename __bind_rc<_Fp>::type result_type;
    _CCW_LIBCPP_HIDE_FROM_ABI __bind3(_Fp __f, _A1 __a1, _A2 __a2, _A3 __a3)
        : __f_(__f), __a1_(__a1), __a2_(__a2), __a3_(__a3), __d_(0) {}
    _CCW_LIBCPP_HIDE_FROM_ABI result_type operator()() {
        return invoke(__f_, __bind_mu<_A1>::__go(__a1_, __d_, __d_, __d_),
                            __bind_mu<_A2>::__go(__a2_, __d_, __d_, __d_),
                            __bind_mu<_A3>::__go(__a3_, __d_, __d_, __d_));
    }
    template <class _Ca1> _CCW_LIBCPP_HIDE_FROM_ABI result_type operator()(_Ca1 __c1) {
        return invoke(__f_, __bind_mu<_A1>::__go(__a1_, __c1, __d_, __d_),
                            __bind_mu<_A2>::__go(__a2_, __c1, __d_, __d_),
                            __bind_mu<_A3>::__go(__a3_, __c1, __d_, __d_));
    }
    template <class _Ca1, class _Ca2> _CCW_LIBCPP_HIDE_FROM_ABI result_type operator()(_Ca1 __c1, _Ca2 __c2) {
        return invoke(__f_, __bind_mu<_A1>::__go(__a1_, __c1, __c2, __d_),
                            __bind_mu<_A2>::__go(__a2_, __c1, __c2, __d_),
                            __bind_mu<_A3>::__go(__a3_, __c1, __c2, __d_));
    }
    template <class _Ca1, class _Ca2, class _Ca3> _CCW_LIBCPP_HIDE_FROM_ABI result_type operator()(_Ca1 __c1, _Ca2 __c2, _Ca3 __c3) {
        return invoke(__f_, __bind_mu<_A1>::__go(__a1_, __c1, __c2, __c3),
                            __bind_mu<_A2>::__go(__a2_, __c1, __c2, __c3),
                            __bind_mu<_A3>::__go(__a3_, __c1, __c2, __c3));
    }
};

template <class _Fp> struct is_bind_expression<__bind0<_Fp> > : public true_type {};
template <class _Fp, class _A1> struct is_bind_expression<__bind1<_Fp, _A1> > : public true_type {};
template <class _Fp, class _A1, class _A2> struct is_bind_expression<__bind2<_Fp, _A1, _A2> > : public true_type {};
template <class _Fp, class _A1, class _A2, class _A3> struct is_bind_expression<__bind3<_Fp, _A1, _A2, _A3> > : public true_type {};

template <class _Fp>
_CCW_LIBCPP_HIDE_FROM_ABI __bind0<_Fp> bind(_Fp __f) { return __bind0<_Fp>(__f); }
template <class _Fp, class _A1>
_CCW_LIBCPP_HIDE_FROM_ABI __bind1<_Fp, _A1> bind(_Fp __f, _A1 __a1) { return __bind1<_Fp, _A1>(__f, __a1); }
template <class _Fp, class _A1, class _A2>
_CCW_LIBCPP_HIDE_FROM_ABI __bind2<_Fp, _A1, _A2> bind(_Fp __f, _A1 __a1, _A2 __a2) { return __bind2<_Fp, _A1, _A2>(__f, __a1, __a2); }
template <class _Fp, class _A1, class _A2, class _A3>
_CCW_LIBCPP_HIDE_FROM_ABI __bind3<_Fp, _A1, _A2, _A3> bind(_Fp __f, _A1 __a1, _A2 __a2, _A3 __a3) { return __bind3<_Fp, _A1, _A2, _A3>(__f, __a1, __a2, __a3); }

_CCW_LIBCPP_END_NAMESPACE_STD
#endif // _CCW_LIBCPP___FUNCTIONAL_BIND_H
