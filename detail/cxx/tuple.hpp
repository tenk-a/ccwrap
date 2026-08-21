/**
 *  @file   _ccwrap_detail/tuple.hpp
 *  @brief  _CCW_STD::tuple (up to 10 elements) for pre-C++11 VC, recursive head/tail.
 *  @license Boost Software License Version 1.0
 *  @note   Value elements only (no reference members / tie); no move on C++03.
 */
#ifndef _CCW_DETAIL_TUPLE_HPP
#define _CCW_DETAIL_TUPLE_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>
#include <cstddef>
#include <new>         /* placement new, for __ccw_tup_ctor below */
#include <utility>     /* std::pair, for the pair tuple-protocol below */
#include <type_traits>
#include "fnctmpl.hpp"
#include "move03.hpp"      /* _CCW_RV_REF / _CCW_MOVED_OBJ / _ccw::move */

namespace _CCW_STD {

struct __ccw_tuple_nil {};
template<class T> struct __ccw_tnil                  { enum { v = 0 }; };
template<>        struct __ccw_tnil<__ccw_tuple_nil> { enum { v = 1 }; };

template<class T0 = __ccw_tuple_nil, class T1 = __ccw_tuple_nil, class T2 = __ccw_tuple_nil,
         class T3 = __ccw_tuple_nil, class T4 = __ccw_tuple_nil, class T5 = __ccw_tuple_nil,
         class T6 = __ccw_tuple_nil, class T7 = __ccw_tuple_nil, class T8 = __ccw_tuple_nil,
         class T9 = __ccw_tuple_nil>
struct tuple {
    T0 head;
    tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, __ccw_tuple_nil> tail;

    tuple() : head(), tail() {}
    tuple(const T0& v0,
          const T1& v1 = T1(), const T2& v2 = T2(), const T3& v3 = T3(), const T4& v4 = T4(),
          const T5& v5 = T5(), const T6& v6 = T6(), const T7& v7 = T7(), const T8& v8 = T8(),
          const T9& v9 = T9())
        : head(v0), tail(v1, v2, v3, v4, v5, v6, v7, v8, v9) {}

#if !_CCW_HAS_REAL_RVALUE_REF
    tuple(_CCW_RV_REF(tuple) __r)
        : head(::_ccw::move(_CCW_MOVED_OBJ(__r).head)),
          tail(::_ccw::move(_CCW_MOVED_OBJ(__r).tail)) {}
    tuple& operator=(_CCW_RV_REF(tuple) __r) {
        tuple& __o = _CCW_MOVED_OBJ(__r);
        head = ::_ccw::move(__o.head);
        tail = ::_ccw::move(__o.tail);
        return *this;
    }
#endif

