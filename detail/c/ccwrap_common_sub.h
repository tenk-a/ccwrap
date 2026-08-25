/**
 *  @file   detail/ccwrap_common_sub.h
 *  @brief  Compiler-agnostic body shared by vc/, gcc/ and watcom/ ccwrap_common.h.
 *  @author tenka@6809.net (Masashi Kitamura)
 *  @license Boost Software License Version 1.0
 *  @note   Each vc/ gcc/ watcom/ ccwrap_common.h includes its own ccwrap_header.h
 *          first (which sets _ccw_* attributes, _CCW_TARGET_CXX, etc.), then this
 *          file. Everything here is written against those macros and the standard
 *          predefined ones (_MSC_VER / __GNUC__ / __WATCOMC__ / __cplusplus), so it
 *          is common to all configs. Do not include this directly.
 */
#ifndef _CCW_DETAIL_CCWRAP_COMMON_SUB_H
#define _CCW_DETAIL_CCWRAP_COMMON_SUB_H
#ifdef _CCW_HAS_PRAGMA_ONCE
# pragma once
#endif

#if !defined(_CCW_MESSAGE)
 #define _CCW_MESSAGE(x)
#endif

#if defined(__cplusplus)
#define _CCW_EXTERN_C   extern "C"
#else
#define _CCW_EXTERN_C   extern
#endif

#if defined(_CCW_LIBRARY_LIB_FILE_GENERATE)
#undef  _CCW_LIBRARY_HEADER_ONLY
#undef  _CCW_LIBRARY_LIB_FILE
#define _CCW_LIBRARY_LIB_FILE
#elif !defined(_CCW_LIBRARY_HEADER_ONLY) && !defined(_CCW_LIBRARY_LIB_FILE)
#define _CCW_LIBRARY_HEADER_ONLY
#endif

#if defined(_WIN32)
#define _CCW_WINAPI(ty)                     _ccw_dllimport ty _ccw_stdcall
#endif

#define _CCW_F_DECL(ty, name, params)       _CCW_EXTERN_C ty _ccw_native_decl name params
#define _CCW_C_DECL(ty, name, params)       _CCW_EXTERN_C ty _ccw_cdecl       name params
#define _CCW_I_DECL(ty, name, params)       static inline ty _ccw_native_decl name params


#define _CCW_F_INL(FnPr, ty, name, params, args) \
            static _ccw_forceinline ty name params

#if defined(_CCW_LIBRARY_LIB_FILE_GENERATE)
 #define _CCW_F_DEF(FnPr, ty, name, params, args) \
            _CCW_EXTERN_C ty _ccw_native_decl name params
 #define _CCW_C_DEF(FnPr, ty, name, params, args) \
            _CCW_EXTERN_C ty _ccw_cdecl name params
 #define _CCW_DETAIL_DEF(ty, name, params) \
            _CCW_EXTERN_C ty _ccw_native_decl _CCW_M_CAT(__ccw_DETAIL_,name) params
 #define _CCW_DETAIL_INL(ty, name, params) \
            static _ccw_forceinline ty _ccw_native_decl _CCW_M_CAT(__ccw_DETAIL_,name) params
 #define _CCW_DETAIL_CALL(fn, ...) \
            _CCW_M_CAT(__ccw_DETAIL_,fn)(__VA_ARGS__)
 #define _CCW_DETAIL_PTR(fn) \
            _CCW_M_CAT(__ccw_DETAIL_,fn)
