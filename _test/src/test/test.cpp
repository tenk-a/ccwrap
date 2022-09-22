#include <cstdio>
#include <cstdint>
#include <ccwrap/ccwrap_test.hpp>

#ifndef __ccwrap_llong
#define __ccwrap_llong   long long
#endif

int main(int argc, char* argv[])
{
    static_assert(true, "");
    std::printf("\n%s\n", argv[0]);
 #if defined(__CCWRAP_COMPILER)
    std::printf("%s\n", __CCWRAP_COMPILER);
 #endif
    std::printf("%s (%d): %s\n",__FILE__,__LINE__,__func__);
    std::printf("\t__cplusplus = %d\n", int(__cplusplus));
    std::printf("\tsizeof(bool) = %d\n", int(sizeof(bool)));
    std::printf("\tsizeof(wchar_t) = %d\n", int(sizeof(wchar_t)));
    std::printf("\tsizeof(long long) = %d\n", int(sizeof(__ccwrap_llong)));
    std::printf("\tsizeof(std::intmax_t) = %2d  alignof(std::intmax_t) = %2d\n", int(sizeof(std::intmax_t)), int(alignof(std::intmax_t)));
    std::printf("\tsizeof(long double) = %2d  alignof(long double) = %2d\n", int(sizeof(long double)), int(alignof(long double)));
    std::printf("\talignof(std::max_align_t) = %d\n", int(alignof(std::max_align_t)));

    CCWRAP_TEST_RUN();
    return 0;
}
