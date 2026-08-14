/* The same checks as cstdarg_test.cpp, but against <stdarg.h>: [depr.c.headers]
   requires every name <cstdarg> puts in namespace std to be in the global
   namespace as well.  STD_NS becomes empty here. */
#include <stdarg.h>
#define TST_GLOBAL_C_NAMES 1
#include "test_cxx.hpp"

#undef  STD_NS
#define STD_NS

#undef  TEST_GROUP_SUFFIX
#define TEST_GROUP_SUFFIX _h

#include "cstdarg_test.cpp"
