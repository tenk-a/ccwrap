/**
 * @file ccwrap_header_sub.hh
 * @brief included from ccwrap_header.h
 */
#ifndef __CCWRAP_HEADER_SUB_HH_INCLUDED
#define __CCWRAP_HEADER_SUB_HH_INCLUDED

// ==================================== ======================================= =======================================
#ifdef __cplusplus
 #if !defined(__CCWRAP_CXX11) && !defined(__CCWRAP_LESS_CXX11)
  #if __cplusplus >= 201101L
   #define __CCWRAP_CXX11        1
  #else
   #define __CCWRAP_LESS_CXX11   1
  #endif
 #endif

 #if !defined(__CCWRAP_CXX14) && !defined(__CCWRAP_LESS_CXX14)
  #if __cplusplus >= 201402L
   #define __CCWRAP_CXX14        1
  #else
   #define __CCWRAP_LESS_CXX14   1
  #endif
 #endif

 #if !defined(__CCWRAP_CXX17) && !defined(__CCWRAP_LESS_CXX17)
  #if __cplusplus >= 201703L
   #define __CCWRAP_CXX17        1
  #else
   #define __CCWRAP_LESS_CXX17   1
  #endif
 #endif

 #if !defined(__CCWRAP_CXX20) && !defined(__CCWRAP_LESS_CXX20)
  #if __cplusplus >= 202000L
   #define __CCWRAP_CXX20        1
  #else
   #define __CCWRAP_LESS_CXX20   1
  #endif
 #endif
#endif

// ==================================== ======================================= =======================================
// OS
//  ex) _WIN32,_WIN64, WINVER, _WINDOWS_, _WIN32_WINNT, _MAC
//      linux,__linux,__linux__

#if defined(_WIN32) && !defined(__CCWRAP_WIN32)
 #ifndef _WIN32_WINNT
  //#define _WIN32_WINNT    0x0500// Win2k or later.
  #define _WIN32_WINNT      0x0600 // Vista or later.
 #endif
  #define __CCWRAP_WIN32     _WIN32_WINNT
#endif

#if defined(_WIN64)
 #define __CCWRAP_WIN64
#endif

#ifndef __CCWRAP_LINUX
 #if defined linux || defined __linux || defined __linux__ || defined __LINUX__
  #define __CCWRAP_LINUX
 #endif
#endif

#ifndef __CCWRAP_OSX
 #if defined __APPLE__ || defined __APPLE_CC__
  #define __CCWRAP_APPLE
 #endif
#endif


// ==================================== ======================================= =======================================
// Compiler.
//  ex) __GNUC__,_MSC_VER, __BORLANDC__, __WATCOMC__,__INTEL_COMPILER,
//      __MWERKS__{Codewarrior}, __DMC__,__SC__{DigitalMars},


// ==================================== ======================================= =======================================
// macro

#ifndef __CCWRAP_M_CAT
#define __CCWRAP_M_CAT(a,b)                  __CCWRAP_M_CAT_S2(a,b)
#define __CCWRAP_M_CAT_S2(a,b)               a##b
#endif

#ifndef __CCWRAP_M_STR
#define __CCWRAP_M_STR(s)                    __CCWRAP_M_STR_S2(s)
#define __CCWRAP_M_STR_S2(s)                 #s
#endif

#ifndef __CCWRAP_PRIF_LL
 #define __CCWRAP_PRIF_LL                    "ll"
#endif

// ==================================== ======================================= =======================================
// CPU.
//  ex) (vc,bcc,mingw): _X86_, _MIPS_, _ALPHA_, _PPC_, _68K_, _MPPC_, _IA64_
//      (mwerks):    __INTEL__, _MC68K_, _MIPS_, _MIPS_ISA2~4_, _POWERPC_


// ------------------------------------
// endian & alignment
#if defined _M_IX86 || defined _X86_ || defined __CCWRAP_CPU_X86 || defined _M_AMD64 || defined __amd64__ || defined __AMD64__ || defined __x86_64__
  #define __CCWRAP_MEM_ACCESS_SAFE_REINTERPRET         1   // enable byte align access
  #undef  __CCWRAP_BIG_ENDIAN
  #ifndef __CCWRAP_CPU_X86
   #define __CCWRAP_CPU_X86
  #endif
  #define __CCWRAP_HAS_X87
