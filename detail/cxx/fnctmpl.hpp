/**
 *  @file   _ccwrap_detail/fnctmpl.hpp
 *  @brief  Macros to define "variadic" function templates (0..10 args) on compilers
 *          without variadic templates. (base: my_old_fnctmpl_gene.hpp / ara)
 *  @author Masashi Kitamura ( https://github.com/tenk-a/ )
 *  @license Boost Software License Version 1.0
 *  @note
 *      When variadic templates are available they are used directly; otherwise
 *      _CCW_FNCTMPL_GENERATE(GEN) expands GEN(0)..GEN(10).
 *
 *      _CCW_FNCTMPL_TMPLARG(N)     template parameter list:   class A1, ...
 *      _CCW_FNCTMPL_DECLARG(N)     function parameter list:   A1 a1, ...
 *      _CCW_FNCTMPL_FNCARG(N)      call argument list:        a1, ...
 *      _CCW_FNCTMPL_CM_*(N)        same, with a leading comma (for a fixed first arg)
 *      _CCW_FNCTMPL_GENERATE(GEN)  emit GEN(0)..GEN(10)
 *
 *      Example:
 *          #define GEN_MAKE(N) \
 *            template<class T _CCW_FNCTMPL_CM_TMPLARG(N)> \
 *            unique_ptr<T> make_unique(_CCW_FNCTMPL_DECLARG(N)) { return unique_ptr<T>(new T(_CCW_FNCTMPL_FNCARG(N))); }
 *          _CCW_FNCTMPL_GENERATE(GEN_MAKE)
 *          #undef GEN_MAKE
 */
#ifndef _CCW_DETAIL_FNCTMPL_HPP
#define _CCW_DETAIL_FNCTMPL_HPP
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#include <ccwrap_common.h>

#ifdef __cplusplus

#if _CCW_NO_VARIADIC_TEMPLATES

