#include "c_test.h"
#include "test_env.h"

TEST_DECL_BEGIN()
#ifdef TST_HAVE_assert
    TEST_DECL_SUITE(assert)
#endif
#ifdef TST_HAVE_complex
    TEST_DECL_SUITE(complex)
#endif
#ifdef TST_HAVE_ctype
    TEST_DECL_SUITE(ctype)
#endif
#ifdef TST_HAVE_errno
    TEST_DECL_SUITE(errno)
#endif
#ifdef TST_HAVE_fenv
    TEST_DECL_SUITE(fenv)
#endif
#ifdef TST_HAVE_float
    TEST_DECL_SUITE(float)
#endif
#ifdef TST_HAVE_inttypes
    TEST_DECL_SUITE(inttypes)
#endif
#ifdef TST_HAVE_iso646
    TEST_DECL_SUITE(iso646)
#endif
#ifdef TST_HAVE_limits
    TEST_DECL_SUITE(limits)
#endif
#ifdef TST_HAVE_locale
    TEST_DECL_SUITE(locale)
#endif
#ifdef TST_HAVE_math
    TEST_DECL_SUITE(math)
#endif
#ifdef TST_HAVE_setjmp
    TEST_DECL_SUITE(setjmp)
#endif
#ifdef TST_HAVE_signal
    TEST_DECL_SUITE(signal)
#endif
#ifdef TST_HAVE_stdalign
    TEST_DECL_SUITE(stdalign)
#endif
#ifdef TST_HAVE_stdarg
    TEST_DECL_SUITE(stdarg)
#endif
#ifdef TST_HAVE_stdatomic
    TEST_DECL_SUITE(stdatomic)
#endif
#ifdef TST_HAVE_stdbit
    TEST_DECL_SUITE(stdbit)
#endif
#ifdef TST_HAVE_stdbool
    TEST_DECL_SUITE(stdbool)
#endif
#ifdef TST_HAVE_stdckdint
    TEST_DECL_SUITE(stdckdint)
#endif
#ifdef TST_HAVE_stddef
    TEST_DECL_SUITE(stddef)
#endif
#ifdef TST_HAVE_stdint
    TEST_DECL_SUITE(stdint)
#endif
#ifdef TST_HAVE_stdio
    TEST_DECL_SUITE(stdio)
#endif
#ifdef TST_HAVE_stdnoreturn
    TEST_DECL_SUITE(stdnoreturn)
#endif
#ifdef TST_HAVE_stdlib
    TEST_DECL_SUITE(stdlib)
#endif
#ifdef TST_HAVE_string
    TEST_DECL_SUITE(string)
#endif
#ifdef TST_HAVE_tgmath
    TEST_DECL_SUITE(tgmath)
#endif
#ifdef TST_HAVE_threads
    TEST_DECL_SUITE(threads)
#endif
#ifdef TST_HAVE_time
    TEST_DECL_SUITE(time)
#endif
#ifdef TST_HAVE_uchar
    TEST_DECL_SUITE(uchar)
#endif
#ifdef TST_HAVE_wchar
    TEST_DECL_SUITE(wchar)
#endif
#ifdef TST_HAVE_wctype
    TEST_DECL_SUITE(wctype)
#endif
TEST_DECL_END()

int main(int argc, char** argv) {
    TEST_SET_PASS_LOG_DIR(test_env_enter_sandbox("test/result_c_test", argc, argv));
    {   int rc = (int)TEST_RUN();
        test_env_leave_sandbox();
        return rc;
    }
}
