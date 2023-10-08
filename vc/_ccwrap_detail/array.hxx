#if __CCWRAP_CXX < 201103L
 #if _MSC_VER < 1600 || (_MSC_VER == 1600 && defined __CCWRAP_NO_TR1)    // vc9 or earlier or no-TR1
  #include "../../ccwrap/ccwrap_misc/array.hpp"
 #endif
#endif
