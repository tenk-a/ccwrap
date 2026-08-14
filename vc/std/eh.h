// ccwrap <eh.h>
#pragma once

#include <ccwrap_common.h>

#if __cplusplus < 202302L && _CCW_TARGET_CXX >= 2023
 #pragma push_macro("unexpected")
 #undef unexpected
 #define unexpected __ccw_native_unexpected
 #include _CCW_NATIVE_STL_HEADER_PATH(eh.h)
 #undef unexpected
 #pragma pop_macro("unexpected")
#else
 #include _CCW_NATIVE_STL_HEADER_PATH(eh.h)
#endif
