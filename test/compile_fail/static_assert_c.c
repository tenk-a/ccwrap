#include <assert.h>

static_assert(sizeof(int) >= 2, "int is at least 16 bits");
static_assert(1, "a non-zero constant is accepted");

#ifdef REJ_STATIC_ASSERT_FALSE
static_assert(sizeof(int) == 999, "must be rejected");
#endif

#ifdef REJ_STATIC_ASSERT_ZERO
static_assert(0, "must be rejected");
#endif

int main(void)
{
    static_assert(sizeof(char) == 1, "char is 1 byte");
#ifdef REJ_STATIC_ASSERT_LOCAL
    static_assert(sizeof(char) == 2, "must be rejected");
#endif
    return 0;
}
