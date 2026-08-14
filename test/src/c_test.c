/**
 *  @file   c_test.c
 *  @brief  Unit test implementation (C language port of test.hpp).
 *  @author Masashi Kitamura (tenka@6809.net)
 *  @date   2020-2026
 *  @license Boost Software License Version 1.0
 */
#include "c_test.h"
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#if defined(_MSC_VER) && _MSC_VER < 1900
#define vsnprintf _vsnprintf
#endif

static test_case_t* g_suites[TEST_SUITE_MAX];
static char const*  g_suite_names[TEST_SUITE_MAX];
static int          g_suite_count = 0;

static char const*  g_cur_group  = NULL;
static char const*  g_cur_name   = NULL;
static unsigned     g_errors     = 0;
static unsigned     g_check      = 0;
static unsigned     g_skip       = 0;
static int          g_case_skip  = 0;
static unsigned     g_expr_skip  = 0;

static char         g_seg_note[256]   = { 0 };
static char         g_pass_dir[1024]  = { 0 };
static unsigned     g_pass_prev_check = 0;
static unsigned     g_pass_prev_err   = 0;
static unsigned     g_pass_prev_skip  = 0;
static char const*  g_pass_opened[TEST_SUITE_MAX];
static int          g_pass_opened_n   = 0;

void _TeST_printf(char const* fmt, ...) {
    char    buf[TEST_PUTS_BUF_SIZE];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, (sizeof buf) - 1, fmt, ap);
    va_end(ap);
    buf[(sizeof buf) - 1] = 0;
    TEST_PUTS(buf);
}

void _TeST_add(test_case_t* cases, char const* suite_name) {
    if (g_suite_count < TEST_SUITE_MAX) {
        g_suites[g_suite_count]      = cases;
        g_suite_names[g_suite_count] = suite_name;
        ++g_suite_count;
    } else {
        _TeST_printf("[TEST] too many suites (max %d): '%s' ignored.\n",
                     (int)TEST_SUITE_MAX, suite_name);
    }
}

static int _TeST_ent(void) {
    ++g_check;
    if (g_expr_skip) {
        --g_expr_skip;
        ++g_skip;
        return 0;
    }
    return 1;
}

static void _TeST_err(char const* file, int line) {
    ++g_errors;
    _TeST_printf("%s (%d) [%s.%s]: ", file, line, g_cur_group, g_cur_name);
}

void _TeST_true(int cc, char const* expr, char const* file, int line) {
    if (!_TeST_ent())
        return;
    if (!cc) {
        _TeST_err(file, line);
        _TeST_printf("'%s' is failed.\n", expr);
    }
}

static int _TeST_is_mem_addr(void const* p) {
    size_t s  = (size_t)p;
    size_t mi = 0xff;
    size_t ma = ~(size_t)mi;
    return (mi < s && s < ma);
}

void _TeST_ptr(void const* p, int zero_ok, char const* expr, char const* file, int line) {
    if (!_TeST_ent())
        return;
    if (p == NULL) {
        if (!zero_ok) {
            _TeST_err(file, line);
            _TeST_printf("'%s'(%p) is null pointer.\n", expr, p);
        }
    } else if (!_TeST_is_mem_addr(p)) {
        _TeST_err(file, line);
        _TeST_printf("'%s'(%p) is bad pointer.\n", expr, p);
    }
}

void _TeST_str_cc(char const* a, char const* b, int eq, char const* expr,
                  char const* file, int line) {
    int equal;
    if (!_TeST_ent())
        return;
    equal = (strcmp(a ? a : "", b ? b : "") == 0);
    if (equal != eq) {
        _TeST_err(file, line);
        _TeST_printf("'%s' is failed.\n", expr);
    }
}

void _TeST_fail(char const* file, int line) {
    ++g_check;
    _TeST_err(file, line);
    _TeST_printf("test_fail().\n");
}

void _TeST_setExprSkip(void) { ++g_expr_skip; }
void _TeST_setCaseSkip(void) { g_case_skip = 1; }

void _TeST_note(char const* note) {
    if (note && note[0]) {
        size_t i = 0;
        for (; note[i] && i < sizeof(g_seg_note) - 1; ++i)
            g_seg_note[i] = note[i];
        g_seg_note[i] = 0;
    }
}

void _TeST_set_pass_dir(char const* dir) {
    if (dir && dir[0]) {
        size_t i = 0;
        for (; dir[i] && i < sizeof(g_pass_dir) - 1; ++i)
            g_pass_dir[i] = dir[i];
        g_pass_dir[i] = 0;
    } else {
        g_pass_dir[0] = 0;
    }
    g_pass_opened_n = 0;
}

static int _TeST_pass_first(char const* suite) {
    int i;
    for (i = 0; i < g_pass_opened_n; ++i)
        if (strcmp(g_pass_opened[i], suite) == 0)
            return 0;
    if (g_pass_opened_n < TEST_SUITE_MAX)
        g_pass_opened[g_pass_opened_n++] = suite;
    return 1;
}

