/**
 *  @file   _ccwrap_detail/move03.hpp
 *  @brief  Boost.Move-style rvalue-reference emulation for C++03 compilers.
 *  @license Boost Software License Version 1.0
 */
#ifndef _CCW_DETAIL_MOVE03_HPP
#define _CCW_DETAIL_MOVE03_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

#ifndef _CCW_HAS_REAL_RVALUE_REF
# if (defined(_MSC_VER) && _MSC_VER >= 1600) || defined(__cpp_rvalue_references) \
     || (defined(__cplusplus) && __cplusplus >= 201103L)
#  define _CCW_HAS_REAL_RVALUE_REF 1
# else
#  define _CCW_HAS_REAL_RVALUE_REF 0
# endif
#endif

#ifdef __cplusplus

namespace _ccw {

#if _CCW_HAS_REAL_RVALUE_REF

template <class _Tp> struct __ccw_remove_ref_m       { typedef _Tp type; };
template <class _Tp> struct __ccw_remove_ref_m<_Tp&> { typedef _Tp type; };
template <class _Tp> struct __ccw_remove_ref_m<_Tp&&>{ typedef _Tp type; };

template <class _Tp>
inline typename __ccw_remove_ref_m<_Tp>::type&& move(_Tp&& __v) {
    return static_cast<typename __ccw_remove_ref_m<_Tp>::type&&>(__v);
}

template <class _Tp>
inline typename __ccw_remove_ref_m<_Tp>::type&& move_if_noexcept(_Tp&& __v) {
    return static_cast<typename __ccw_remove_ref_m<_Tp>::type&&>(__v);
}

#else // !_CCW_HAS_REAL_RVALUE_REF -- emulate

template <class _Tp>
class rv : public _Tp {
private:
    rv();
    ~rv();
    rv(rv const&);
    void operator=(rv const&);
};

typedef char __ccw_yes_t;
struct __ccw_no_t { char __pad_[2]; };
template <class _Up> __ccw_yes_t __ccw_ic_test(int, void (_Up::*)(void) = 0);
template <class _Up> __ccw_no_t  __ccw_ic_test(long);

template <class _Tp> struct __ccw_is_class {
    static const bool value = sizeof(__ccw_ic_test<_Tp>(0)) == sizeof(__ccw_yes_t);
};

template <bool _IsClass, class _Tp> struct __ccw_move_ret_impl { typedef _Tp     __obj; typedef _Tp&     type; };
template <class _Tp> struct __ccw_move_ret_impl<true, _Tp>     { typedef rv<_Tp> __obj; typedef rv<_Tp>& type; };

template <class _Tp> struct __ccw_move_ret {
    typedef typename __ccw_move_ret_impl<__ccw_is_class<_Tp>::value, _Tp>::__obj __obj;
    typedef typename __ccw_move_ret_impl<__ccw_is_class<_Tp>::value, _Tp>::type  type;
};
template <class _Tp> struct __ccw_move_ret<_Tp*>      { typedef _Tp*      __obj; typedef _Tp*&      type; };
template <class _Tp> struct __ccw_move_ret<const _Tp> { typedef const _Tp __obj; typedef const _Tp& type; };

#define _CCW_DECLARE_MOVE_AS_COPY(TYPE)     namespace _ccw { template <> struct __ccw_move_ret< TYPE >         { typedef TYPE __obj; typedef TYPE& type; }; }

template <class _Tp>
inline typename __ccw_move_ret<_Tp>::type move(_Tp& __v) {
    return *(typename __ccw_move_ret<_Tp>::__obj*)(void*)&__v;
}

template <class _Tp>
inline typename __ccw_move_ret<_Tp>::type move_if_noexcept(_Tp& __v) { return move(__v); }

template <class _Tp>
inline _Tp& moved_object(rv<_Tp>& __v) { return __v; }

#endif // _CCW_HAS_REAL_RVALUE_REF

} // namespace _ccw

#if _CCW_HAS_REAL_RVALUE_REF
# define _CCW_RV_REF(TYPE)      TYPE&&
# define _CCW_RV_REF_BEG
# define _CCW_RV_REF_END        &&
# define _CCW_MOVED_OBJ(X)      (X)
# ifndef _CCW_DECLARE_MOVE_AS_COPY
#  define _CCW_DECLARE_MOVE_AS_COPY(TYPE) /* not needed with real rvalue refs */
# endif
#else
# define _CCW_RV_REF(TYPE)      ::_ccw::rv< TYPE >&
# define _CCW_RV_REF_BEG        ::_ccw::rv<
# define _CCW_RV_REF_END        >&
# define _CCW_MOVED_OBJ(X)      ::_ccw::moved_object(X)
#endif

#define _CCW_MOVE(X) ::_ccw::move(X)

#endif // __cplusplus
#endif // _CCW_DETAIL_MOVE03_HPP
