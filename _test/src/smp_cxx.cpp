#include <cstddef>
#include "array_test.hh"
#include "string_view_test.hh"
#include "string_util_test.hh"
#include <cstdint>

int main(int argc, char* argv[])
{
    using namespace std;
    printf("\n%s\n", argv[0]);
    printf("%s (%d): %s -- __FILE__,__LINE__,__func__\n", __FILE__, __LINE__, __func__);
    printf("\t__cplusplus = %d\n", int(__cplusplus));
    printf("\tsizeof(bool) = %d\n", int(sizeof(bool)));
    printf("\tsizeof(wchar_t) = %d\n", int(sizeof(wchar_t)));
    printf("\tsizeof(intmax_t) = %2d  alignof(intmax_t) = %2d\n", int(sizeof(intmax_t)), int(alignof(intmax_t)));
    printf("\tsizeof(long double) = %2d  alignof(long double) = %2d\n", int(sizeof(long double)), int(alignof(long double)));
    printf("\talignof(max_align_t) = %d\n", int(alignof(max_align_t)));
    printf("\n");

    array_test();
    to_string_smp();
    string_view_test();
    string_util_test();

    return 0;
}