    void swap(tuple& r) { T0 t = head; head = r.head; r.head = t; tail.swap(r.tail); }
};

template<class T0>
struct tuple<T0, __ccw_tuple_nil, __ccw_tuple_nil, __ccw_tuple_nil, __ccw_tuple_nil,
             __ccw_tuple_nil, __ccw_tuple_nil, __ccw_tuple_nil, __ccw_tuple_nil, __ccw_tuple_nil> {
    T0 head;

    tuple() : head() {}
    tuple(const T0& v0,
          const __ccw_tuple_nil& = __ccw_tuple_nil(), const __ccw_tuple_nil& = __ccw_tuple_nil(),
          const __ccw_tuple_nil& = __ccw_tuple_nil(), const __ccw_tuple_nil& = __ccw_tuple_nil(),
          const __ccw_tuple_nil& = __ccw_tuple_nil(), const __ccw_tuple_nil& = __ccw_tuple_nil(),
          const __ccw_tuple_nil& = __ccw_tuple_nil(), const __ccw_tuple_nil& = __ccw_tuple_nil(),
          const __ccw_tuple_nil& = __ccw_tuple_nil())
        : head(v0) {}

#if !_CCW_HAS_REAL_RVALUE_REF
    tuple(_CCW_RV_REF(tuple) __r) : head(::_ccw::move(_CCW_MOVED_OBJ(__r).head)) {}
    tuple& operator=(_CCW_RV_REF(tuple) __r) { head = ::_ccw::move(_CCW_MOVED_OBJ(__r).head); return *this; }
#endif

    void swap(tuple& r) { T0 t = head; head = r.head; r.head = t; }
};

template<class Tup> struct tuple_size;
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
struct tuple_size<tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> > {
    enum { value = 10 - (__ccw_tnil<T0>::v + __ccw_tnil<T1>::v + __ccw_tnil<T2>::v
                       + __ccw_tnil<T3>::v + __ccw_tnil<T4>::v + __ccw_tnil<T5>::v
                       + __ccw_tnil<T6>::v + __ccw_tnil<T7>::v + __ccw_tnil<T8>::v
                       + __ccw_tnil<T9>::v) };
};

template<_CCW_STD::size_t N, class Tup> struct tuple_element;
#if defined(__WATCOMC__)
template<_CCW_STD::size_t N> struct __ccw_te_pick;
template<> struct __ccw_te_pick<0> { template<class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9> struct at { typedef A0 type; }; };
template<> struct __ccw_te_pick<1> { template<class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9> struct at { typedef A1 type; }; };
template<> struct __ccw_te_pick<2> { template<class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9> struct at { typedef A2 type; }; };
template<> struct __ccw_te_pick<3> { template<class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9> struct at { typedef A3 type; }; };
template<> struct __ccw_te_pick<4> { template<class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9> struct at { typedef A4 type; }; };
template<> struct __ccw_te_pick<5> { template<class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9> struct at { typedef A5 type; }; };
template<> struct __ccw_te_pick<6> { template<class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9> struct at { typedef A6 type; }; };
template<> struct __ccw_te_pick<7> { template<class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9> struct at { typedef A7 type; }; };
template<> struct __ccw_te_pick<8> { template<class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9> struct at { typedef A8 type; }; };
template<> struct __ccw_te_pick<9> { template<class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9> struct at { typedef A9 type; }; };
template<_CCW_STD::size_t N, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
struct tuple_element<N, tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> > {
    typedef typename __ccw_te_pick<N>::template at<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::type type;
};
#else
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
struct tuple_element<0, tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> > { typedef T0 type; };
template<_CCW_STD::size_t N, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
struct tuple_element<N, tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> > {
    typedef typename tuple_element<N - 1, tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, __ccw_tuple_nil> >::type type;
};
#endif

template<_CCW_STD::size_t N> struct __ccw_tget {
    template<class Tup> static typename tuple_element<N, Tup>::type& ref(Tup& t) { return __ccw_tget<N - 1>::ref(t.tail); }
};
template<> struct __ccw_tget<0> {
    template<class Tup> static typename tuple_element<0, Tup>::type& ref(Tup& t) { return t.head; }
};
template<_CCW_STD::size_t N> struct __ccw_tget_c {
    template<class Tup> static const typename tuple_element<N, Tup>::type& ref(const Tup& t) { return __ccw_tget_c<N - 1>::ref(t.tail); }
};
template<> struct __ccw_tget_c<0> {
    template<class Tup> static const typename tuple_element<0, Tup>::type& ref(const Tup& t) { return t.head; }
};

template<_CCW_STD::size_t N, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline typename tuple_element<N, tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> >::type&
get(tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& t) { return __ccw_tget<N>::ref(t); }

template<_CCW_STD::size_t N, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline const typename tuple_element<N, tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> >::type&
get(const tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& t) { return __ccw_tget_c<N>::ref(t); }

template<_CCW_STD::size_t _Np, class _Tp> struct __ccw_tup_ctor;

template<class _Tp> struct __ccw_tup_ctor<0, _Tp> {
    template<class _Tup> static _Tp make(const _Tup&) { return _Tp(); }
    template<class _Tup> static void ctor(void* __q, const _Tup&) { _Tp __z = _Tp(); ::new (__q) _Tp(__z); }
    template<class _A0, class _A1, class _Tup>
    static void ctor2(void* __q, const _A0& __a0, const _A1& __a1, const _Tup&) { ::new (__q) _Tp(__a0, __a1); }
};

#define _CCW_TUP_CTOR_DEF(N, ARGS, ARGSA)                                             \
template<class _Tp> struct __ccw_tup_ctor<N, _Tp> {                                   \
    template<class _Tup> static _Tp make(const _Tup& __t) { _Tp __r ARGS; return __r; } \
    template<class _Tup> static void ctor(void* __q, const _Tup& __t) { ::new (__q) _Tp ARGS; } \
    template<class _A0, class _A1, class _Tup>                                        \
    static void ctor2(void* __q, const _A0& __a0, const _A1& __a1, const _Tup& __t)   \
        { ::new (__q) _Tp ARGSA; }                                                    \
};
#define _CCW_TCE(I) __ccw_tget_c<I>::ref(__t)
_CCW_TUP_CTOR_DEF(1, (_CCW_TCE(0)), (__a0, __a1, _CCW_TCE(0)))
_CCW_TUP_CTOR_DEF(2, (_CCW_TCE(0), _CCW_TCE(1)), (__a0, __a1, _CCW_TCE(0), _CCW_TCE(1)))
_CCW_TUP_CTOR_DEF(3, (_CCW_TCE(0), _CCW_TCE(1), _CCW_TCE(2)),
                     (__a0, __a1, _CCW_TCE(0), _CCW_TCE(1), _CCW_TCE(2)))
_CCW_TUP_CTOR_DEF(4, (_CCW_TCE(0), _CCW_TCE(1), _CCW_TCE(2), _CCW_TCE(3)),
                     (__a0, __a1, _CCW_TCE(0), _CCW_TCE(1), _CCW_TCE(2), _CCW_TCE(3)))
_CCW_TUP_CTOR_DEF(5, (_CCW_TCE(0), _CCW_TCE(1), _CCW_TCE(2), _CCW_TCE(3), _CCW_TCE(4)),
                     (__a0, __a1, _CCW_TCE(0), _CCW_TCE(1), _CCW_TCE(2), _CCW_TCE(3), _CCW_TCE(4)))
_CCW_TUP_CTOR_DEF(6, (_CCW_TCE(0), _CCW_TCE(1), _CCW_TCE(2), _CCW_TCE(3), _CCW_TCE(4), _CCW_TCE(5)),
                     (__a0, __a1, _CCW_TCE(0), _CCW_TCE(1), _CCW_TCE(2), _CCW_TCE(3), _CCW_TCE(4), _CCW_TCE(5)))
_CCW_TUP_CTOR_DEF(7, (_CCW_TCE(0), _CCW_TCE(1), _CCW_TCE(2), _CCW_TCE(3), _CCW_TCE(4), _CCW_TCE(5), _CCW_TCE(6)),
                     (__a0, __a1, _CCW_TCE(0), _CCW_TCE(1), _CCW_TCE(2), _CCW_TCE(3), _CCW_TCE(4), _CCW_TCE(5), _CCW_TCE(6)))
_CCW_TUP_CTOR_DEF(8, (_CCW_TCE(0), _CCW_TCE(1), _CCW_TCE(2), _CCW_TCE(3), _CCW_TCE(4), _CCW_TCE(5), _CCW_TCE(6), _CCW_TCE(7)),
                     (__a0, __a1, _CCW_TCE(0), _CCW_TCE(1), _CCW_TCE(2), _CCW_TCE(3), _CCW_TCE(4), _CCW_TCE(5), _CCW_TCE(6), _CCW_TCE(7)))
_CCW_TUP_CTOR_DEF(9, (_CCW_TCE(0), _CCW_TCE(1), _CCW_TCE(2), _CCW_TCE(3), _CCW_TCE(4), _CCW_TCE(5), _CCW_TCE(6), _CCW_TCE(7), _CCW_TCE(8)),
                     (__a0, __a1, _CCW_TCE(0), _CCW_TCE(1), _CCW_TCE(2), _CCW_TCE(3), _CCW_TCE(4), _CCW_TCE(5), _CCW_TCE(6), _CCW_TCE(7), _CCW_TCE(8)))
_CCW_TUP_CTOR_DEF(10, (_CCW_TCE(0), _CCW_TCE(1), _CCW_TCE(2), _CCW_TCE(3), _CCW_TCE(4), _CCW_TCE(5), _CCW_TCE(6), _CCW_TCE(7), _CCW_TCE(8), _CCW_TCE(9)),
                      (__a0, __a1, _CCW_TCE(0), _CCW_TCE(1), _CCW_TCE(2), _CCW_TCE(3), _CCW_TCE(4), _CCW_TCE(5), _CCW_TCE(6), _CCW_TCE(7), _CCW_TCE(8), _CCW_TCE(9)))
#undef _CCW_TCE
#undef _CCW_TUP_CTOR_DEF

#if _CCW_STD_LIB_LT(1600, 201103L) && !(defined(__WATCOMC__) || defined(_CCW_STD_RELOCATED))
template<class _T1, class _T2> struct tuple_size< _CCW_STD::pair<_T1, _T2> > {
    enum { value = 2 };
};
template<class _T1, class _T2> struct tuple_element<0, _CCW_STD::pair<_T1, _T2> > { typedef _T1 type; };
template<class _T1, class _T2> struct tuple_element<1, _CCW_STD::pair<_T1, _T2> > { typedef _T2 type; };

template<_CCW_STD::size_t _Ip> struct __ccw_pget;
template<> struct __ccw_pget<0> {
    template<class _T1, class _T2> static _T1&       ref(_CCW_STD::pair<_T1, _T2>& __p)       { return __p.first; }
    template<class _T1, class _T2> static const _T1& ref(const _CCW_STD::pair<_T1, _T2>& __p) { return __p.first; }
};
template<> struct __ccw_pget<1> {
    template<class _T1, class _T2> static _T2&       ref(_CCW_STD::pair<_T1, _T2>& __p)       { return __p.second; }
    template<class _T1, class _T2> static const _T2& ref(const _CCW_STD::pair<_T1, _T2>& __p) { return __p.second; }
};
template<_CCW_STD::size_t _Ip, class _T1, class _T2>
inline typename tuple_element<_Ip, _CCW_STD::pair<_T1, _T2> >::type& get(_CCW_STD::pair<_T1, _T2>& __p) {
    return __ccw_pget<_Ip>::ref(__p);
}
template<_CCW_STD::size_t _Ip, class _T1, class _T2>
inline const typename tuple_element<_Ip, _CCW_STD::pair<_T1, _T2> >::type& get(const _CCW_STD::pair<_T1, _T2>& __p) {
    return __ccw_pget<_Ip>::ref(__p);
}
#endif

#if _CCW_STD_LIB_LT(1900, 201402L) && !(defined(__WATCOMC__) || defined(_CCW_STD_RELOCATED))
template<bool _Bp> struct __ccw_pget_t;
template<> struct __ccw_pget_t<true> {
    template<class _T1, class _T2> static _T1&       ref(_CCW_STD::pair<_T1, _T2>& __p)       { return __p.first; }
    template<class _T1, class _T2> static const _T1& ref(const _CCW_STD::pair<_T1, _T2>& __p) { return __p.first; }
};
template<> struct __ccw_pget_t<false> {
    template<class _T1, class _T2> static _T2&       ref(_CCW_STD::pair<_T1, _T2>& __p)       { return __p.second; }
    template<class _T1, class _T2> static const _T2& ref(const _CCW_STD::pair<_T1, _T2>& __p) { return __p.second; }
};
template<class _Tp, class _T1, class _T2>
inline _Tp& get(_CCW_STD::pair<_T1, _T2>& __p) {
    return __ccw_pget_t< _CCW_STD::is_same<_Tp, _T1>::value >::ref(__p);
}
template<class _Tp, class _T1, class _T2>
inline const _Tp& get(const _CCW_STD::pair<_T1, _T2>& __p) {
    return __ccw_pget_t< _CCW_STD::is_same<_Tp, _T1>::value >::ref(__p);
}
#endif

#if !(defined(__WATCOMC__) || defined(_CCW_STD_RELOCATED))
template<class T, class A> struct uses_allocator;
template<class T0, class T1, class T2, class T3, class T4,
         class T5, class T6, class T7, class T8, class T9, class A>
struct uses_allocator<tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>, A> {
    static const bool value = true;
};
#endif

inline tuple<> make_tuple() { return tuple<>(); }

#define _CCW_GEN_MAKE_TUPLE(N)                                                       \
    template<_CCW_FNCTMPL_TMPLARG(N)>                                                \
    inline tuple<_CCW_FNCTMPL_TYPEARG(N)> make_tuple(_CCW_FNCTMPL_DECLARG(N)) {      \
        return tuple<_CCW_FNCTMPL_TYPEARG(N)>(_CCW_FNCTMPL_FNCARG(N));               \
    }
#if _CCW_NO_VARIADIC_TEMPLATES
_CCW_GEN_MAKE_TUPLE(1)
_CCW_GEN_MAKE_TUPLE(2)
_CCW_GEN_MAKE_TUPLE(3)
_CCW_GEN_MAKE_TUPLE(4)
_CCW_GEN_MAKE_TUPLE(5)
_CCW_GEN_MAKE_TUPLE(6)
_CCW_GEN_MAKE_TUPLE(7)
_CCW_GEN_MAKE_TUPLE(8)
_CCW_GEN_MAKE_TUPLE(9)
_CCW_GEN_MAKE_TUPLE(10)
#else
_CCW_GEN_MAKE_TUPLE(1)
#endif
#undef _CCW_GEN_MAKE_TUPLE

inline tuple<> forward_as_tuple() { return tuple<>(); }

#define _CCW_GEN_FWD_AS_TUPLE(N)                                                      \
    template<_CCW_FNCTMPL_TMPLARG(N)>                                                 \
    inline tuple<_CCW_FNCTMPL_TYPEARG(N)> forward_as_tuple(_CCW_FNCTMPL_DECLARG(N)) { \
        return tuple<_CCW_FNCTMPL_TYPEARG(N)>(_CCW_FNCTMPL_FNCARG(N));                \
    }
#if _CCW_NO_VARIADIC_TEMPLATES
_CCW_GEN_FWD_AS_TUPLE(1)
_CCW_GEN_FWD_AS_TUPLE(2)
_CCW_GEN_FWD_AS_TUPLE(3)
_CCW_GEN_FWD_AS_TUPLE(4)
_CCW_GEN_FWD_AS_TUPLE(5)
_CCW_GEN_FWD_AS_TUPLE(6)
_CCW_GEN_FWD_AS_TUPLE(7)
_CCW_GEN_FWD_AS_TUPLE(8)
_CCW_GEN_FWD_AS_TUPLE(9)
_CCW_GEN_FWD_AS_TUPLE(10)
#else
_CCW_GEN_FWD_AS_TUPLE(1)
#endif
#undef _CCW_GEN_FWD_AS_TUPLE

template<_CCW_STD::size_t I, _CCW_STD::size_t N> struct __ccw_tcmp {
    template<class Tup> static bool eq(const Tup& a, const Tup& b) {
        return __ccw_tget_c<I>::ref(a) == __ccw_tget_c<I>::ref(b) && __ccw_tcmp<I + 1, N>::eq(a, b);
    }
    template<class Tup> static bool lt(const Tup& a, const Tup& b) {
        if (__ccw_tget_c<I>::ref(a) < __ccw_tget_c<I>::ref(b)) return true;
        if (__ccw_tget_c<I>::ref(b) < __ccw_tget_c<I>::ref(a)) return false;
        return __ccw_tcmp<I + 1, N>::lt(a, b);
    }
};
template<_CCW_STD::size_t N> struct __ccw_tcmp<N, N> {
    template<class Tup> static bool eq(const Tup&, const Tup&) { return true; }
    template<class Tup> static bool lt(const Tup&, const Tup&) { return false; }
};

#define _CCW_TUPLE_TY   tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>
#define _CCW_TUPLE_TP   class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9
template<_CCW_TUPLE_TP> inline bool operator==(const _CCW_TUPLE_TY& a, const _CCW_TUPLE_TY& b) { return __ccw_tcmp<0, (_CCW_STD::size_t)tuple_size<_CCW_TUPLE_TY>::value>::eq(a, b); }
template<_CCW_TUPLE_TP> inline bool operator!=(const _CCW_TUPLE_TY& a, const _CCW_TUPLE_TY& b) { return !(a == b); }
template<_CCW_TUPLE_TP> inline bool operator< (const _CCW_TUPLE_TY& a, const _CCW_TUPLE_TY& b) { return __ccw_tcmp<0, (_CCW_STD::size_t)tuple_size<_CCW_TUPLE_TY>::value>::lt(a, b); }
template<_CCW_TUPLE_TP> inline bool operator> (const _CCW_TUPLE_TY& a, const _CCW_TUPLE_TY& b) { return b < a; }
template<_CCW_TUPLE_TP> inline bool operator<=(const _CCW_TUPLE_TY& a, const _CCW_TUPLE_TY& b) { return !(b < a); }
template<_CCW_TUPLE_TP> inline bool operator>=(const _CCW_TUPLE_TY& a, const _CCW_TUPLE_TY& b) { return !(a < b); }
#undef _CCW_TUPLE_TY
#undef _CCW_TUPLE_TP

#if !defined(__WATCOMC__)
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline void swap(tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& a,
                 tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& b) { a.swap(b); }
#endif

template<class T, class Tup> struct __ccw_tfind;
template<class T, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
struct __ccw_tfind<T, tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> > {
    enum { value = _CCW_STD::is_same<T, T0>::value ? 0
                 : _CCW_STD::is_same<T, T1>::value ? 1
                 : _CCW_STD::is_same<T, T2>::value ? 2
                 : _CCW_STD::is_same<T, T3>::value ? 3
                 : _CCW_STD::is_same<T, T4>::value ? 4
                 : _CCW_STD::is_same<T, T5>::value ? 5
                 : _CCW_STD::is_same<T, T6>::value ? 6
                 : _CCW_STD::is_same<T, T7>::value ? 7
                 : _CCW_STD::is_same<T, T8>::value ? 8
                 : _CCW_STD::is_same<T, T9>::value ? 9 : -1 };
};

template<class T, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline T& get(tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& t) {
    return get<(_CCW_STD::size_t)__ccw_tfind<T, tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> >::value>(t);
}
template<class T, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline const T& get(const tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& t) {
    return get<(_CCW_STD::size_t)__ccw_tfind<T, tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> >::value>(t);
}

struct __ccw_ignore_t {
    template<class T> const __ccw_ignore_t& operator=(const T&) const { return *this; }
};
_ccw_inline_const __ccw_ignore_t ignore = __ccw_ignore_t();

}   // namespace _CCW_STD

#endif  // _CCW_DETAIL_TUPLE_HPP
