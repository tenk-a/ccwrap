cmake_minimum_required(VERSION 3.29)

# CMP0136 (>=3.27) is what makes CMAKE_WATCOM_RUNTIME_LIBRARY effective: under its OLD
# behaviour the Open Watcom platform hardcodes "-br -bm" (the DLL C++ runtime) into the
# base flags and the runtime-library abstraction is ignored. cmake_minimum_required at
# 3.29 already defaults CMP0136 to NEW, so the preset's CMAKE_WATCOM_RUNTIME_LIBRARY
# selects the STATIC runtime. (Set it explicitly too, so the intent survives a lower
# minimum being set later.)
if(POLICY CMP0136)
    cmake_policy(SET CMP0136 NEW)
endif()

project(ccwrap_header_test C CXX)

# ---------------------------------------------------------------------------
# Dedicated build for the per-header suite (test/cxx_test/*_test.cpp).
#
# Self-contained: configure this directory directly, for any of the three targets.
#
#   cmake --preset watcom && cmake --build --preset watcom && ctest --preset watcom
#   cmake --preset vc145  && cmake --build --preset vc145  && ctest --preset vc145
#   cmake --preset gcc    && cmake --build --preset gcc    && ctest --preset gcc
#
# 4 スイートをまとめて回すなら test/ の umbrella (test/CMakePresets.json) を使う。
# コンパイラ別の配線は test/bin/ccwrap_test.cmake に集約してある。
#
# Why a separate CMake here (not run_watcom.bat's hand-rolled link): the suite has
# 150+ units. Listing every .obj on one wcl386/wlink command line overflows cmd.exe's
# ~8 KB limit and the tail is *silently dropped* -- the run then reports "all green"
# while 70+ groups never linked in. CMake drives the linker through a response file,
# which has no such limit, so every group actually runs. It also probes each file on
# its own (a header that stops compiling is listed, not allowed to sink the suite).
#
# Single-file / subset workflow (the reason this lives in its own folder): pass the
# header base name(s) via -DTST_ONLY to build just those, skipping the full probe:
#
#   cmake --preset watcom -DTST_ONLY=locale
#   cmake --build --preset watcom && ctest --preset watcom
#   cmake --preset watcom -DTST_ONLY=vector;string        # a ;-separated subset
#
# The aggregate main (test_cxx_vc.cpp) and, on Watcom, the prebuilt libcxx03 library
# are always linked in, so a one-file build still runs end to end.
# ---------------------------------------------------------------------------

include("${CMAKE_CURRENT_LIST_DIR}/../bin/ccwrap_test.cmake")

# ABSOLUTE, normalised. The un-normalised form ("…/test/c_test/../..") is handed to
# the test as its working directory, and Open Watcom bug A24 (wide swprintf) flips
# on exactly that -- the same binary is green run by hand and red under ctest.
get_filename_component(CCWRAP_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)

option(TST_PROBE "Probe each test file and drop the ones that do not compile" ON)
# **既定は素のコンパイラ (ccwrap を通さない)**。値を入れたときだけ _CCW_TARGET_CXX が
# 定義され、ccwrap 経由になる (= pass ログの tag に -ccw<n> が付くビルド)。
set(TEST_TARGET_CXX "" CACHE STRING "_CCW_TARGET_CXX: the feature level ccwrap should expose. EMPTY (default) = do not use ccwrap at all.")
set(TST_ONLY "" CACHE STRING "Build only these cxx_test files (base names w/o _test.cpp, ;-separated). Empty = all.")
set(TST_NS "std" CACHE STRING "Namespace environment on Open Watcom: 'std' (default) or 'ccwstd' (libcxx03 relocated; forces watcom/ccwstd/ + the ccwstd prebuilt lib). Tests spell STD:: so both work.")
set(TST_MSVC_STD "c++latest" CACHE STRING "MSVC only: the /std: level (c++14 / c++17 / c++20 / c++latest)")
# clang only: which standard library the preset asked for. The flag itself
# (-stdlib=libc++) comes from CMAKE_CXX_FLAGS; this just tells the tests, so the
# pass-log tag says -libcxx and libstdc++ results are not overwritten.
set(TST_STDLIB "" CACHE STRING "clang only: 'libcxx' when the preset passes -stdlib=libc++. Empty = the compiler's default library.")

enable_testing()

