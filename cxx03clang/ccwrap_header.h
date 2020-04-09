#ifndef __CCWRAP_HEADER_H
#define __CCWRAP_HEADER_H

#define __CCWRAP_HAS_INCLUDE_NEXT

#define __CCWRAP__

#if defined _WIN32
#define __CCWRAP_LONG_BIT           32
#endif
#if defined _WIN64 || defined _M_AMD64 || defined __amd64__ || defined __AMD64__ || defined __x86_64__ || defined __arm64__ || defined __ARM64__ || defined _M_ARM64
#define __CCWRAP_NATIVE_INT_BIT     64
#else
#define __CCWRAP_NATIVE_INT_BIT     32
#endif

#ifndef __CCWRAP_M_CAT
 #define __CCWRAP_M_CAT(a,b)      __CCWRAP_M_CAT_S2(a,b)
 #define __CCWRAP_M_CAT_S2(a,b)   __CCWRAP_M_CAT_S3(a##b)
 #define __CCWRAP_M_CAT_S3(x)     x
#endif

#if defined(__cplusplus)
 #if __cplusplus < 201703
  #define __CCWRAP_NO_HEADER_STRING_VIEW    0
 #endif
 #if __cplusplus < 201100
  #ifndef nullptr
   #define nullptr                  0
  #endif
  #if !defined(alignas)
    #define alignas(a)              __attribute__((aligned(x)))
  #endif
  #if !defined(noexcept)
    #define noexcept                throw()
  #endif
  #if !defined(override)
    #define override
  #endif
  #if !defined(final)
    #define final
  #endif
  #if !defined(thread_local)
    #define thread_local
  #endif
  #if !defined(constexpr)
    #define constexpr
  #endif
  #define __CCWRAP_NO_CXX11_STRING
  #define __CCWRAP_NO_CXX11_AUTO    1
  #define __CCWRAP_NO_DECLTYPE      1
  #define __CCWRAP_NO_HEADER_ARRAY  0
  #if !defined(alignof)
    namespace __ccwrap_detail {
        template<class T> class __align_of {
            struct U { char a; T b; };
            enum { nt = sizeof(T), nu = sizeof(U), d = nu - nt };
        public:
            enum { value = (nt < d) ? nt : d };
        };
    }
    #define alignof(a)              (__ccwrap_detail::__align_of<a>::value)
  #endif
  #if !defined(static_assert)
    namespace __ccwrap { template<int x> struct static_assert_check{}; }
    template <bool x> struct __static_assert_FAILED_;
    template <> struct __static_assert_FAILED_<true> { enum { value = 1 }; };
    #define static_assert(c, ...) typedef __ccwrap::static_assert_check<sizeof(__static_assert_FAILED_<(c) != 0>)> __CCWRAP_M_CAT(__ccwrap_static_assert_L_, __LINE__)
  #endif
 #endif
#endif

#endif
