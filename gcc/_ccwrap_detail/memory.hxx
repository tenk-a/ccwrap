#if __CCWRAP_CXX < 201103L

#include <cstddef>

#if defined(_MSC_VER) && _MSC_VER >= 1400
# pragma warning(push)
# pragma warning(disable:4355)
#endif

#include "../../ccwrap/ccwrap_misc/unique_ptr_cxx03.hpp"

namespace std {
    template <class InpIte, class Size, class FwdIte>
    FwdIte  uninitialized_copy_n(InpIte first, Size n, FwdIte result) {
        for ( ; n > 0; ++result, ++first, --n) {
          ::new (static_cast<void*>(&*result)) typename iterator_traits<FwdIte>::value_type(*first);
        }
    }
}

#if defined(_MSC_VER) && _MSC_VER >= 1400
# pragma warning(pop)
#endif

#endif  // __CCWRAP_NO_CXX11_MEMORY