#elif defined _M_ARM || defined _ARM_ || defined __arm__ || defined __CCWRAP_CPU_ARM
  #undef  __CCWRAP_BIG_ENDIAN
  #ifndef __CCWRAP_CPU_ARM
   #define __CCWRAP_CPU_ARM
  #endif
#elif defined _PPC_ || defined _POWERPC_ ||  || defined __CCWRAP_CPU_PPC
  #define __CCWRAP_MEM_ACCESS_SAFE_REINTERPRET         1   // enable byte align access
  #define __CCWRAP_BIG_ENDIAN                1
  #ifndef __CCWRAP_CPU_PPC
   #define __CCWRAP_CPU_PPC
  #endif
#elif defined _MIPS_ || defined __mips || defined MIPS || defined __CCWRAP_CPU_MIPS
  #undef  __CCWRAP_BIG_ENDIAN
  #ifndef __CCWRAP_CPU_MIPS
   #define __CCWRAP_CPU_MIPS
  #endif
#elif defined _68K_ || defined _MC68K_ || defined __CCWRAP_CPU_68K
  #define __CCWRAP_BIG_ENDIAN                1
  #ifndef __CCWRAP_CPU_68K
   #define __CCWRAP_CPU_68K
  #endif
#elif defined BIG_ENDIAN
  #define __CCWRAP_BIG_ENDIAN                1
#endif

#if defined __CCWRAP_BIG_ENDIAN
  #define __CCWRAP_ENDIAN                    1   // 0:little endian  1:big endian
#else
  #define __CCWRAP_ENDIAN                    0   // 0:little endian  1:big endian
#endif


// ==================================== ======================================= =======================================

#if !defined(__CCWRAP_CPU_BIT)
 #if (defined _WIN64) ||(defined __WORDSIZE && __WORDSIZE == 64) || defined _M_AMD64 || defined _M_AMD64 || defined __amd64__ || defined __AMD64__ || defined __x86_64__ || defined _IA64_
  #define __CCWRAP_CPU_BIT   64
  #define __CCWRAP_CPU64
 #else
  #define __CCWRAP_CPU_BIT   32
  #define __CCWRAP_CPU32
 #endif
#endif

#if defined(_WIN64) || defined(__CCWRAP_WIN64) //|| defined (__CCWRAP_LINUX)
 #if !defined __CCWRAP_LP64 && !defined __CCWRAP_LLP64
  #define __CCWRAP_LLP64
 #endif
#elif __CCWRAP_CPU_BIT == 64
 #if !defined __CCWRAP_LP64 && !defined __CCWRAP_LLP64
  #define __CCWRAP_LP64
 #endif
#endif


// ==================================== ======================================= =======================================
// type bits macro

#ifndef __CCWRAP_WCHAR_BIT
 #if defined(_WIN32)
  #define __CCWRAP_WCHAR_BIT                 16
 #else
  #define __CCWRAP_WCHAR_BIT                 32
 #endif
#endif

#ifndef __CCWRAP_INT_BIT
 #define __CCWRAP_INT_BIT                    32
#endif

#ifndef __CCWRAP_LONG_BIT
 #if defined(__CCWRAP_LP64)
  #define __CCWRAP_LONG_BIT                  64
 #else
  #define __CCWRAP_LONG_BIT                  32
 #endif
#endif

#ifndef __CCWRAP_LLONG_BIT
 #define __CCWRAP_LLONG_BIT                  64
#endif

#ifndef  __CCWRAP_PTR_BIT
 #if __CCWRAP_CPU_BIT == 64
  #define __CCWRAP_PTR_BIT                   64
 #else
  #define __CCWRAP_PTR_BIT                   32
 #endif
#endif

#ifndef __CCWRAP_DOUBLE_BIT
 #define __CCWRAP_DOUBLE_BIT                 64
#endif


// ==================================== ======================================= =======================================
// type define & macro

#ifndef __ccwrap_llong
 typedef long long                           __ccwrap_llong;
 typedef unsigned long long                  __ccwrap_ullong;
 #define __ccwrap_llong                      __ccwrap_llong
 #define __ccwrap_ullong                     __ccwrap_ullong