static void _TeST_pass_write(char const* suite, char const* pass,
                             char const* result, char const* note) {
    char   path[1200];
    FILE*  fp;
    int    first;
    size_t dl, sl;
    if (!g_pass_dir[0] || !suite)
        return;
    dl = strlen(g_pass_dir);
    sl = strlen(suite);
    if (dl + 1 + sl + 5 >= sizeof(path))
        return;
    memcpy(path, g_pass_dir, dl);
    path[dl] = '/';
    memcpy(path + dl + 1, suite, sl);
    strcpy(path + dl + 1 + sl, ".tsv");

    first = _TeST_pass_first(suite);
    fp = fopen(path, first ? "w" : "a");
    if (!fp)
        return;
    if (first)
        fprintf(fp, "\"suite\"\t\"pass\"\t\"result\"\t\"note\"\n");
    fprintf(fp, "\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\n",
            suite, pass, result, note ? note : "");
    fclose(fp);
}

void _TeST_pass(char const* pass_str) {

    g_skip  += g_expr_skip;
    g_check += g_expr_skip;
    g_expr_skip = 0;

    if (pass_str && pass_str[0]) {
        unsigned seg_check = g_check  - g_pass_prev_check;
        unsigned seg_err   = g_errors - g_pass_prev_err;
        unsigned seg_skip  = g_skip   - g_pass_prev_skip;
        unsigned seg_ran   = (seg_check >= seg_skip) ? seg_check - seg_skip : 0;
        unsigned seg_ok    = (seg_ran  >= seg_err)   ? seg_ran  - seg_err   : 0;
        char const* result = seg_err ? "fail" : (seg_ok ? "pass" : "skip");
        _TeST_pass_write(g_cur_group, pass_str, result, g_seg_note);
    }

    g_pass_prev_check = g_check;
    g_pass_prev_err   = g_errors;
    g_pass_prev_skip  = g_skip;
    g_seg_note[0] = 0;
}

#if defined(_MSC_VER)
#include <stdlib.h>
#include <crtdbg.h>
#include <windows.h>

static void __cdecl _TeST_invalid_param(wchar_t const* expr, wchar_t const* func,
                                        wchar_t const* file, unsigned int line,
                                        uintptr_t reserved)
{
    (void)expr; (void)func; (void)file; (void)line; (void)reserved;

}

static void _TeST_silence_dialogs(void) {
    _set_invalid_parameter_handler(_TeST_invalid_param);
#if defined(_DEBUG)
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR,  _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR,  _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_WARN,   _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN,   _CRTDBG_FILE_STDERR);
#endif

    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
}
#else
static void _TeST_silence_dialogs(void) {}
#endif

unsigned _TeST_run(void) {

    extern void _TeST_decl_begin(void);
    unsigned gok = 0, gsz, gerr;
    int      si;

    _TeST_silence_dialogs();

    setvbuf(stdout, (char*)0, _IONBF, 0);

    g_suite_count = 0;
    _TeST_decl_begin();

    _TeST_printf("[TEST]...\n");

    for (si = 0; si < g_suite_count; ++si) {
        test_case_t* cases  = g_suites[si];
        char const*  gname  = g_suite_names[si];
        unsigned     idx, sz;
        unsigned     oks = 0, errors = 0, cskips = 0;

        for (sz = 0; cases[sz].name != NULL; ++sz)
            ;

        for (idx = 0; idx < sz; ++idx) {
            unsigned nok;
            int      rc;

            g_cur_group = gname;
            g_cur_name  = cases[idx].name;
            g_errors    = g_check = g_skip = 0;
            g_case_skip = 0;
            g_expr_skip = 0;
            g_pass_prev_check = g_pass_prev_err = g_pass_prev_skip = 0;
            g_seg_note[0] = 0;

            cases[idx].func();

            g_skip  += g_expr_skip;
            g_check += g_expr_skip;
            g_expr_skip = 0;

            rc = (g_errors == 0);
            if (g_case_skip) {
                ++cskips;
                _TeST_printf("   %s  %s [%d/%d]\t%s\t(case skipped)\n",
                             "skp", gname, idx + 1, sz, cases[idx].name);
                continue;
            }
            oks    += rc;
            errors += !rc;
            nok = (g_check >= g_errors + g_skip)
                ? g_check - g_errors - g_skip : 0;
            _TeST_printf("   %s  %s [%d/%d]\t%s\t(%d = ok%d + err%d + skip%d)\n",
                         (rc) ? " ok" : "NG!", gname, idx + 1, sz,
                         cases[idx].name,
                         g_check, nok, g_errors, g_skip);
        }

        if (cskips) {
            _TeST_printf("\t    %s: %d %s. %d %s. %d skipped.\n", gname,
                         oks, (oks > 1) ? "successes" : "success",
                         errors, (errors > 1) ? "failures" : "failure",
                         cskips);
        } else {
            _TeST_printf("\t    %s: %d %s. %d %s.\n", gname,
                         oks, (oks > 1) ? "successes" : "success",
                         errors, (errors > 1) ? "failures" : "failure");
        }
        gok += (errors == 0);
    }

    gsz  = (unsigned)g_suite_count;
    gerr = gsz - gok;
    _TeST_printf("[Result] Run %d %s, %d %s, %d %s.\n\n",
                 gsz,  (gsz  > 1) ? "test-groups" : "test-group",
                 gok,  (gok  > 1) ? "successes"   : "success",
                 gerr, (gerr > 1) ? "failures"    : "failure");
    return gerr;
}
