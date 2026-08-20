#ifndef TST_TEST_ENV_H
#define TST_TEST_ENV_H

#include <stdio.h>
#include <string.h>

#if defined(_MSC_VER) || defined(__ORANGEC__)
#  include <direct.h>
#  define TST_TEST_MKDIR(d)   _mkdir(d)
#  define TST_TEST_RMDIR(d)   _rmdir(d)
#  define TST_TEST_CHDIR(d)   _chdir(d)
#  define TST_TEST_GETCWD(b,n) _getcwd((b), (n))
#elif defined(_WIN32) && !defined(__CYGWIN__) && (defined(__MINGW32__) || defined(__MINGW64__))

#  include <direct.h>
#  define TST_TEST_MKDIR(d)   _mkdir(d)
#  define TST_TEST_RMDIR(d)   _rmdir(d)
#  define TST_TEST_CHDIR(d)   _chdir(d)
#  define TST_TEST_GETCWD(b,n) _getcwd((b), (n))
#elif defined(__WATCOMC__) || defined(__DMC__)
#  include <direct.h>
#  define TST_TEST_MKDIR(d)   mkdir(d)
#  define TST_TEST_RMDIR(d)   rmdir(d)
#  define TST_TEST_CHDIR(d)   chdir(d)
#  define TST_TEST_GETCWD(b,n) getcwd((b), (n))
#else
#  include <sys/stat.h>
#  include <sys/types.h>
#  include <unistd.h>
#  define TST_TEST_MKDIR(d)   mkdir((d), 0777)
#  define TST_TEST_RMDIR(d)   rmdir(d)
#  define TST_TEST_CHDIR(d)   chdir(d)
#  define TST_TEST_GETCWD(b,n) getcwd((b), (n))
#endif

#if defined(__cplusplus) && defined(TEST_HAS_EH) && !(TEST_HAS_EH)
#  define TST_ENV_HAS_EH    0
#else
#  define TST_ENV_HAS_EH    1
#endif
#if !defined(__cplusplus) || defined(_CPPRTTI) || defined(__GXX_RTTI) || defined(__cpp_rtti)
#  define TST_ENV_HAS_RTTI  1
#else
#  define TST_ENV_HAS_RTTI  0
#endif

static const char* test_env_tag(char* buf) {
    char        comp[32];
    char        tgt[16];
    const char* os  = "";

#if defined(__cplusplus) && defined(TST_STDLIB_LIBCXX)
    const char* lib = "-libcxx";
#else
    const char* lib = "";
#endif
    const char* cpu;
    const char* std_;

#if defined(_MSC_VER)
    {   int v = _MSC_VER, p;
        if      (v >= 1950) p = 145; else if (v >= 1940) p = 144;
        else if (v >= 1930) p = 143; else if (v >= 1920) p = 142;
        else if (v >= 1910) p = 141; else if (v >= 1900) p = 140;
        else if (v >= 1800) p = 120; else if (v >= 1700) p = 110;
        else if (v >= 1600) p = 100; else if (v >= 1500) p = 90;
        else if (v >= 1400) p = 80;  else p = v;
        sprintf(comp, "vc%d", p);
    }
#elif defined(__WATCOMC__)
    sprintf(comp, "watcom%d", (int)__WATCOMC__);
#elif defined(__DMC__)
    sprintf(comp, "dmc%d", (int)__DMC__);
#elif defined(__ORANGEC__)
    sprintf(comp, "orangec%d", (int)__ORANGEC_MAJOR__);
#elif defined(__clang__)
    sprintf(comp, "clang%d", (int)__clang_major__);
#elif defined(__GNUC__)
    sprintf(comp, "gcc%d", (int)__GNUC__);
#else
    strcpy(comp, "cc");
#endif

#if defined(__DOS__)
    os = "-dos";
#elif (defined(__clang__) || defined(__GNUC__)) && !defined(_MSC_VER) && !defined(__WATCOMC__)
# if defined(_WIN32)
    os = "-win";
# elif defined(__APPLE__)
    os = "-mac";
# elif defined(__linux__)
    os = "-linux";
# endif
#endif

#if defined(__aarch64__) || defined(_M_ARM64)
    cpu = (sizeof(void*) == 8) ? "arm64" : "arm";
#else
    cpu = (sizeof(void*) == 8) ? "x64" : "x86";
#endif

#if defined(__cplusplus)
#if !defined(_tst_cplusplus)
 #define _tst_cplusplus  __cplusplus
#endif
# if defined(__WATCOMC__)
    std_ = "cxx03";
# else

    std_ = (_tst_cplusplus >  202302L) ? "cxx26" : (_tst_cplusplus >= 202302L) ? "cxx23"
         : (_tst_cplusplus >= 202002L) ? "cxx20"
         : (_tst_cplusplus >= 201703L) ? "cxx17" : (_tst_cplusplus >= 201402L) ? "cxx14"
         : (_tst_cplusplus >= 201103L) ? "cxx11" : "cxx03";
# endif
#elif defined(__STDC_VERSION__)

    std_ = (__STDC_VERSION__ >= 202311L) ? "c2023" : (__STDC_VERSION__ >= 201710L) ? "c2017"
         : (__STDC_VERSION__ >= 201112L) ? "c2011" : (__STDC_VERSION__ >= 199901L) ? "c1999" : "c1990";
#else
    std_ = "c1990";
#endif

    tgt[0] = 0;
#if defined(__cplusplus) && defined(_CCW_TARGET_CXX)
    sprintf(tgt, "-ccw%02d", (int)((_CCW_TARGET_CXX) % 100));
#elif defined(_CCW_TARGET_C)
    sprintf(tgt, "-ccw%02d", (int)((_CCW_TARGET_C) % 100));
#endif
#if !TST_ENV_HAS_EH && TST_ENV_HAS_RTTI
    sprintf(buf, "%s%s-%s-%s%s%s-xr", comp, os, cpu, std_, lib, tgt);
#elif !TST_ENV_HAS_EH
    sprintf(buf, "%s%s-%s-%s%s%s-noeh", comp, os, cpu, std_, lib, tgt);
#elif !TST_ENV_HAS_RTTI
    sprintf(buf, "%s%s-%s-%s%s%s-xst", comp, os, cpu, std_, lib, tgt);
#elif defined(_TST_ENV_XRXS)
    sprintf(buf, "%s%s-%s-%s%s%s-xrxs", comp, os, cpu, std_, lib, tgt);
#elif defined(_TST_ENV_CCWSTD)
    sprintf(buf, "%s%s-%s-%s%s%s-ccwstd", comp, os, cpu, std_, lib, tgt);
#else
    sprintf(buf, "%s%s-%s-%s%s%s", comp, os, cpu, std_, lib, tgt);
#endif
    return buf;
}