#define _CCW_FNCTMPL_TMPLARG_0
#define _CCW_FNCTMPL_DECLARG_0
#define _CCW_FNCTMPL_FNCARG_0
#define _CCW_FNCTMPL_TMPLARG_1  class _CCW_FT1
#define _CCW_FNCTMPL_DECLARG_1  _CCW_FT1 __ccw_fa1
#define _CCW_FNCTMPL_FNCARG_1   __ccw_fa1
#define _CCW_FNCTMPL_TMPLARG_2  class _CCW_FT1,class _CCW_FT2
#define _CCW_FNCTMPL_DECLARG_2  _CCW_FT1 __ccw_fa1,_CCW_FT2 __ccw_fa2
#define _CCW_FNCTMPL_FNCARG_2   __ccw_fa1,__ccw_fa2
#define _CCW_FNCTMPL_TMPLARG_3  class _CCW_FT1,class _CCW_FT2,class _CCW_FT3
#define _CCW_FNCTMPL_DECLARG_3  _CCW_FT1 __ccw_fa1,_CCW_FT2 __ccw_fa2,_CCW_FT3 __ccw_fa3
#define _CCW_FNCTMPL_FNCARG_3   __ccw_fa1,__ccw_fa2,__ccw_fa3
#define _CCW_FNCTMPL_TMPLARG_4  class _CCW_FT1,class _CCW_FT2,class _CCW_FT3,class _CCW_FT4
#define _CCW_FNCTMPL_DECLARG_4  _CCW_FT1 __ccw_fa1,_CCW_FT2 __ccw_fa2,_CCW_FT3 __ccw_fa3,_CCW_FT4 __ccw_fa4
#define _CCW_FNCTMPL_FNCARG_4   __ccw_fa1,__ccw_fa2,__ccw_fa3,__ccw_fa4
#define _CCW_FNCTMPL_TMPLARG_5  class _CCW_FT1,class _CCW_FT2,class _CCW_FT3,class _CCW_FT4,class _CCW_FT5
#define _CCW_FNCTMPL_DECLARG_5  _CCW_FT1 __ccw_fa1,_CCW_FT2 __ccw_fa2,_CCW_FT3 __ccw_fa3,_CCW_FT4 __ccw_fa4,_CCW_FT5 __ccw_fa5
#define _CCW_FNCTMPL_FNCARG_5   __ccw_fa1,__ccw_fa2,__ccw_fa3,__ccw_fa4,__ccw_fa5
#define _CCW_FNCTMPL_TMPLARG_6  class _CCW_FT1,class _CCW_FT2,class _CCW_FT3,class _CCW_FT4,class _CCW_FT5,class _CCW_FT6
#define _CCW_FNCTMPL_DECLARG_6  _CCW_FT1 __ccw_fa1,_CCW_FT2 __ccw_fa2,_CCW_FT3 __ccw_fa3,_CCW_FT4 __ccw_fa4,_CCW_FT5 __ccw_fa5,_CCW_FT6 __ccw_fa6
#define _CCW_FNCTMPL_FNCARG_6   __ccw_fa1,__ccw_fa2,__ccw_fa3,__ccw_fa4,__ccw_fa5,__ccw_fa6
#define _CCW_FNCTMPL_TMPLARG_7  class _CCW_FT1,class _CCW_FT2,class _CCW_FT3,class _CCW_FT4,class _CCW_FT5,class _CCW_FT6,class _CCW_FT7
#define _CCW_FNCTMPL_DECLARG_7  _CCW_FT1 __ccw_fa1,_CCW_FT2 __ccw_fa2,_CCW_FT3 __ccw_fa3,_CCW_FT4 __ccw_fa4,_CCW_FT5 __ccw_fa5,_CCW_FT6 __ccw_fa6,_CCW_FT7 __ccw_fa7
#define _CCW_FNCTMPL_FNCARG_7   __ccw_fa1,__ccw_fa2,__ccw_fa3,__ccw_fa4,__ccw_fa5,__ccw_fa6,__ccw_fa7
#define _CCW_FNCTMPL_TMPLARG_8  class _CCW_FT1,class _CCW_FT2,class _CCW_FT3,class _CCW_FT4,class _CCW_FT5,class _CCW_FT6,class _CCW_FT7,class _CCW_FT8
#define _CCW_FNCTMPL_DECLARG_8  _CCW_FT1 __ccw_fa1,_CCW_FT2 __ccw_fa2,_CCW_FT3 __ccw_fa3,_CCW_FT4 __ccw_fa4,_CCW_FT5 __ccw_fa5,_CCW_FT6 __ccw_fa6,_CCW_FT7 __ccw_fa7,_CCW_FT8 __ccw_fa8
#define _CCW_FNCTMPL_FNCARG_8   __ccw_fa1,__ccw_fa2,__ccw_fa3,__ccw_fa4,__ccw_fa5,__ccw_fa6,__ccw_fa7,__ccw_fa8
#define _CCW_FNCTMPL_TMPLARG_9  class _CCW_FT1,class _CCW_FT2,class _CCW_FT3,class _CCW_FT4,class _CCW_FT5,class _CCW_FT6,class _CCW_FT7,class _CCW_FT8,class _CCW_FT9
#define _CCW_FNCTMPL_DECLARG_9  _CCW_FT1 __ccw_fa1,_CCW_FT2 __ccw_fa2,_CCW_FT3 __ccw_fa3,_CCW_FT4 __ccw_fa4,_CCW_FT5 __ccw_fa5,_CCW_FT6 __ccw_fa6,_CCW_FT7 __ccw_fa7,_CCW_FT8 __ccw_fa8,_CCW_FT9 __ccw_fa9
#define _CCW_FNCTMPL_FNCARG_9   __ccw_fa1,__ccw_fa2,__ccw_fa3,__ccw_fa4,__ccw_fa5,__ccw_fa6,__ccw_fa7,__ccw_fa8,__ccw_fa9
#define _CCW_FNCTMPL_TMPLARG_10 class _CCW_FT1,class _CCW_FT2,class _CCW_FT3,class _CCW_FT4,class _CCW_FT5,class _CCW_FT6,class _CCW_FT7,class _CCW_FT8,class _CCW_FT9,class _CCW_FT10
#define _CCW_FNCTMPL_DECLARG_10 _CCW_FT1 __ccw_fa1,_CCW_FT2 __ccw_fa2,_CCW_FT3 __ccw_fa3,_CCW_FT4 __ccw_fa4,_CCW_FT5 __ccw_fa5,_CCW_FT6 __ccw_fa6,_CCW_FT7 __ccw_fa7,_CCW_FT8 __ccw_fa8,_CCW_FT9 __ccw_fa9,_CCW_FT10 __ccw_fa10
#define _CCW_FNCTMPL_FNCARG_10  __ccw_fa1,__ccw_fa2,__ccw_fa3,__ccw_fa4,__ccw_fa5,__ccw_fa6,__ccw_fa7,__ccw_fa8,__ccw_fa9,__ccw_fa10

