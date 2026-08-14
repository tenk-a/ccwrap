#include "test_cxx.hpp"
#include "../src/test_env.h"

#if defined(_MSC_VER)
#include <crtdbg.h>
#include <cstdlib>
#include <cstdio>
#include <cstdint>

extern "C" __declspec(dllimport) unsigned long __stdcall SetErrorMode(unsigned long);

static void __cdecl ccw_invalid_param(wchar_t const*, wchar_t const*, wchar_t const*,
                                      unsigned int, uintptr_t) {}

static void ccw_no_crash_dialogs() {
    const unsigned long SEM_FAILCRITICALERRORS = 0x0001;
    const unsigned long SEM_NOGPFAULTERRORBOX  = 0x0002;
    const unsigned long SEM_NOOPENFILEERRORBOX = 0x8000;
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

    _set_invalid_parameter_handler(ccw_invalid_param);
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);

    for (int i = 0; i < 3; ++i) {
        _CrtSetReportMode(i, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(i, _CRTDBG_FILE_STDERR);
    }

    setvbuf(stdout, (char*)0, _IONBF, 0);
}
#else
static void ccw_no_crash_dialogs() {}
#endif

#if defined(__WATCOMC__)
#include <cstdio>
static void ccw_force_float_runtime() {
    volatile double __d = 1.5; char __b[32];
    STD::sprintf(__b, "%f", __d);
}
#else
static void ccw_force_float_runtime() {}
#endif

int main(int argc, char** argv) {
    ccw_no_crash_dialogs();
    ccw_force_float_runtime();

    TEST_SET_PASS_LOG_DIR(test_env_enter_sandbox("test/result_cxx_test", argc, argv));
    {   int rc = (int)TEST_RUN();
        test_env_leave_sandbox();
        return rc;
    }
}
