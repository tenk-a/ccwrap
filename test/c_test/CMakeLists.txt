cmake_minimum_required(VERSION 3.21)

# CMP0136 (>=3.27): makes CMAKE_WATCOM_RUNTIME_LIBRARY effective (static runtime).
if(POLICY CMP0136)
    cmake_policy(SET CMP0136 NEW)
endif()
# CMP0091 (>=3.15): abstract MSVC runtime selection so the STATIC CRT can be forced.
# Old vc (vc80..vc100) link the CRT as a side-by-side assembly DLL (e.g. msvcr90d.dll)
# that is not present in WinSxS, so a dynamically-linked exe fails to load at startup
# (0xC0000135 DLL-not-found). Static linking makes every vc c_test exe self-contained.
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

project(ccwrap_c_test C)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# ---------------------------------------------------------------------------
# Per-header tests for the ccwrap wrapper *C* headers (<string.h>, <ctype.h>, ...),
# driven from C (not C++). The counterpart of test/cxx_test/ but one language down:
# each <header>_test.c exercises the C header directly with test/src/c_test.h's C
# TEST_CASE/test_eq framework. Each file is modelled on the matching cxx_test/
# c<header>_test.cpp (the rich C++ suite), keeping the observable-behaviour checks
# and dropping the C++-only ones (overload/type-trait/decltype).
#
#   cmake --preset watcom && cmake --build --preset watcom && ctest --preset watcom
#   cmake --preset vc145  && cmake --build --preset vc145  && ctest --preset vc145
#   cmake --preset gcc    && cmake --build --preset gcc    && ctest --preset gcc
#   cmake --preset dmc    && cmake --build --preset dmc    && ctest --preset dmc
#   cmake --preset orangec && cmake --build --preset orangec && ctest --preset orangec
#
# 4 スイートをまとめて回すなら test/ の umbrella (test/CMakePresets.json) を使う。
# コンパイラ別の配線は test/bin/ccwrap_test.cmake に集約してある。
#
# The C framework registers suites by hand (test_main.c lists every suite guarded by
# #ifdef TST_HAVE_<suite>), so we hand it one -DTST_HAVE_<suite> per test file that
# compiled here -- the suite name is the file's base name with the trailing _test
# removed (so <header>_test.c MUST spell TEST_SUITE(<header>)).
#
# Single-file / subset: -DTST_ONLY=<header>[;<header>...] builds just those, no probe.
#   cmake --preset watcom -DTST_ONLY=string
# ---------------------------------------------------------------------------

include("${CMAKE_CURRENT_LIST_DIR}/../bin/ccwrap_test.cmake")

# ABSOLUTE, normalised. The un-normalised form ("…/test/c_test/../..") is handed to
# the test as its working directory, and Open Watcom bug A24 (wide swprintf) flips
# on exactly that -- the same binary is green run by hand and red under ctest.
get_filename_component(CCWRAP_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)

option(TST_PROBE "Probe each test file and drop the ones that do not compile" ON)
# **既定は素のコンパイラ (ccwrap を通さない)**。値を入れたときだけ _CCW_TARGET_C が
# 定義され、ccwrap 経由になる (= pass ログの tag に -ccw<n> が付くビルド)。
set(TEST_TARGET_C "" CACHE STRING "_CCW_TARGET_C: the C feature level ccwrap should expose. EMPTY (default) = do not use ccwrap at all.")
set(TST_ONLY "" CACHE STRING "Build only these <header>_test.c (base names w/o _test.c, ;-separated). Empty = all.")

enable_testing()

# ---------------------------------------------------------------------------
# per-compiler routing (include dirs / options / defs), shared by probe + target
# ---------------------------------------------------------------------------
# TEST_TARGET_C が空 = 強制 include も ccwrap の include パスも _CCW_TARGET_C も付けない
# = **素のコンパイラの C ライブラリだけ**で回す。そこで落ちるものが「ccwrap が補って
# いる分」そのもので、逆にそこで通るものはテスト側の妥当性の裏づけになる。
# (c_test.h は元から ccwrap 非依存で、TEST_TARGET_C を __STDC_VERSION__ から導く。)
ccwrap_test_routing(LANG C LEVEL "${TEST_TARGET_C}" C_CONFORM
                    OUT_INCS TST_INCS OUT_OPTS TST_OPTS OUT_DEFS TST_DEFS)