/* _CCW_CFG_FNCTMPL_ARG_BYREF: take the arguments by reference, not by value. */
#if defined(_CCW_CFG_FNCTMPL_ARG_BYREF) && _CCW_CFG_FNCTMPL_ARG_BYREF
#undef  _CCW_FNCTMPL_DECLARG_1
#define _CCW_FNCTMPL_DECLARG_1  const _CCW_FT1& __ccw_fa1
#undef  _CCW_FNCTMPL_DECLARG_2
#define _CCW_FNCTMPL_DECLARG_2  const _CCW_FT1& __ccw_fa1,const _CCW_FT2& __ccw_fa2
#undef  _CCW_FNCTMPL_DECLARG_3
#define _CCW_FNCTMPL_DECLARG_3  const _CCW_FT1& __ccw_fa1,const _CCW_FT2& __ccw_fa2,const _CCW_FT3& __ccw_fa3
#undef  _CCW_FNCTMPL_DECLARG_4
#define _CCW_FNCTMPL_DECLARG_4  const _CCW_FT1& __ccw_fa1,const _CCW_FT2& __ccw_fa2,const _CCW_FT3& __ccw_fa3,const _CCW_FT4& __ccw_fa4
#undef  _CCW_FNCTMPL_DECLARG_5
#define _CCW_FNCTMPL_DECLARG_5  const _CCW_FT1& __ccw_fa1,const _CCW_FT2& __ccw_fa2,const _CCW_FT3& __ccw_fa3,const _CCW_FT4& __ccw_fa4,const _CCW_FT5& __ccw_fa5
#undef  _CCW_FNCTMPL_DECLARG_6
#define _CCW_FNCTMPL_DECLARG_6  const _CCW_FT1& __ccw_fa1,const _CCW_FT2& __ccw_fa2,const _CCW_FT3& __ccw_fa3,const _CCW_FT4& __ccw_fa4,const _CCW_FT5& __ccw_fa5,const _CCW_FT6& __ccw_fa6
#undef  _CCW_FNCTMPL_DECLARG_7
#define _CCW_FNCTMPL_DECLARG_7  const _CCW_FT1& __ccw_fa1,const _CCW_FT2& __ccw_fa2,const _CCW_FT3& __ccw_fa3,const _CCW_FT4& __ccw_fa4,const _CCW_FT5& __ccw_fa5,const _CCW_FT6& __ccw_fa6,const _CCW_FT7& __ccw_fa7
#undef  _CCW_FNCTMPL_DECLARG_8
#define _CCW_FNCTMPL_DECLARG_8  const _CCW_FT1& __ccw_fa1,const _CCW_FT2& __ccw_fa2,const _CCW_FT3& __ccw_fa3,const _CCW_FT4& __ccw_fa4,const _CCW_FT5& __ccw_fa5,const _CCW_FT6& __ccw_fa6,const _CCW_FT7& __ccw_fa7,const _CCW_FT8& __ccw_fa8
#undef  _CCW_FNCTMPL_DECLARG_9
#define _CCW_FNCTMPL_DECLARG_9  const _CCW_FT1& __ccw_fa1,const _CCW_FT2& __ccw_fa2,const _CCW_FT3& __ccw_fa3,const _CCW_FT4& __ccw_fa4,const _CCW_FT5& __ccw_fa5,const _CCW_FT6& __ccw_fa6,const _CCW_FT7& __ccw_fa7,const _CCW_FT8& __ccw_fa8,const _CCW_FT9& __ccw_fa9
#undef  _CCW_FNCTMPL_DECLARG_10
#define _CCW_FNCTMPL_DECLARG_10 const _CCW_FT1& __ccw_fa1,const _CCW_FT2& __ccw_fa2,const _CCW_FT3& __ccw_fa3,const _CCW_FT4& __ccw_fa4,const _CCW_FT5& __ccw_fa5,const _CCW_FT6& __ccw_fa6,const _CCW_FT7& __ccw_fa7,const _CCW_FT8& __ccw_fa8,const _CCW_FT9& __ccw_fa9,const _CCW_FT10& __ccw_fa10
#endif