# ---------------------------------------------------------------------------
# per-compiler routing (include dirs / options / defs), shared by probe + target
# ---------------------------------------------------------------------------
# C_CONFORM: c_test と同じく**準拠モード**で回す。vc は既定だと vc 間互換を優先して
# native の非準拠な挙動を残す (例: vc120 の <float.h> は DECIMAL_DIG を 10 = float の値に
# 誤定義しており、ccwrap はこの opt-in のときだけ 17 に直す)。テストは規格の側を見たい.
ccwrap_test_routing(LANG CXX LEVEL "${TEST_TARGET_CXX}" NS "${TST_NS}" MSVC_STD "${TST_MSVC_STD}" C_CONFORM
                    OUT_INCS TST_INCS OUT_OPTS TST_OPTS OUT_DEFS TST_DEFS)

# <filesystem> の path::value_type は既定でターゲット追従 (Windows=wchar_t / DOS=char)。
# これは char 側へ倒す opt-out。ABI が変わるのでプリビルドライブラリも -fs-char 版と
# 対で選ぶ (下の TST_LIB_VARIANT)。
option(TEST_FS_CHAR "filesystem: path::value_type を char に固定する (_CCW_FS_WCHAR=0)" OFF)
if(TEST_FS_CHAR)
    list(APPEND TST_DEFS _CCW_FS_WCHAR=0)
    message(STATUS "cxx_test: filesystem path::value_type = char (_CCW_FS_WCHAR=0)")
endif()
if(TEST_TARGET_CXX)
    message(STATUS "cxx_test: routed through ccwrap (_CCW_TARGET_CXX=${TEST_TARGET_CXX}, ns=${TST_NS})")
else()
    # Open Watcom の純正 C++ ライブラリはハーネスすら満たせない (<string_view> が無く、
    # <sstream> は std::ostringstream を持たない) ので、素の構成は事実上 vc/gcc 用。
    message(STATUS "cxx_test: the compiler's own standard library only (no ccwrap)")
endif()
# cxx_test/ itself is on the include path so sources in subdirs (libcxx03_test/) still
# find the shared "test_cxx.hpp".
list(INSERT TST_INCS 0 "${CMAKE_CURRENT_LIST_DIR}")

# ---------------------------------------------------------------------------
# select the test files (+ probe: which of them compile here?)
# ---------------------------------------------------------------------------
# libcxx03_test/ holds suites that depend on libcxx03 internals (__tree_rb_valid etc.);
# they only build on a libcxx03 target (Open Watcom).
set(TST_DIRS "${CMAKE_CURRENT_LIST_DIR}")
ccwrap_test_compiler(TST_CC)
if(TST_CC STREQUAL "watcom")
    list(APPEND TST_DIRS "${CMAKE_CURRENT_LIST_DIR}/libcxx03_test")
endif()

ccwrap_test_select(EXT ".cpp" DIRS ${TST_DIRS} ONLY "${TST_ONLY}"
                   OUT_SRC TST_HTEST_ALL OUT_PROBE TST_DO_PROBE)
if(TST_ONLY)
    message(STATUS "cxx_test: building only: ${TST_ONLY}")
endif()
if(NOT TST_PROBE)
    set(TST_DO_PROBE OFF)
endif()

if(NOT TST_HTEST_ALL)
    # umbrella で TST_ONLY が他スイートの名前だけを指していた場合。畳んで終わる。
    message(STATUS "cxx_test: nothing selected -- skipped")
    return()
endif()

if(TST_DO_PROBE)
    ccwrap_test_probe(SOURCES ${TST_HTEST_ALL} INCS "${TST_INCS}" OPTS "${TST_OPTS}"
                      DEFS "${TST_DEFS}" PREFIX TST_COMPILES LABEL "cxx_test"
                      OUT_SRC TST_HTEST_SRC REQUIRE_ANY)
else()
    set(TST_HTEST_SRC ${TST_HTEST_ALL})
endif()

# Files that COMPILE but fail to LINK against the native library (not a ccwrap issue), so
# the compile-only probe above cannot catch them:
#   codecvt -- libstdc++ never emits std::codecvt_utf8's out-of-line destructor (a known
#   libstdc++ gap), leaving an unresolved reference at link time. Covered on vc/watcom
#   instead. Without this the whole gcc/clang link fails, taking every other group with it.
#   libc++ DOES emit it, so the clang -stdlib=libc++ presets keep the file in -- that
#   configuration is precisely what covers this gap on the gcc/clang side.
# (run_gcc.sh has the same list -- keep them in sync.)
set(TST_LINK_EXCLUDE_NAMES "")
if(TST_CC STREQUAL "gnu" AND NOT TST_STDLIB STREQUAL "libcxx")
    list(APPEND TST_LINK_EXCLUDE_NAMES codecvt)