#endif

#if defined(__cpp_char8_t) // _MSC_VER >= 1922 || _MSVC_LANG >= 202000
  #define __CCWRAP_HAS_CHAR8_T               1
#else
  #define __CCWRAP_HAS_CHAR8_T               0
  typedef unsigned char   char8_t;
#endif

#ifndef  __CCWRAP_LONG_C
 #define __CCWRAP_LONG_C(val)                val##L
 #define __CCWRAP_ULONG_C(val)               val##UL
#endif
#ifndef  __CCWRAP_LLONG_C
 #define __CCWRAP_LLONG_C(val)               val##LL
 #define __CCWRAP_ULLONG_C(val)              val##ULL
#endif

#if __CCWRAP_INT_BIT < 32
 #define __CCWRAP_INT32_C(v)                 (__CCWRAP_LONG_C(v))
 #define __CCWRAP_UINT32_C(v)                (__CCWRAP_ULONG_C(v))
#else
 #define __CCWRAP_INT32_C(v)                 (v)
 #define __CCWRAP_UINT32_C(v)                (v)
#endif
#if   __CCWRAP_LONG_BIT == 64
 #define __CCWRAP_INT64_C(v)                 __CCWRAP_LONG_C(v)
 #define __CCWRAP_UINT64_C(v)                __CCWRAP_ULONG_C(v)
#elif __CCWRAP_LLONG_BIT == 64
 #define __CCWRAP_INT64_C(v)                 __CCWRAP_LLONG_C(v)
 #define __CCWRAP_UINT64_C(v)                __CCWRAP_ULLONG_C(v)
#endif


// ==================================== ======================================= =======================================
// decl. macro

#ifndef __ccwrap_forceinline
 #define __ccwrap_forceinline               inline
#endif

#ifndef __ccwrap_noinline
 #define __ccwrap_noinline
#endif

#if defined(__cplusplus)

 #ifndef  __ccwrap_noreturn
  #if defined(__CCWRAP_CXX11)
   #define __ccwrap_noreturn                 [[noreturn]]
  #else
   #define __ccwrap_noreturn
  #endif
 #endif

 #ifndef __ccwrap_deprecated
  #if defined(__CCWRAP_CXX11)
   #define __ccwrap_carries_dependency       [[carries_dependency]]
  #else
   #define __ccwrap_carries_dependency
  #endif
 #endif

 #ifndef __ccwrap_deprecated
  #if defined(__CCWRAP_CXX14)
    #define __ccwrap_deprecated              [[deprecated]]
  #else
    #define __ccwrap_deprecated
  #endif
 #endif

 #ifndef __ccwrap_fallthrough
  #if defined(__CCWRAP_CXX17)
   #define __ccwrap_fallthrough              [[fallthrough]]
  #else
   #define __ccwrap_fallthrough
  #endif
 #endif

 #ifndef __ccwrap_maybe_unused
  #if defined(__CCWRAP_CXX17)
     #define __ccwrap_maybe_unused           [[maybe_unused]]
  #else
     #define __ccwrap_maybe_unused
  #endif
 #endif

 #ifndef __ccwrap_nodiscard
  #if defined(__CCWRAP_CXX20)
   #define __ccwrap_nodiscard                [[nodiscard]]
  #else
   #define __ccwrap_nodiscard
  #endif
 #endif

 #ifndef __ccwrap_constexpr14
  #if defined(__CCWRAP_CXX14)
   #define __ccwrap_constexpr14              constexpr
  #else
   #define __ccwrap_constexpr14
  #endif
 #endif

 #ifndef __ccwrap_constexpr_or_const
  #if defined(__CCWRAP_CXX11)
   #define __ccwrap_constexpr_or_const       constexpr
  #else
   #define __ccwrap_constexpr_or_const       const
  #endif
 #endif

 #ifndef __ccwrap_move_or_swap
  #if defined(__CCWRAP_CXX11)
   #define __ccwrap_move_or_swap(l,r)        (l) = std::move(r)
  #else
   #define __ccwrap_move_or_swap(l,r)        swap((l), (r))
  #endif
 #endif

 #ifndef __ccwrap_move_or_copy
  #if defined(__CCWRAP_CXX11)
   #define __ccwrap_move_or_copy(l,r)        (l) = std::move(r)
  #else
   #define __ccwrap_move_or_copy(l,r)        (l) = (r)
  #endif
 #endif

 #ifndef __ccwrap_static_assert
  #define __ccwrap_static_assert(c)          static_assert((c), "`"#c "' is failed.")
 #endif