#elif defined(_CCW_LIBRARY_LIB_FILE)
 #define _CCW_F_DEF(FnPr, ty, name, params, args) \
            _CCW_EXTERN_C ty _ccw_native_decl name params; \
            static inline ty _ccw_native_decl _CCW_M_CAT(__ccw_DUMMY_,name) params
 #define _CCW_C_DEF(FnPr, ty, name, params, args) \
            _CCW_EXTERN_C ty _ccw_cdecl name params; \
            static inline ty _ccw_cdecl _CCW_M_CAT(__ccw_DUMMY_,name) params
 #define _CCW_DETAIL_DEF(ty, name, params) \
            _CCW_EXTERN_C ty _ccw_native_decl _CCW_M_CAT(__ccw_DETAIL_,name) params; \
            static inline ty _ccw_native_decl _CCW_M_CAT(__ccw_DUMMY_,name) params
 #define _CCW_DETAIL_INL(ty, name, params) \
            _CCW_EXTERN_C ty _ccw_native_decl _CCW_M_CAT(__ccw_DETAIL_,name) params; \
            static inline ty _ccw_native_decl _CCW_M_CAT(__ccw_DUMMY_,name) params
 #define _CCW_DETAIL_CALL(fn, ...) \
            _CCW_M_CAT(__ccw_DETAIL_,fn)(__VA_ARGS__)
 #define _CCW_DETAIL_PTR(fn) \
            _CCW_M_CAT(__ccw_DETAIL_,fn)
#else   // _CCW_LIBRARY_HEADER_ONLY
 #if defined(__cplusplus)
  #if defined(_MSC_VER) && _MSC_VER < 1800
  #define _CCW_F_DEF(FnPr, ty, name, params, args) \
            static _ccw_forceinline ty _ccw_native_decl name params
  #define _CCW_C_DEF(FnPr, ty, name, params, args) \
            static _ccw_forceinline ty _ccw_cdecl name params
  #define _CCW_DETAIL_DEF(ty, name, params) \
            static _ccw_forceinline ty _ccw_native_decl _CCW_M_CAT(__ccw_DETAIL_,name) params
  #define _CCW_DETAIL_INL(ty, name, params) \
            static _ccw_forceinline ty _ccw_native_decl _CCW_M_CAT(__ccw_DETAIL_,name) params
  #define _CCW_DETAIL_CALL(fn, ...) \
            _CCW_M_CAT(__ccw_DETAIL_,fn)(__VA_ARGS__)
  #define _CCW_DETAIL_PTR(fn) \
            _CCW_M_CAT(__ccw_DETAIL_,fn)
  #else
  #define _CCW_F_DEF(FnPr, ty, name, params, args) \
            extern "C++" template<class __DMY> ty _ccw_native_decl _CCW_M_CAT(__ccw_DETAIL_,name) params; \
            static _ccw_forceinline ty _ccw_native_decl name params { \
                _CCW_M_CAT(__ccw_ret_,FnPr) _CCW_M_CAT(__ccw_DETAIL_,name)<void> args; \
            } \
            extern "C++" template<class __DMY> ty _ccw_native_decl _CCW_M_CAT(__ccw_DETAIL_,name) params
  #define _CCW_C_DEF(FnPr, ty, name, params, args) \
            extern "C++" template<class __DMY> ty _ccw_cdecl _CCW_M_CAT(__ccw_DETAIL_,name) params; \
            static _ccw_forceinline ty _ccw_cdecl name params { \
                _CCW_M_CAT(__ccw_ret_,FnPr) _CCW_M_CAT(__ccw_DETAIL_,name)<void> args; \
            } \
            extern "C++" template<class __DMY> ty _ccw_cdecl _CCW_M_CAT(__ccw_DETAIL_,name) params
  #define _CCW_DETAIL_DEF(ty, name, params) \
            extern "C++" template<class __DMY> ty _ccw_native_decl _CCW_M_CAT(__ccw_DETAIL_,name) params
  #define _CCW_DETAIL_INL(ty, name, params) \
            extern "C++" template<class __DMY> static _ccw_forceinline ty _ccw_native_decl _CCW_M_CAT(__ccw_DETAIL_,name) params
  #define _CCW_DETAIL_CALL(fn, ...) \
            _CCW_M_CAT(__ccw_DETAIL_,fn)<void>(__VA_ARGS__)
  #define _CCW_DETAIL_PTR(fn) \
            _CCW_M_CAT(__ccw_DETAIL_,fn)<void>
  #endif  // _MSC_VER < 1800
 #else  // !defined(__cplusplus)
  #define _CCW_F_DEF(FnPr, ty, name, params, args) \
            static inline ty _ccw_native_decl name params
  #define _CCW_C_DEF(FnPr, ty, name, params, args) \
            static inline ty _ccw_cdecl name params
  #define _CCW_DETAIL_DEF(ty, name, params) \
            static inline ty _ccw_native_decl _CCW_M_CAT(__ccw_DETAIL_,name) params
  #define _CCW_DETAIL_INL(ty, name, params) \
            static _ccw_forceinline ty _ccw_native_decl _CCW_M_CAT(__ccw_DETAIL_,name) params
  #define _CCW_DETAIL_CALL(fn, ...) \
            _CCW_M_CAT(__ccw_DETAIL_,fn)(__VA_ARGS__)
  #define _CCW_DETAIL_PTR(fn) \
            _CCW_M_CAT(__ccw_DETAIL_,fn)
 #endif