if(TEST_TARGET_C)
    message(STATUS "c_test: routed through ccwrap (_CCW_TARGET_C=${TEST_TARGET_C})")
else()
    message(STATUS "c_test: the compiler's own C library only (no ccwrap)")
endif()
list(INSERT TST_INCS 0 "${CCWRAP_ROOT}/test/src")   # test/src: c_test.h + test_env.h

# Annex K (the _s functions and errno_t / rsize_t) is only DECLARED when the user asks
# for it before including anything -- a library that advertises __STDC_LIB_EXT1__ still
# hides the names otherwise (K.3.1.1). Open Watcom advertises 200509L, so without this
# the Annex K rows read "the library has it" and then fail to compile.
list(APPEND TST_DEFS __STDC_WANT_LIB_EXT1__=1)

# ---------------------------------------------------------------------------
# select the test files (+ probe: which of them compile here?)
# ---------------------------------------------------------------------------
ccwrap_test_select(EXT ".c" DIRS "${CMAKE_CURRENT_LIST_DIR}" ONLY "${TST_ONLY}"
                   OUT_SRC TST_CTEST_ALL OUT_PROBE TST_DO_PROBE)
if(TST_ONLY)
    message(STATUS "c_test: building only: ${TST_ONLY}")
endif()
if(NOT TST_PROBE)
    set(TST_DO_PROBE OFF)
endif()

if(NOT TST_CTEST_ALL)
    # umbrella で TST_ONLY が他スイートの名前だけを指していた場合。畳んで終わる。
    message(STATUS "c_test: nothing selected -- skipped")
    return()
endif()

if(TST_DO_PROBE)
    ccwrap_test_probe(SOURCES ${TST_CTEST_ALL} INCS "${TST_INCS}" OPTS "${TST_OPTS}"
                      DEFS "${TST_DEFS}" PREFIX TST_C_COMPILES LABEL "c_test"
                      OUT_SRC TST_CTEST_SRC REQUIRE_ANY)
else()
    set(TST_CTEST_SRC ${TST_CTEST_ALL})
endif()

# ---------------------------------------------------------------------------
# select the suites for the committed aggregating main (test_main.c)
# ---------------------------------------------------------------------------
set(TST_MAIN "${CMAKE_CURRENT_LIST_DIR}/test_main.c")
set(TST_HAVE_DEFS "")
foreach(src IN LISTS TST_CTEST_SRC)
    get_filename_component(name "${src}" NAME_WE)
    string(REGEX REPLACE "_test$" "" suite "${name}")
    list(APPEND TST_HAVE_DEFS "TST_HAVE_${suite}")
endforeach()

# ---------------------------------------------------------------------------
# the suite executable
# ---------------------------------------------------------------------------
add_executable(ccw_ctest "${CCWRAP_ROOT}/test/src/c_test.c" "${TST_MAIN}" ${TST_CTEST_SRC})
target_include_directories(ccw_ctest PRIVATE ${TST_INCS})
target_compile_options(ccw_ctest PRIVATE ${TST_OPTS})
target_compile_definitions(ccw_ctest PRIVATE ${TST_DEFS} ${TST_HAVE_DEFS})
ccwrap_test_link_libs(ccw_ctest LANG C)      # gcc/clang: -lm / threads / -latomic
ccwrap_test_utf8_acp(ccw_ctest)             # msvc: プロセスの ACP を UTF-8 にする

# ccw_ctest exits with the number of failed groups (0 = all good), so ctest sees it.
# Run from the repo root so the default pass-log dir (test/result_c_test/<tag>) lands there.
add_test(NAME ctest COMMAND ccw_ctest WORKING_DIRECTORY "${CCWRAP_ROOT}")

# The TEST_SKIP1() count in an #else must match the checks it stands in for; nothing at
# run time can notice a mismatch, so it is a separate static check (registered once).
ccwrap_test_skip_counts()