#endif  // __cplusplus


// ==================================== ======================================= =======================================

#if !defined(_MSC_VER)  //TODO:

// ccwrap impliment

#if !defined(__CCWRAP_CXX17)
 #if !defined(__CCWRAP_NO_HEADER_STRING_VIEW)
  #define __CCWRAP_NO_HEADER_STRING_VIEW         0
 #endif
#endif

#if !defined(__CCWRAP_CXX14)
 #if !defined(__CCWRAP_NO_CXX14_ITERATOR)
  #define __CCWRAP_NO_CXX14_ITERATOR             0
 #endif
#endif

#if !defined(__CCWRAP_CXX11)
 #if !defined(__CCWRAP_NO_CXX11_STRING)
  #define __CCWRAP_NO_CXX11_STRING               0
 #endif

 #if !defined(__CCWRAP_NO_CXX11_ITERATOR)
  #define __CCWRAP_NO_CXX11_ITERATOR             0
 #endif

 #if !defined(__CCWRAP_NO_HEADER_ARRAY)
  #define __CCWRAP_NO_HEADER_ARRAY               0
 #endif
#endif


// ------------------------------------ ---------------------------------------
#if !defined(__CCWRAP_CXX11)

 #if !defined(__CCWRAP_NO_HEADER_INITIALIZER_LIST)
  #define __CCWRAP_NO_HEADER_INITIALIZER_LIST    1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_THREAD)
  #define __CCWRAP_NO_HEADER_THREAD              1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_THREAD)
  #define __CCWRAP_NO_HEADER_THREAD              1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_ATOMIC)
  #define __CCWRAP_NO_HEADER_ATOMIC              1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_CHRONO)
  #define __CCWRAP_NO_HEADER_CHRONO              1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_CONDITION_VARIABLE)
  #define __CCWRAP_NO_HEADER_CONDITION_VARIABLE  1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_FUTURE)
  #define __CCWRAP_NO_HEADER_FUTURE              1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_MUTEX)
  #define __CCWRAP_NO_HEADER_MUTEX               1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_RATIO)
  #define __CCWRAP_NO_HEADER_RATIO               1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_SCOPED_ALLOCATOR)
  #define __CCWRAP_NO_HEADER_SCOPED_ALLOCATOR    1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_FORWARD_LIST)
  #define __CCWRAP_NO_HEADER_FORWARD_LIST        1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_RANDOM)
  #define __CCWRAP_NO_HEADER_RANDOM      1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_REGEX)
  #define __CCWRAP_NO_HEADER_REGEX       1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_TUPLE)
  #define __CCWRAP_NO_HEADER_TUPLE       1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_TYPE_TRAITS)
  #define __CCWRAP_NO_HEADER_TYPE_TRAITS     1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_UNORDERED_MAP)
  #define __CCWRAP_NO_HEADER_UNORDERED_MAP       1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_UNORDERED_SET)
  #define __CCWRAP_NO_HEADER_UNORDERED_SET       1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_CODECVT)
  #define __CCWRAP_NO_HEADER_CODECVT             1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_TYPEINDEX)
  #define __CCWRAP_NO_HEADER_TYPEINDEX           1
 #endif

 #if !defined(__CCWRAP_NO_CXX11_ALGORITHM)
  #define __CCWRAP_NO_CXX11_ALGORITHM            1
 #endif

 #if !defined(__CCWRAP_NO_CXX11_COMPLEX)
  #define __CCWRAP_NO_CXX11_COMPLEX              1
 #endif

 #if !defined(__CCWRAP_NO_CXX11_FUNCTIONAL)
  #define __CCWRAP_NO_CXX11_FUNCTIONAL           1
 #endif

 #if !defined(__CCWRAP_NO_CXX11_IOMANIP)
  #define __CCWRAP_NO_CXX11_IOMANIP              1
 #endif

 #if !defined(__CCWRAP_NO_CXX11_IOS)
  #define __CCWRAP_NO_CXX11_IOS                  1
 #endif

 #if !defined(__CCWRAP_NO_CXX11_LIMITS)
  #define __CCWRAP_NO_CXX11_LIMITS               1
 #endif

 #if !defined(__CCWRAP_NO_CXX11_MEMORY)
  #define __CCWRAP_NO_CXX11_MEMORY               1
 #endif

 #if !defined(__CCWRAP_NO_CXX11_UTILITY)
  #define __CCWRAP_NO_CXX11_UTILITY              1
 #endif

 #if !defined(__CCWRAP_NO_CXX11_LOCALE)
  #define __CCWRAP_NO_CXX11_LOCALE               1
 #endif

 #if !defined(__CCWRAP_NO_CXX11_NUMERIC)
  #define __CCWRAP_NO_CXX11_NUMERIC              1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_SYSTEM_ERROR)
  #define __CCWRAP_NO_HEADER_SYSTEM_ERROR        1
 #endif