elseif(TST_CC STREQUAL "msvc" AND MSVC_VERSION GREATER_EQUAL 1910 AND MSVC_VERSION LESS 1920)
    # codecvt: the VS2017 import library has no codecvt<char16_t>::id /
    # codecvt<char32_t>::id, so the NATIVE codecvt does not link either. Nothing ccwrap
    # can supply. Keyed on the TOOLSET, not on /std: measured on 14.16.27023 it is
    # missing at c++14 / c++17 / c++latest alike, while vc120 / vc140 / vc142 / vc143 /
    # vc145 all resolve it.
    # (<filesystem> used to be here too -- detail/llibcxx03/src/filesystem.cpp now builds on
    # MSVC and is linked in below, so the backport resolves.)
    list(APPEND TST_LINK_EXCLUDE_NAMES codecvt)
endif()
if(TST_LINK_EXCLUDE_NAMES)
    set(TST_LINK_EXCLUDE "")
    set(TST_LINK_EXCLUDED "")
    foreach(src IN LISTS TST_HTEST_SRC)
        get_filename_component(n "${src}" NAME_WE)
        string(REGEX REPLACE "_test$" "" b "${n}")
        list(FIND TST_LINK_EXCLUDE_NAMES "${b}" idx)
        if(NOT idx EQUAL -1)
            list(APPEND TST_LINK_EXCLUDE "${src}")
            list(APPEND TST_LINK_EXCLUDED "${b}")
        endif()
    endforeach()
    if(TST_LINK_EXCLUDE)
        list(REMOVE_ITEM TST_HTEST_SRC ${TST_LINK_EXCLUDE})
        string(REPLACE ";" " " _e "${TST_LINK_EXCLUDED}")
        message(STATUS "cxx_test: excluded (compiles but does not link here): ${_e}")
    endif()
endif()

# ---------------------------------------------------------------------------
# the suite
# ---------------------------------------------------------------------------
# On Open Watcom the libcxx03 out-of-line definitions (thread/locale/filesystem) come from
# the PREBUILT namespace-variant static library, not by compiling the src into every suite.
# Build it first: `cmake --preset watcom` in ../../../watcom/lib
#   (-> watcom/lib/win32-<ns>[-fs-char][-<opts>]/libccwcxx.lib。<opts> は無印 / -xrxs / -xst / -noeh).
set(TST_EXTRA_SRC "")
if(TST_CC STREQUAL "watcom" AND TEST_TARGET_CXX)
    # RTTI の有無でライブラリを分けてある。-xr でビルドしたテストに非 RTTI の
    # libccwcxx.lib を混ぜるのは想定外なので、必ず対で選ぶ。
    # DOS には ccwstd 版が無い (ccwrap が C++ ライブラリの全部なので分ける意味が無い)。
    if(CMAKE_SYSTEM_NAME STREQUAL "DOS")
        set(TST_LIB_VARIANT "dos32-std")
    else()
        set(TST_LIB_VARIANT "win32-${TST_NS}")
    endif()

    # path::value_type が変わると filesystem.cpp の OS 層も別物 (narrow CRT / _w* CRT)
    # になり、path のレイアウトも変わる。名前修飾は同じままなので、取り違えると
    # 黙ってリンクが通って壊れる。RTTI と同じく必ず対で選ぶ。
    # (DOS は既定が narrow なので -fs-char 版は無い。)
    if(TEST_FS_CHAR AND NOT CMAKE_SYSTEM_NAME STREQUAL "DOS")
        set(TST_LIB_VARIANT "${TST_LIB_VARIANT}-fs-char")
    endif()
    # LFN も同じ理由で対にする。__WATCOM_LFN__ は _MAX_PATH / NAME_MAX を広げるので
    # struct _finddata_t の大きさが変わり、filesystem.cpp はそれを _findfirst へ渡す。
    if(TEST_WATCOM_LFN AND CMAKE_SYSTEM_NAME STREQUAL "DOS")
        set(TST_LIB_VARIANT "${TST_LIB_VARIANT}-lfn")
    endif()
    # 接尾辞は「推奨の既定 (-xst -xr) から何を変えたか」。
    # (watcom/lib/CMakeLists.txt と同じ規則)
    if(TEST_NO_EH AND TEST_WATCOM_RTTI)
        set(TST_LIB_VARIANT "${TST_LIB_VARIANT}-xr")
    elseif(TEST_NO_EH)
        set(TST_LIB_VARIANT "${TST_LIB_VARIANT}-noeh")
    elseif(NOT TEST_WATCOM_RTTI)
        set(TST_LIB_VARIANT "${TST_LIB_VARIANT}-xst")
    elseif(TEST_WATCOM_XS)
        set(TST_LIB_VARIANT "${TST_LIB_VARIANT}-xrxs")
    endif()
    set(TST_CCWRAP_LIB "${CCWRAP_ROOT}/watcom/lib/${TST_LIB_VARIANT}/libccwcxx.lib")
    if(NOT EXISTS "${TST_CCWRAP_LIB}")
        if(TEST_NO_EH AND TEST_WATCOM_RTTI)
            # -xr (RTTI 有り + 例外無し) は誰も作っていない組み合わせ。preset で
            # 揃えるのが正しく、その 1 本のために全 variant を増やす価値が無い。
            message(FATAL_ERROR "TEST_NO_EH=ON + TEST_WATCOM_RTTI=ON (-xr) は prebuilt を用意していない -- preset `watcom-noeh` (例外も RTTI も無し) を使うか、TEST_WATCOM_RTTI=OFF を渡すこと")
        endif()
        message(FATAL_ERROR "prebuilt ${TST_CCWRAP_LIB} missing -- run `gen` in ${CCWRAP_ROOT}/watcom/lib (that builds <os>-std and <os>-std-noeh; ccwstd / -fs-char / -xst are opt-in: `gen full`; -xrxs only from `gen xrxs`)")
    endif()
