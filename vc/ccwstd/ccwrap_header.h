// ccwrap vc/ccwstd: force-include for the ccwstd environment on Visual C++.
//
// Put vc/ccwstd/ on the include path (IN PLACE OF vc/) together with detail/llibcxx03/include, and
// force-include THIS file (/FI vc/ccwstd/ccwrap_header.h). It selects namespace `ccwstd`
// for the libcxx03 C++ headers, then runs the normal vc prelude. The plain vc/ environment
// is untouched and keeps augmenting the NATIVE std.
#ifndef _CCW_STD
#define _CCW_STD ccwstd
#endif

#ifndef _CCW_LIBCXX03_HEADER_DIR
#define _CCW_LIBCXX03_HEADER_DIR ../../detail/llibcxx03/include
#endif
#ifndef _CCW_LIBCXX03_HEADER_PATH
#define _CCW_LIBCXX03_HEADER_PATH(x) <_CCW_LIBCXX03_HEADER_DIR/x>
#endif

#include "../std/ccwrap_header.h"