#define _CCW_FNCTMPL_CM_TMPLARG_0
#define _CCW_FNCTMPL_CM_DECLARG_0
#define _CCW_FNCTMPL_CM_FNCARG_0
#define _CCW_FNCTMPL_CM_TMPLARG_1  ,_CCW_FNCTMPL_TMPLARG_1
#define _CCW_FNCTMPL_CM_DECLARG_1  ,_CCW_FNCTMPL_DECLARG_1
#define _CCW_FNCTMPL_CM_FNCARG_1   ,_CCW_FNCTMPL_FNCARG_1
#define _CCW_FNCTMPL_CM_TMPLARG_2  ,_CCW_FNCTMPL_TMPLARG_2
#define _CCW_FNCTMPL_CM_DECLARG_2  ,_CCW_FNCTMPL_DECLARG_2
#define _CCW_FNCTMPL_CM_FNCARG_2   ,_CCW_FNCTMPL_FNCARG_2
#define _CCW_FNCTMPL_CM_TMPLARG_3  ,_CCW_FNCTMPL_TMPLARG_3
#define _CCW_FNCTMPL_CM_DECLARG_3  ,_CCW_FNCTMPL_DECLARG_3
#define _CCW_FNCTMPL_CM_FNCARG_3   ,_CCW_FNCTMPL_FNCARG_3
#define _CCW_FNCTMPL_CM_TMPLARG_4  ,_CCW_FNCTMPL_TMPLARG_4
#define _CCW_FNCTMPL_CM_DECLARG_4  ,_CCW_FNCTMPL_DECLARG_4
#define _CCW_FNCTMPL_CM_FNCARG_4   ,_CCW_FNCTMPL_FNCARG_4
#define _CCW_FNCTMPL_CM_TMPLARG_5  ,_CCW_FNCTMPL_TMPLARG_5
#define _CCW_FNCTMPL_CM_DECLARG_5  ,_CCW_FNCTMPL_DECLARG_5
#define _CCW_FNCTMPL_CM_FNCARG_5   ,_CCW_FNCTMPL_FNCARG_5
#define _CCW_FNCTMPL_CM_TMPLARG_6  ,_CCW_FNCTMPL_TMPLARG_6
#define _CCW_FNCTMPL_CM_DECLARG_6  ,_CCW_FNCTMPL_DECLARG_6
#define _CCW_FNCTMPL_CM_FNCARG_6   ,_CCW_FNCTMPL_FNCARG_6
#define _CCW_FNCTMPL_CM_TMPLARG_7  ,_CCW_FNCTMPL_TMPLARG_7
#define _CCW_FNCTMPL_CM_DECLARG_7  ,_CCW_FNCTMPL_DECLARG_7
#define _CCW_FNCTMPL_CM_FNCARG_7   ,_CCW_FNCTMPL_FNCARG_7
#define _CCW_FNCTMPL_CM_TMPLARG_8  ,_CCW_FNCTMPL_TMPLARG_8
#define _CCW_FNCTMPL_CM_DECLARG_8  ,_CCW_FNCTMPL_DECLARG_8
#define _CCW_FNCTMPL_CM_FNCARG_8   ,_CCW_FNCTMPL_FNCARG_8
#define _CCW_FNCTMPL_CM_TMPLARG_9  ,_CCW_FNCTMPL_TMPLARG_9
#define _CCW_FNCTMPL_CM_DECLARG_9  ,_CCW_FNCTMPL_DECLARG_9
#define _CCW_FNCTMPL_CM_FNCARG_9   ,_CCW_FNCTMPL_FNCARG_9
#define _CCW_FNCTMPL_CM_TMPLARG_10 ,_CCW_FNCTMPL_TMPLARG_10
#define _CCW_FNCTMPL_CM_DECLARG_10 ,_CCW_FNCTMPL_DECLARG_10
#define _CCW_FNCTMPL_CM_FNCARG_10  ,_CCW_FNCTMPL_FNCARG_10

