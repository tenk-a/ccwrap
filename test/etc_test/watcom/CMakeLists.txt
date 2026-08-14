cmake_minimum_required(VERSION 3.29)

# CMP0136 (>=3.27): makes CMAKE_WATCOM_RUNTIME_LIBRARY effective (static runtime).
# 既定の DLL ランタイムだと Watcom 純正 C++ ライブラリが実行時にリンクされ、
# libcxx03 が置き換えるべき実装がそちらで満たされてしまう。
if(POLICY CMP0136)
    cmake_policy(SET CMP0136 NEW)
endif()

project(ccwrap_etc_test_watcom C CXX)

# ---------------------------------------------------------------------------
# Open Watcom 用の「ヘッダ別でない」スモークテスト (etc_test)。
#
#   cmake --preset watcom && cmake --build --preset watcom && ctest --preset watcom
#
#   watcom_ctest     C スモーク (Win32)
#   watcom_win32_coexist  Win32 の本物の宣言と同居できるか (C。2 は逆の include 順)
#   watcom_win32_coexist_cxx  同じことを C++ 側 (<mutex> 等) で
#   watcom_cinclude  全 C ヘッダが **C から** include できること
#   watcom_cmath     C99 の f/l 数学関数が C から呼べること (<cmath> だけでなく)
#   watcom_c99       C99 限定の面 (<fenv.h>)。native ヘッダの都合で -za99 が要る
#   watcom_stdbit    <stdbit.h> (C23) の型総称マクロを C から
#   intrin           watcom/intrin.h の intrinsic。**C++ のフレームワークテスト**なので
#                    cxx_test の集約 main と libcxx03 を一緒にリンクする
#   intrin_noeh      同じものを例外・RTTI 無しで (codegen が変わりうるので両方見る)
#   watcom_hdr_leak  <algorithm> が <locale> を引き込まないこと (コンパイルが検査そのもの)
#   bugchk           ccwrap_watcom_bugs.md のバグがまだ生きているか (watcom_bugchk/)
#   eh_sweep         全ヘッダが -xs 無しでも通るか (eh_sweep/)
#
# 個別ビルドは他スイートと同じ knob:  -DTST_ONLY=watcom_c99
# ---------------------------------------------------------------------------

include("${CMAKE_CURRENT_LIST_DIR}/../../bin/ccwrap_test.cmake")

ccwrap_test_compiler(TST_CC)
if(NOT TST_CC STREQUAL "watcom")
    message(FATAL_ERROR "etc_test/watcom needs Open Watcom (wcl386). Use the watcom preset.")
endif()

