// ccwrap <fenv.h>
#pragma once

#include <ccwrap_common.h>

#if !defined(_MSC_VER) || _MSC_VER >= 1800

 #include _CCW_NATIVE_UC_HEADER_PATH(fenv.h)

 #if _MSC_VER == 1800
  #include <float.h>
  static __inline int __ccw_fesetenv_dfl(const fenv_t* __e) {
      if (__e == FE_DFL_ENV) { _fpreset(); return 0; }   /* native leaves x87 unmasked */
      return fesetenv(__e);                              /* (native; macro defined below) */
  }
  #define fesetenv(e) __ccw_fesetenv_dfl(e)
 #endif

#else

#include <float.h>

typedef struct { unsigned int _ctrl; unsigned int _stat; } fenv_t;
typedef unsigned int fexcept_t;

#define FE_INEXACT      _SW_INEXACT
#define FE_UNDERFLOW    _SW_UNDERFLOW
#define FE_OVERFLOW     _SW_OVERFLOW
#define FE_DIVBYZERO    _SW_ZERODIVIDE
#define FE_INVALID      _SW_INVALID
#define FE_ALL_EXCEPT   (FE_INEXACT | FE_UNDERFLOW | FE_OVERFLOW | FE_DIVBYZERO | FE_INVALID)

#define FE_TONEAREST    _RC_NEAR
#define FE_DOWNWARD     _RC_DOWN
#define FE_UPWARD       _RC_UP
#define FE_TOWARDZERO   _RC_CHOP

#define FE_DFL_ENV      ((const fenv_t*)0)

static __inline int fetestexcept(int excepts) {
    return (int)(_statusfp() & (unsigned int)excepts & FE_ALL_EXCEPT);
}
static __inline int feraiseexcept(int excepts) {
    volatile double __z = 0.0, __one = 1.0, __r = 0.0;
    if (excepts & FE_INVALID)   { __r = __z / __z; }
    if (excepts & FE_DIVBYZERO) { __r = __one / __z; }
    if (excepts & FE_OVERFLOW)  { volatile double __b = 1e308;   __r = __b * __b; }
    if (excepts & FE_UNDERFLOW) { volatile double __s = 1e-308;  __r = __s * __s; }
    if (excepts & FE_INEXACT)   { __r = __one / 3.0; }
    (void)__r;
    return 0;
}
static __inline int feclearexcept(int excepts) {
    unsigned int __keep = _statusfp() & FE_ALL_EXCEPT & ~(unsigned int)excepts;
    _clearfp();
    if (__keep) feraiseexcept((int)__keep);
    return 0;
}
static __inline int fegetexceptflag(fexcept_t* flagp, int excepts) {
    *flagp = (fexcept_t)(_statusfp() & (unsigned int)excepts & FE_ALL_EXCEPT);
    return 0;
}
static __inline int fesetexceptflag(const fexcept_t* flagp, int excepts) {
    unsigned int __want = (unsigned int)*flagp & (unsigned int)excepts & FE_ALL_EXCEPT;
    unsigned int __keep = _statusfp() & FE_ALL_EXCEPT & ~(unsigned int)excepts;
    _clearfp();
    if (__keep | __want) feraiseexcept((int)(__keep | __want));
    return 0;
}
#if defined(_M_IX86)
#  define _CCW_FENV_CW_MASK (_MCW_DN | _MCW_EM | _MCW_RC | _MCW_PC | _MCW_IC)
#else
#  define _CCW_FENV_CW_MASK (_MCW_DN | _MCW_EM | _MCW_RC)
#endif

static __inline int fegetround(void) {
    unsigned int c;
    _controlfp_s(&c, 0, 0);
    return (int)(c & _MCW_RC);
}
static __inline int fesetround(int mode) {
    unsigned int c;
    _controlfp_s(&c, (unsigned int)mode, _MCW_RC);
    return 0;
}
static __inline int fegetenv(fenv_t* envp) {
    unsigned int c;
    _controlfp_s(&c, 0, 0);
    envp->_ctrl = c;
    envp->_stat = _statusfp();
    return 0;
}
static __inline int fesetenv(const fenv_t* envp) {
    unsigned int c;
    if (envp == FE_DFL_ENV || envp == 0) {
        _fpreset();
    } else {
        _controlfp_s(&c, envp->_ctrl, _CCW_FENV_CW_MASK);
        _clearfp();
        if (envp->_stat & FE_ALL_EXCEPT) feraiseexcept((int)(envp->_stat & FE_ALL_EXCEPT));
    }
    return 0;
}
static __inline int feholdexcept(fenv_t* envp) {
    unsigned int c;
    fegetenv(envp);
    _clearfp();
    _controlfp_s(&c, _MCW_EM, _MCW_EM);   /* mask all -> non-stop mode */
    return 0;
}
static __inline int feupdateenv(const fenv_t* envp) {
    unsigned int __raised = _statusfp() & FE_ALL_EXCEPT;
    fesetenv(envp);
    if (__raised) feraiseexcept((int)__raised);
    return 0;
}

#endif