#define _CCW_FNCTMPL_TYPEARG_0
#define _CCW_FNCTMPL_TYPEARG_1  _CCW_FT1
#define _CCW_FNCTMPL_TYPEARG_2  _CCW_FT1,_CCW_FT2
#define _CCW_FNCTMPL_TYPEARG_3  _CCW_FT1,_CCW_FT2,_CCW_FT3
#define _CCW_FNCTMPL_TYPEARG_4  _CCW_FT1,_CCW_FT2,_CCW_FT3,_CCW_FT4
#define _CCW_FNCTMPL_TYPEARG_5  _CCW_FT1,_CCW_FT2,_CCW_FT3,_CCW_FT4,_CCW_FT5
#define _CCW_FNCTMPL_TYPEARG_6  _CCW_FT1,_CCW_FT2,_CCW_FT3,_CCW_FT4,_CCW_FT5,_CCW_FT6
#define _CCW_FNCTMPL_TYPEARG_7  _CCW_FT1,_CCW_FT2,_CCW_FT3,_CCW_FT4,_CCW_FT5,_CCW_FT6,_CCW_FT7
#define _CCW_FNCTMPL_TYPEARG_8  _CCW_FT1,_CCW_FT2,_CCW_FT3,_CCW_FT4,_CCW_FT5,_CCW_FT6,_CCW_FT7,_CCW_FT8
#define _CCW_FNCTMPL_TYPEARG_9  _CCW_FT1,_CCW_FT2,_CCW_FT3,_CCW_FT4,_CCW_FT5,_CCW_FT6,_CCW_FT7,_CCW_FT8,_CCW_FT9
#define _CCW_FNCTMPL_TYPEARG_10 _CCW_FT1,_CCW_FT2,_CCW_FT3,_CCW_FT4,_CCW_FT5,_CCW_FT6,_CCW_FT7,_CCW_FT8,_CCW_FT9,_CCW_FT10

#define _CCW_FNCTMPL_TMPLARG(N)     _CCW_FNCTMPL_TMPLARG_##N
#define _CCW_FNCTMPL_DECLARG(N)     _CCW_FNCTMPL_DECLARG_##N
#define _CCW_FNCTMPL_FNCARG(N)      _CCW_FNCTMPL_FNCARG_##N
#define _CCW_FNCTMPL_TYPEARG(N)     _CCW_FNCTMPL_TYPEARG_##N
#define _CCW_FNCTMPL_CM_TMPLARG(N)  _CCW_FNCTMPL_CM_TMPLARG_##N
#define _CCW_FNCTMPL_CM_DECLARG(N)  _CCW_FNCTMPL_CM_DECLARG_##N
#define _CCW_FNCTMPL_CM_FNCARG(N)   _CCW_FNCTMPL_CM_FNCARG_##N

#define _CCW_FNCTMPL_GENERATE(GEN)  GEN(0) GEN(1) GEN(2) GEN(3) GEN(4) GEN(5) GEN(6) GEN(7) GEN(8) GEN(9) GEN(10)

#else   // variadic templates available

#define _CCW_FNCTMPL_TMPLARG(DMY)       class... _CCW_FTs
#define _CCW_FNCTMPL_DECLARG(DMY)       _CCW_FTs&&... __ccw_fas
#define _CCW_FNCTMPL_FNCARG(DMY)        static_cast<_CCW_FTs&&>(__ccw_fas)...
#define _CCW_FNCTMPL_TYPEARG(DMY)       _CCW_FTs...
#define _CCW_FNCTMPL_CM_TMPLARG(DMY)    , _CCW_FNCTMPL_TMPLARG(DMY)
#define _CCW_FNCTMPL_CM_DECLARG(DMY)    , _CCW_FNCTMPL_DECLARG(DMY)
#define _CCW_FNCTMPL_CM_FNCARG(DMY)     , _CCW_FNCTMPL_FNCARG(DMY)

#define _CCW_FNCTMPL_GENERATE(GEN)      GEN(0)

#define _CCW_FNCTMPL_TMPLARG_SIZE       10

#endif  // _CCW_NO_VARIADIC_TEMPLATES

#endif  // __cplusplus

#endif  // _CCW_DETAIL_FNCTMPL_HPP
