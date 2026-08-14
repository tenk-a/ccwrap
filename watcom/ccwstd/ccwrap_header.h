// ccwrap watcom/ccwstd: force-include for the ccwstd environment.
//
// Put watcom/ccwstd/ on the include path (in place of watcom/) and force-include THIS file
// (-fi=watcom/ccwstd/ccwrap_header.h). It selects namespace `ccwstd` for the libcxx03 C++
// headers, then runs the normal Watcom prelude. The plain watcom/ environment is unchanged
// and keeps everything in std. The thin wrappers in this directory forward each header to
// its watcom/ counterpart; libcxx03 then lands in `ccwstd` because _CCW_STD is set here.
#ifndef _CCW_STD
#define _CCW_STD ccwstd
#endif
#ifndef _CCW_NATIVE_STL_HEADER_DIR
#define _CCW_NATIVE_STL_HEADER_DIR ../../detail/llibcxx03/include
#endif
#include "../std/ccwrap_header.h"