#endif

#define __ccw_ret_FUNC  return
#define __ccw_ret_PROC

#ifndef _CCW_ASSERT
#define _CCW_ASSERT(...)        assert(__VA_ARGS__)
#endif

#if defined(__cplusplus)
#ifndef _CCW_HAS_EXCEPTIONS
#define _CCW_HAS_EXCEPTIONS 1
#endif
#ifndef _CCW_HAS_RTTI
#define _CCW_HAS_RTTI 1
#endif

#if _CCW_HAS_EXCEPTIONS
#define _CCW_TRY                try
#define _CCW_CATCH(decl)        catch (decl)
#define _CCW_CATCH_ALL          catch (...)
#define _CCW_THROW(x)           throw (x)
#define _CCW_RETHROW            throw
#else
#define _CCW_TRY                if (1)
#define _CCW_CATCH(decl)        else if (0)
#define _CCW_CATCH_ALL          else if (0)
#define _CCW_THROW(x)           ::__ccw_eh_fail(#x, __FILE__, __LINE__)
#define _CCW_RETHROW            ::__ccw_eh_fail("rethrow", __FILE__, __LINE__)
#endif
#endif  // defined(__cplusplus)

#if defined(__cplusplus)
#ifndef _CCW_STD
#define _CCW_STD std
#else   // _CCW_STD was pre-defined
#define _CCW_STD_RELOCATED 1
#endif
#if _CCW_TARGET_CXX >= 2014
#define _CCW_STD14 std
#else
#define _CCW_STD14 _ccw
#endif
#if _CCW_TARGET_CXX >= 2017
#define _CCW_STD17 std
#else
#define _CCW_STD17 _ccw
#endif
#if _CCW_TARGET_CXX >= 2020
#define _CCW_STD20 std
#else
#define _CCW_STD20 _ccw
#endif
#if _CCW_TARGET_CXX >= 2023
#define _CCW_STD23 std
#else
#define _CCW_STD23 _ccw
#endif
#if _CCW_TARGET_CXX >= 2026
#define _CCW_STD26 std
#else
#define _CCW_STD26 _ccw
#endif

#if defined(_MSC_VER)
 #define _CCW_STD_LIB_LT(vcver, cxxstd) (_MSC_VER < (vcver))
#elif defined(_CCW_STD_LIB_LIBCXX)
 #define _CCW_STD_LIB_LT(vcver, cxxstd) ((__cplusplus > 201103L ? __cplusplus : 201103L) < (cxxstd))
#else
 #define _CCW_STD_LIB_LT(vcver, cxxstd) (__cplusplus < (cxxstd))
#endif

#if defined(__GNUC__) && (defined(__ELF__) || defined(__APPLE__))
#define _CCW_VIS_HIDDEN_BEGIN   _Pragma("GCC visibility push(hidden)")
#define _CCW_VIS_HIDDEN_END     _Pragma("GCC visibility pop")
#else
#define _CCW_VIS_HIDDEN_BEGIN
#define _CCW_VIS_HIDDEN_END
#endif
#endif  // defined(__cplusplus)

#if defined(__cplusplus)
namespace std {}
namespace _ccw {
    using namespace ::std;
}
#endif

#if defined(__cplusplus) && !defined(_MSC_VER) && !defined(__WATCOMC__) && !defined(_CCW_NO_EXPECTED_CXX23)
 #define _CCW_NO_EXPECTED_CXX23
#endif

#endif  // _CCW_DETAIL_CCWRAP_COMMON_SUB_H