#endif  // __CCWRAP_CXX11

#if !defined(__CCWRAP_CXX14)
 #if !defined(__CCWRAP_NO_HEADER_CXX14_MEMORY)
  #define __CCWRAP_NO_HEADER_CXX14_MEMORY    1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_SHARED_MUTEX)
  #define __CCWRAP_NO_HEADER_SHARED_MUTEX    1
 #endif

 #if !defined(__CCWRAP_NO_HEADER_CFENV)
  #define __CCWRAP_NO_HEADER_CFENV           1
 #endif
#endif  // __CCWRAP_CXX14

#if !defined(__CCWRAP_CXX17)
 #if !defined(__CCWRAP_NO_HEADER_CXX17_ITERATOR)
  #define __CCWRAP_NO_HEADER_CXX17_ITERATOR       1
 #endif
 #if !defined(__CCWRAP_NO_HEADER_STRING_VIEW)
  #define __CCWRAP_NO_HEADER_STRING_VIEW          1
 #endif
 #if !defined(__CCWRAP_NO_HEADER_FILESYSTEM)
  #define __CCWRAP_NO_HEADER_FILESYSTEM           1
 #endif
 #if !defined(__CCWRAP_NO_CXX17_ALGORITHM)
  #define __CCWRAP_NO_CXX17_ALGORITHM             1
 #endif
 #if !defined(__CCWRAP_NO_HEADER_ANY)
  #define __CCWRAP_NO_HEADER_ANY                  1
 #endif
 #if !defined(__CCWRAP_NO_HEADER_VARIANT)
  #define __CCWRAP_NO_HEADER_VARIANT              1
 #endif
 #if !defined(__CCWRAP_NO_HEADER_OPTIONAL)
  #define __CCWRAP_NO_HEADER_OPTIONAL             1
 #endif
#endif

#endif  // !defined(_MSC_VER)


// ==================================== ======================================= =======================================
#if !defined(NDEBUG) && defined(__cplusplus)
 //#if !defined(__cplusplus)
 // #include <assert.h>
 //#endif
 static_assert(__CCWRAP_INT_BIT        == 8*sizeof(int)        , "");
 //#ifdef __cplusplus
  static_assert(__CCWRAP_WCHAR_BIT     == 8*sizeof(wchar_t)    , "");
 //#endif
 static_assert(__CCWRAP_LONG_BIT       == 8*sizeof(long)       , "");
 static_assert(__CCWRAP_LLONG_BIT      == 8*sizeof(__ccwrap_llong), "");
 static_assert(__CCWRAP_DOUBLE_BIT     == 8*sizeof(double)     , "");
 static_assert(__CCWRAP_LDOUBLE_BIT    == 8*sizeof(long double), "");
 static_assert(__CCWRAP_PTR_BIT        == 8*sizeof(void*)      , "");
 static_assert(__CCWRAP_CPU_BIT == 64 || __CCWRAP_CPU_BIT == 32, "");
#endif


// ==================================== ======================================= =======================================
#ifdef __cplusplus
namespace ccwrap {}
namespace __ccwrap {}
#endif

#endif  // __ccwrap_header_sub_h_INCLUDED