elseif(TEST_TARGET_CXX AND (TST_CC STREQUAL "msvc" OR TST_CC STREQUAL "gnu"))
    # vc / gcc には名前空間別のプリビルドライブラリがまだ無いので、
    # out-of-line 分はターゲットに直接足す。
    #
    # filesystem.cpp は **std 環境でも要る**: native <filesystem> は C++17 かつ
    # vc141 15.7 以降なので、それ未満 (vc8..vc120 と C++14 モード) は vc/filesystem が
    # libcxx03 の backport に落ち、その OS 層がここにしかない。ヘッダ側は通るので
    # probe では落ちず、**リンクだけ** 33 個の未解決で落ちていた。
    # **gcc も同じ** — C++17 未満で回すと native <filesystem> は空なので backport に落ちる。
    # このファイルは自分でゲートする (`_CCW_LIBCPP_FILESYSTEM` が無ければ空) ので、
    # native filesystem を使う構成に足しても無害。
    set(TST_CCWRAP_LIB "")
    list(APPEND TST_EXTRA_SRC "${CCWRAP_ROOT}/detail/llibcxx03/src/filesystem.cpp")
    if(TST_NS STREQUAL "ccwstd")
        # locale.cpp / thread.cpp は「libcxx03 のヘッダを前に置いてコンパイルした」
        # 構成でしか成立しない (native の locale::id を定義しに行ってしまう)。
        foreach(s thread locale)
            list(APPEND TST_EXTRA_SRC "${CCWRAP_ROOT}/detail/llibcxx03/src/${s}.cpp")
        endforeach()
    endif()
else()
    set(TST_CCWRAP_LIB "")
endif()

add_executable(ccw_htest "${CMAKE_CURRENT_LIST_DIR}/test_cxx_vc.cpp" ${TST_HTEST_SRC} ${TST_EXTRA_SRC})
target_include_directories(ccw_htest PRIVATE ${TST_INCS})
target_compile_options(ccw_htest PRIVATE ${TST_OPTS})
target_compile_definitions(ccw_htest PRIVATE ${TST_DEFS})
ccwrap_test_link_libs(ccw_htest LANG CXX)     # gcc/clang: threads / -latomic / -lm
ccwrap_test_utf8_acp(ccw_htest)             # msvc: プロセスの ACP を UTF-8 にする

# <stacktrace> は EBP チェーンを辿るので -of+ が要るが、スイート全体に付けると
# A1'' を踏んで落ちる。この 1 ファイルだけに付ける。
if(TST_CC STREQUAL "watcom")
    set(TST_ST_SRC "${CMAKE_CURRENT_LIST_DIR}/stacktrace_test.cpp")
    if(EXISTS "${TST_ST_SRC}")
        set_source_files_properties("${TST_ST_SRC}" PROPERTIES COMPILE_OPTIONS "-of+")
    endif()
endif()
if(TST_CCWRAP_LIB)
    target_link_libraries(ccw_htest PRIVATE "${TST_CCWRAP_LIB}")
endif()

# ccw_htest exits with the number of failed groups (0 = all good), so ctest sees it.
# Run from the repo root so the default pass-log dir (test/result_cxx_test/<tag>) lands there.
add_test(NAME htest COMMAND ccw_htest WORKING_DIRECTORY "${CCWRAP_ROOT}")

# The TEST_SKIP1() count in an #else must match the checks it stands in for; nothing at
# run time can notice a mismatch, so it is a separate static check (registered once).
ccwrap_test_skip_counts()