set(CCWRAP_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../..")

# etc_test / compile_fail は **ccwrap そのもののテスト**なので、ここだけは既定で
# ccwrap を通す (値が空だとテスト内容が成立しない)。ヘッダ別スイート (c_test /
# cxx_test) は逆に既定が「素のコンパイラ」で、値を渡したときだけ ccwrap になる。
set(TEST_TARGET_C "2026" CACHE STRING "_CCW_TARGET_C: the C feature level the ccwrap backports expose")
set(TEST_TARGET_CXX "2026" CACHE STRING "_CCW_TARGET_CXX: the C++ feature level the ccwrap backports expose")
set(TST_NS "std" CACHE STRING "Namespace environment: 'std' (default) or 'ccwstd' (libcxx03 relocated)")
set(TST_ONLY "" CACHE STRING "Build only these etc_test programs (watcom_ctest;watcom_cinclude;...). Empty = all.")

enable_testing()

ccwrap_test_routing(LANG C   LEVEL "${TEST_TARGET_C}" NS "${TST_NS}"
                    OUT_INCS TST_C_INCS OUT_OPTS TST_C_OPTS OUT_DEFS TST_C_DEFS)
ccwrap_test_routing(LANG CXX LEVEL "${TEST_TARGET_CXX}" NS "${TST_NS}"
                    OUT_INCS TST_CXX_INCS OUT_OPTS TST_CXX_OPTS OUT_DEFS TST_CXX_DEFS)

# 例外モデル (-xs / -xst) にあわせて prebuilt の variant を選ぶ。無印は -xst、
# -xrxs が -xs。watcom/lib/CMakeLists.txt と同じ規則。
set(TST_LIB_SFX "")
if(TEST_WATCOM_XS)
    set(TST_LIB_SFX "-xrxs")
endif()

# ---------------------------------------------------------------------------
# C スモーク: 1 ファイル = 1 実行ファイル
# ---------------------------------------------------------------------------
set(TST_ADDED "")

function(ccwrap_etc_wat_add_c name src)
    cmake_parse_arguments(A "" "" "OPTS" ${ARGN})
    if(TST_ONLY)
        list(FIND TST_ONLY "${name}" idx)
        if(idx EQUAL -1)
            return()
        endif()
    endif()
    add_executable(${name} "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
    target_include_directories(${name} PRIVATE ${TST_C_INCS})
    target_compile_options(${name} PRIVATE ${TST_C_OPTS} ${A_OPTS})
    target_compile_definitions(${name} PRIVATE ${TST_C_DEFS})
    add_test(NAME ${name} COMMAND ${name} WORKING_DIRECTORY "${CCWRAP_ROOT}")
    set(added ${TST_ADDED})
    list(APPEND added "${name}")
    set(TST_ADDED "${added}" PARENT_SCOPE)
endfunction()

ccwrap_etc_wat_add_c(watcom_ctest    watcom_ctest.c)
ccwrap_etc_wat_add_c(watcom_cinclude watcom_cinclude_test.c)
ccwrap_etc_wat_add_c(watcom_cmath    watcom_cmath_test.c)
ccwrap_etc_wat_add_c(watcom_stdbit   watcom_stdbit_test.c)
# ccwrap の C ヘッダが Win32 API を自前の構造体で宣言していないことの検査。
# 宣言していると本物のプロトタイプと衝突し、Win32 を使うプログラムが通らない。
ccwrap_etc_wat_add_c(watcom_win32_coexist watcom_win32_coexist_test.c)
ccwrap_etc_wat_add_c(watcom_win32_coexist2 watcom_win32_coexist2_test.c)
# -za99: native <fenv.h> は本体を __STDC_VERSION__ >= 199901L で囲っており、既定の C 方言
# (199409L) では中身が丸ごと消える (ccwrap_watcom_bugs.md C2')。ccwrap は偽装せず
# ユーザーに -za99 を促す方針なので、この 1 本だけ C99 モードで叩く。
ccwrap_etc_wat_add_c(watcom_c99      watcom_c99_test.c OPTS -za99)

# ---------------------------------------------------------------------------
# ヘッダ別スイートでは踏めない C++ の単独 TU。test_cxx.hpp が既に <locale> を
# 露出させているので、「<algorithm> だけでどこまで見えるか」は cxx_test では検査できない。
# ---------------------------------------------------------------------------
function(ccwrap_etc_wat_add_cxx name src)
    if(TST_ONLY)
        list(FIND TST_ONLY "${name}" idx)
        if(idx EQUAL -1)
            return()
        endif()
    endif()
    set(lib "${CCWRAP_ROOT}/watcom/lib/win32-${TST_NS}${TST_LIB_SFX}/libccwcxx.lib")
    if(NOT EXISTS "${lib}")
        message(FATAL_ERROR "prebuilt ${lib} missing -- run `gen` in ${CCWRAP_ROOT}/watcom/lib")
    endif()
    add_executable(${name} "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
    target_include_directories(${name} PRIVATE ${TST_CXX_INCS})
    target_compile_options(${name} PRIVATE ${TST_CXX_OPTS})
    target_compile_definitions(${name} PRIVATE ${TST_CXX_DEFS})
    target_link_libraries(${name} PRIVATE "${lib}")
    add_test(NAME ${name} COMMAND ${name} WORKING_DIRECTORY "${CCWRAP_ROOT}")
    set(added ${TST_ADDED})
    list(APPEND added "${name}")
    set(TST_ADDED "${added}" PARENT_SCOPE)
endfunction()

ccwrap_etc_wat_add_cxx(watcom_hdr_leak watcom_hdr_leak_test.cpp)
ccwrap_etc_wat_add_cxx(watcom_win32_coexist_cxx watcom_win32_coexist_cxx_test.cpp)

# watcom_eh_catch_only: C10. Needs a second TU -- see doc/src/test/etc_test/watcom/.
if(NOT TST_ONLY OR "watcom_eh_catch_only" IN_LIST TST_ONLY)
    set(_ehlib "${CCWRAP_ROOT}/watcom/lib/win32-${TST_NS}${TST_LIB_SFX}/libccwcxx.lib")
    add_executable(watcom_eh_catch_only
        "${CMAKE_CURRENT_SOURCE_DIR}/watcom_eh_catch_only_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/watcom_eh_catch_only_aux.cpp")
    target_include_directories(watcom_eh_catch_only PRIVATE ${TST_CXX_INCS})
    target_compile_options(watcom_eh_catch_only PRIVATE ${TST_CXX_OPTS})
    target_compile_definitions(watcom_eh_catch_only PRIVATE ${TST_CXX_DEFS})
    target_link_libraries(watcom_eh_catch_only PRIVATE "${_ehlib}")
    add_test(NAME watcom_eh_catch_only COMMAND watcom_eh_catch_only
             WORKING_DIRECTORY "${CCWRAP_ROOT}")
    list(APPEND TST_ADDED "watcom_eh_catch_only")
endif()

# ccw_check: _CCW_CFG_CHECK が壊れた不変条件で本当に abort するか。3 通りとも
# **落ちるのが合格**なので WILL_FAIL。落ちなくなったら検出器かレイアウトが壊れている。
if(NOT TST_ONLY OR "ccw_check" IN_LIST TST_ONLY)
    set(lib "${CCWRAP_ROOT}/watcom/lib/win32-${TST_NS}${TST_LIB_SFX}/libccwcxx.lib")
    add_executable(ccw_check "${CMAKE_CURRENT_SOURCE_DIR}/ccw_check_test.cpp")
    target_include_directories(ccw_check PRIVATE ${TST_CXX_INCS})
    target_compile_options(ccw_check PRIVATE ${TST_CXX_OPTS})
    target_compile_definitions(ccw_check PRIVATE ${TST_CXX_DEFS} _CCW_CFG_CHECK=1)
    target_link_libraries(ccw_check PRIVATE "${lib}")
    foreach(n 1 2 3)
        add_test(NAME ccw_check_${n} COMMAND ccw_check ${n} WORKING_DIRECTORY "${CCWRAP_ROOT}")
        set_tests_properties(ccw_check_${n} PROPERTIES WILL_FAIL TRUE)
    endforeach()
    list(APPEND TST_ADDED ccw_check_1 ccw_check_2 ccw_check_3)
endif()

# ---------------------------------------------------------------------------
# intrin: TEST_CASE を使う C++ テストなので、cxx_test の集約 main (TEST_RUN) と
# libcxx03 のプリビルドライブラリを一緒にリンクする (= 1 ファイル版の htest)。
#
# **2 通り建てる**: 規格どおり (-xs -xr、接尾辞なしの lib) と、例外も RTTI も無し
# (-noeh の lib)。intrinsic は例外・RTTI の有無で codegen が変わりうるので、
# 片方だけ見ていると気づけない。結果 tsv は tag が分かれる (後者は -noeh 付き)。
# ---------------------------------------------------------------------------
set(TST_WANT_INTRIN ON)
if(TST_ONLY)
    list(FIND TST_ONLY "intrin" idx)
    if(idx EQUAL -1)
        set(TST_WANT_INTRIN OFF)
    endif()
endif()

# ccwrap_etc_wat_add_intrin(<name> <lib subdir> <opts>...)
function(ccwrap_etc_wat_add_intrin name libdir)
    set(lib "${CCWRAP_ROOT}/watcom/lib/${libdir}/libccwcxx.lib")
    if(NOT EXISTS "${lib}")
        message(FATAL_ERROR "prebuilt ${lib} missing -- run `gen` (and `gen full` if this is a ccwstd / -fs-char / -xst variant) in ${CCWRAP_ROOT}/watcom/lib")
    endif()
    add_executable(${name}
        "${CMAKE_CURRENT_SOURCE_DIR}/intrin_test.cpp"
        "${CCWRAP_ROOT}/test/cxx_test/test_cxx_vc.cpp")
    # test/cxx_test も include パスへ: intrin_test.cpp は共通プロローグ "test_cxx.hpp" を引く
    target_include_directories(${name} PRIVATE "${CCWRAP_ROOT}/test/cxx_test" ${TST_CXX_INCS})
    target_compile_options(${name} PRIVATE ${ARGN})
    target_compile_definitions(${name} PRIVATE ${TST_CXX_DEFS})
    target_link_libraries(${name} PRIVATE "${lib}")
    # 集約 main は失敗グループ数を返すので ctest が失敗を検知できる。
    # リポジトリルートで走らせ、pass ログを test/result_cxx_test/<tag> に落とす。
    add_test(NAME ${name} COMMAND ${name} WORKING_DIRECTORY "${CCWRAP_ROOT}")
endfunction()

if(TST_WANT_INTRIN)
    # 規格どおり (-xs -xr)。TST_CXX_OPTS は既定でこれ。
    ccwrap_etc_wat_add_intrin(intrin "win32-${TST_NS}${TST_LIB_SFX}" ${TST_CXX_OPTS})
    list(APPEND TST_ADDED intrin)

    # 例外も RTTI も無し。routing を「その構成で」もう一度呼んで opts を作る
    # (通常変数がキャッシュを覆うので、呼び終えたら unset で戻す)。
    set(TEST_NO_EH ON)
    set(TEST_WATCOM_RTTI OFF)
    ccwrap_test_routing(LANG CXX LEVEL "${TEST_TARGET_CXX}" NS "${TST_NS}"
                        OUT_OPTS TST_CXX_OPTS_NOEH)
    unset(TEST_NO_EH)
    unset(TEST_WATCOM_RTTI)
    ccwrap_etc_wat_add_intrin(intrin_noeh "win32-${TST_NS}-noeh" ${TST_CXX_OPTS_NOEH})
    list(APPEND TST_ADDED intrin_noeh)
endif()

# ---------------------------------------------------------------------------
# bugchk: doc/watcom/ccwrap_watcom_bugs.md のバグ・制約が**まだ生きているか**を、
# 素のコンパイラだけを叩いて機械的に確かめる (watcom_bugchk/)。ccwrap は踏まない
# ので、ここが赤い = Watcom 側が変わった、と読める。
#
# 終了コードは **REGRESSION (カタログが ok と言っているのに壊れた) のときだけ 1**。
# 「治った」(FIXED) は 0 のまま出力に出る — 治ったかどうかは新旧比較 (--compare)
# でしか確定できないので、日常の ctest で赤にはしない。
#
# Watcom を入れ替えたときは、ctest でなく直接こう叩く (README 参照):
#   python3 test/etc_test/watcom/watcom_bugchk/bugchk.py --compare "C:\WATCOM2.0.pre"
# ---------------------------------------------------------------------------
set(TST_WANT_BUGCHK ON)
if(TST_ONLY)
    list(FIND TST_ONLY "bugchk" idx)
    if(idx EQUAL -1)
        set(TST_WANT_BUGCHK OFF)
    endif()
endif()

if(TST_WANT_BUGCHK)
    find_package(Python3 COMPONENTS Interpreter QUIET)
    if(Python3_Interpreter_FOUND)
        add_test(NAME bugchk
                 COMMAND "${Python3_EXECUTABLE}"
                         "${CMAKE_CURRENT_SOURCE_DIR}/watcom_bugchk/bugchk.py"
                 WORKING_DIRECTORY "${CCWRAP_ROOT}")
        list(APPEND TST_ADDED bugchk)
    else()
        message(STATUS "etc_test/watcom: python not found -- bugchk not registered")
    endif()
endif()

# ---------------------------------------------------------------------------
# eh_sweep: watcom/std と watcom/ccwstd の**全ヘッダを 1 本ずつ TU にして**、-xs
# 有りと無しの両方でコンパイルする。「-xs 有りなら通るのに無しだと落ちる」ものだけを
# 失敗にする (両方落ちるものは例外と無関係なので報告だけ)。
#
# ヘッダ別スイートが**構造的に踏めない形**を見るのがここの役目 — 翻訳単位が最初に
# 読む標準ヘッダが何か。cxx_test は必ず test_cxx.hpp から入るので、「<stdio.h> を
# 最初に include したときだけ壊れる」類の循環 include / 宣言順の事故が見えない
# (実際に 2 件その形で出た)。
# ---------------------------------------------------------------------------
set(TST_WANT_EH_SWEEP ON)
if(TST_ONLY)
    list(FIND TST_ONLY "eh_sweep" idx)
    if(idx EQUAL -1)
        set(TST_WANT_EH_SWEEP OFF)
    endif()
endif()

if(TST_WANT_EH_SWEEP)
    find_package(Python3 COMPONENTS Interpreter QUIET)
    if(Python3_Interpreter_FOUND)
        add_test(NAME eh_sweep
                 COMMAND "${Python3_EXECUTABLE}"
                         "${CMAKE_CURRENT_SOURCE_DIR}/eh_sweep/eh_sweep.py"
                 WORKING_DIRECTORY "${CCWRAP_ROOT}")
        list(APPEND TST_ADDED eh_sweep)
    else()
        message(STATUS "etc_test/watcom: python not found -- eh_sweep not registered")
    endif()
endif()

if(NOT TST_ADDED)
    message(STATUS "etc_test/watcom: nothing selected -- skipped")
endif()
