#if defined(__cpp_char8_t) || defined(__CCWRAP_HAS_CHAR8_T)
namespace std {
 typedef basic_string<char8_t>  u8string;
}
#endif

#if __CCWRAP_CXX && __CCWRAP_CXX < 201103L
namespace std {
    typedef basic_string<char16_t>  u16string;
    typedef basic_string<char32_t>  u32string;
}
#include "../../ccwrap/ccwrap_misc/string_sub.hpp"
#endif
