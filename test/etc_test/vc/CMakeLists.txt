cmake_minimum_required(VERSION 3.21)

# CMP0091 (>=3.15): abstract MSVC runtime selection so the STATIC CRT can be forced
# (old vc link the CRT as a side-by-side assembly DLL that is not in WinSxS).
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

project(ccwrap_etc_test_vc C CXX)

# ---------------------------------------------------------------------------
# MSVC 用の「ヘッダ別でない」スモークテスト (etc_test)。
#
#   cmake --preset vc145 && cmake --build --preset vc145 && ctest --preset vc145
#
# ヘッダ別スイート (c_test / cxx_test) と違い、**1 つの TU から ccwrap の面を丸ごと**
# 触るテスト。よって配線もヘッダ別スイートと同じ (強制 include vc/ccwrap_header.h、
# _CCW_TARGET_C/_CXX、C 準拠 opt-in、static CRT) が要る。_CCW_TARGET_C=2023 が無いと
# テストが使う C23 の名前 (INT_WIDTH / char8_t ...) が補充されずコンパイルできない。
#
#   vc_c.c    C スモーク: 全 C ヘッダが include でき、C11/C23 機能が動く (build+run)
#   vc_cpp.cpp C++ スモーク: C ヘッダが C++ から使える (build+run)
#
# 個別ビルドは他スイートと同じ knob:  -DTST_ONLY=vc_c
# ---------------------------------------------------------------------------

include("${CMAKE_CURRENT_LIST_DIR}/../../bin/ccwrap_test.cmake")

ccwrap_test_compiler(TST_CC)
if(NOT TST_CC STREQUAL "msvc")
    message(FATAL_ERROR "etc_test/vc needs MSVC (cl). Use the vc* presets, or a VS developer prompt.")
endif()

set(CCWRAP_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../..")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# etc_test / compile_fail は **ccwrap そのもののテスト**なので、ここだけは既定で
# ccwrap を通す (値が空だとテスト内容が成立しない)。ヘッダ別スイート (c_test /
# cxx_test) は逆に既定が「素のコンパイラ」で、値を渡したときだけ ccwrap になる。
set(TEST_TARGET_C "2023" CACHE STRING "_CCW_TARGET_C: the C feature level the ccwrap backports expose")
set(TEST_TARGET_CXX "2023" CACHE STRING "_CCW_TARGET_CXX: the C++ feature level the ccwrap backports expose")
set(TST_ONLY "" CACHE STRING "Build only these etc_test programs (vc_c;vc_cpp). Empty = all.")

enable_testing()

ccwrap_test_routing(LANG C   LEVEL "${TEST_TARGET_C}" C_CONFORM
                    OUT_INCS TST_C_INCS OUT_OPTS TST_C_OPTS OUT_DEFS TST_C_DEFS)
ccwrap_test_routing(LANG CXX LEVEL "${TEST_TARGET_CXX}"
                    OUT_INCS TST_CXX_INCS OUT_OPTS TST_CXX_OPTS OUT_DEFS TST_CXX_DEFS)
# ccw/: ccwrap 標準ライブラリ以外の C/C++ ソース (テストが触る)
list(APPEND TST_C_INCS "${CCWRAP_ROOT}/ccw")
list(APPEND TST_CXX_INCS "${CCWRAP_ROOT}/ccw")

# ---------------------------------------------------------------------------
# 1 プログラム = 1 実行ファイル。TST_ONLY で絞れる。
# ---------------------------------------------------------------------------
function(ccwrap_etc_vc_add name src lang)
    if(TST_ONLY)
        list(FIND TST_ONLY "${name}" idx)
        if(idx EQUAL -1)
            return()
        endif()
    endif()
    add_executable(${name} "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
    if(lang STREQUAL "C")
        target_include_directories(${name} PRIVATE ${TST_C_INCS})
        target_compile_options(${name} PRIVATE ${TST_C_OPTS} /TC)
        target_compile_definitions(${name} PRIVATE ${TST_C_DEFS})
    else()
        target_include_directories(${name} PRIVATE ${TST_CXX_INCS})
        target_compile_options(${name} PRIVATE ${TST_CXX_OPTS} /TP)
        target_compile_definitions(${name} PRIVATE ${TST_CXX_DEFS})
    endif()
    # リポジトリルートで走らせる (テストが相対パスを使う場合に他スイートと揃える)
    add_test(NAME ${name} COMMAND ${name} WORKING_DIRECTORY "${CCWRAP_ROOT}")
endfunction()

ccwrap_etc_vc_add(vc_c   vc_c.c     C)
ccwrap_etc_vc_add(vc_cpp vc_cpp.cpp CXX)

if(NOT TARGET vc_c AND NOT TARGET vc_cpp)
    message(STATUS "etc_test/vc: nothing selected -- skipped")
endif()