static void test_env_mkdirs(const char* dir) {
    char tmp[512];
    int  i;
    for (i = 0; dir[i] && i < (int)sizeof(tmp) - 1; ++i) {
        if ((dir[i] == '/' || dir[i] == '\\') && i) {
            tmp[i] = 0;
            TST_TEST_MKDIR(tmp);
        }
        tmp[i] = dir[i];
    }
    tmp[i] = 0;
    if (tmp[0])
        TST_TEST_MKDIR(tmp);
}

static int test_env_is_abs_(const char* p) {
    if (!p || !p[0]) return 0;
    if (p[0] == '/' || p[0] == '\\') return 1;
    return (p[1] == ':' && (p[2] == '/' || p[2] == '\\'));
}

static int test_env_dir_exists_(const char* dir) {
    char cur[1024];
    if (!TST_TEST_GETCWD(cur, (int)sizeof(cur)))
        return 0;
    if (TST_TEST_CHDIR(dir) != 0)
        return 0;
    TST_TEST_CHDIR(cur);
    return 1;
}

static const char* test_env_unnest_root_(const char* result_root) {
    static char buf[1024];
    int  up;
    if (test_env_is_abs_(result_root) || test_env_dir_exists_(result_root))
        return result_root;
    for (up = 1; up <= 4; ++up) {
        int i;
        buf[0] = 0;
        for (i = 0; i < up; ++i)
            strcat(buf, "../");
        if ((int)(strlen(buf) + strlen(result_root)) >= (int)sizeof(buf))
            break;
        strcat(buf, result_root);
        if (test_env_dir_exists_(buf)) {
            fprintf(stderr, "[test] not at the repository root -- writing pass logs to %s\n", buf);
            return buf;
        }
    }
    return result_root;
}

static const char* test_env_result_dir(const char* result_root, int argc, char** argv) {
    static char dir[512];
    if (argc > 1 && argv[1] && argv[1][0]) {
        strncpy(dir, argv[1], sizeof(dir) - 1);
        dir[sizeof(dir) - 1] = 0;
    } else {
        char tag[128];
        test_env_tag(tag);
        if (!tag[0]) { dir[0] = 0; return dir; }
        sprintf(dir, "%s/%s", test_env_unnest_root_(result_root), tag);
    }
    test_env_mkdirs(dir);
    return dir;
}

static char test_env_sandbox_home_[1024];
static char test_env_sandbox_dir_[1024];

static const char* test_env_enter_sandbox(const char* result_root, int argc, char** argv) {
    static char abs_[1024];
    const char* dir = test_env_result_dir(result_root, argc, argv);
    test_env_sandbox_dir_[0] = 0;
    test_env_sandbox_home_[0] = 0;
    if (!dir[0])
        return dir;
    if (test_env_is_abs_(dir)) {
        strncpy(abs_, dir, sizeof(abs_) - 1);
        abs_[sizeof(abs_) - 1] = 0;
    } else {
        if (!TST_TEST_GETCWD(test_env_sandbox_home_, (int)sizeof(test_env_sandbox_home_)))
            return dir;
        if ((int)(strlen(test_env_sandbox_home_) + strlen(dir) + 2) > (int)sizeof(abs_))
            return dir;
        sprintf(abs_, "%s/%s", test_env_sandbox_home_, dir);
    }
    if (!test_env_sandbox_home_[0] &&
        !TST_TEST_GETCWD(test_env_sandbox_home_, (int)sizeof(test_env_sandbox_home_)))
        return abs_;
    if ((int)(strlen(abs_) + 5) > (int)sizeof(test_env_sandbox_dir_))
        return abs_;
    sprintf(test_env_sandbox_dir_, "%s/tmp", abs_);
    TST_TEST_MKDIR(test_env_sandbox_dir_);
    if (TST_TEST_CHDIR(test_env_sandbox_dir_) != 0)
        test_env_sandbox_dir_[0] = 0;
    return abs_;
}

static void test_env_leave_sandbox(void) {
    if (test_env_sandbox_home_[0])
        TST_TEST_CHDIR(test_env_sandbox_home_);
    if (test_env_sandbox_dir_[0])
        TST_TEST_RMDIR(test_env_sandbox_dir_);
}

#endif
