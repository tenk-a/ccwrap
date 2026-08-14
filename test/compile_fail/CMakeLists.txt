cmake_minimum_required(VERSION 3.29)

if(POLICY CMP0136)
    cmake_policy(SET CMP0136 NEW)
endif()

project(ccwrap_compile_fail C CXX)

# ---------------------------------------------------------------------------
# ネガティブテスト (compile-fail)。**コンパイルが通らないこと**が合格。
#
#   cmake --preset watcom && cmake --build --preset watcom && ctest --preset watcom
#
# 規約 (README.md と同じ。従来 run_watcom.bat :run_cfail がやっていたもの):
#   - X.cpp / X.c は **マクロ無しでは通る**こと (無関係な理由で落ちるようになった
#     ファイルは、以下の全ケースを「間違った理由で合格」にしてしまう)。
#   - X.lst に 1 行 1 マクロ。**そのマクロを定義すると通らない**こと。'#' 始まりはコメント。
#
# **今のところ Watcom 前提**: numeric_bool.cpp が C++26 の add_sat/sub_sat/... を使い、
# それを提供しているのが libcxx03 だけなので、vc/gcc ではそのケースの基準ビルドが落ちる
# (ccwrap の vc/gcc 側にサチュレート演算が無いという実際の欠落)。他のケース
# (ratio_illformed / static_assert_c) は -DTST_ONLY= で選べば vc/gcc でも回せる。
#
# CMake での実現: ケースごとに EXCLUDE_FROM_ALL のオブジェクトライブラリを作り、
# 「そのターゲットをビルドする」ことを ctest のテストにする。落ちるべきケースには
# WILL_FAIL を付ける。ジェネレータに依存しないうえ、コンパイラ別の配線
# (強制 include / include パス / -xs) が他スイートと**同じ経路**で効く。
# ---------------------------------------------------------------------------

include("${CMAKE_CURRENT_LIST_DIR}/../bin/ccwrap_test.cmake")

set(CCWRAP_ROOT "${CMAKE_CURRENT_LIST_DIR}/../..")

# etc_test / compile_fail は **ccwrap そのもののテスト**なので、ここだけは既定で
# ccwrap を通す (値が空だとテスト内容が成立しない)。ヘッダ別スイート (c_test /
# cxx_test) は逆に既定が「素のコンパイラ」で、値を渡したときだけ ccwrap になる。
set(TEST_TARGET_C "2023" CACHE STRING "_CCW_TARGET_C: the C feature level the ccwrap backports expose")
set(TEST_TARGET_CXX "2023" CACHE STRING "_CCW_TARGET_CXX: the C++ feature level the ccwrap backports expose")
set(TST_NS "std" CACHE STRING "Namespace environment on Open Watcom: 'std' or 'ccwstd'")
set(TST_ONLY "" CACHE STRING "Build only these compile_fail cases (base names). Empty = all.")

enable_testing()

ccwrap_test_routing(LANG C   LEVEL "${TEST_TARGET_C}" NS "${TST_NS}"
                    OUT_INCS CF_C_INCS OUT_OPTS CF_C_OPTS OUT_DEFS CF_C_DEFS)
ccwrap_test_routing(LANG CXX LEVEL "${TEST_TARGET_CXX}" NS "${TST_NS}"
                    OUT_INCS CF_CXX_INCS OUT_OPTS CF_CXX_OPTS OUT_DEFS CF_CXX_DEFS)

# ---------------------------------------------------------------------------
# 1 ケース = EXCLUDE_FROM_ALL のオブジェクトライブラリ 1 個 + 「それをビルドする」テスト 1 個
# ---------------------------------------------------------------------------
function(ccwrap_compile_fail_target tname src macro incs opts defs must_fail)
    add_library(${tname} OBJECT EXCLUDE_FROM_ALL "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
    target_include_directories(${tname} PRIVATE ${incs})
    target_compile_options(${tname} PRIVATE ${opts})
    target_compile_definitions(${tname} PRIVATE ${defs})
    if(macro)
        target_compile_definitions(${tname} PRIVATE ${macro})
    endif()
    add_test(NAME ${tname}
             COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}"
                     --target ${tname} --config $<CONFIG>)
    if(must_fail)
        set_tests_properties(${tname} PROPERTIES WILL_FAIL TRUE)
    endif()
endfunction()

# ---------------------------------------------------------------------------
# ccwrap_compile_fail_case(<src>) : <base>.lst のマクロ 1 個 = テスト 1 個
# ---------------------------------------------------------------------------
function(ccwrap_compile_fail_case src)
    get_filename_component(base "${src}" NAME_WE)
    get_filename_component(ext "${src}" EXT)
    if(TST_ONLY)
        list(FIND TST_ONLY "${base}" idx)
        if(idx EQUAL -1)
            return()
        endif()
    endif()
    if(ext STREQUAL ".c")
        set(incs ${CF_C_INCS})
        set(opts ${CF_C_OPTS})
        set(defs ${CF_C_DEFS})
    else()
        set(incs ${CF_CXX_INCS})
        set(opts ${CF_CXX_OPTS})
        set(defs ${CF_CXX_DEFS})
    endif()

    set(macros "")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${base}.lst")
        # ENCODING UTF-8 は必須: 既定の file(STRINGS) は strings(1) と同じで
        # **印字可能 ASCII だけ**を拾うため、日本語コメント行が途中でぶつ切りになり
        # 断片がマクロ名として残る。
        file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/${base}.lst" lines ENCODING UTF-8)
        foreach(line IN LISTS lines)
            string(STRIP "${line}" line)
            if(line AND NOT line MATCHES "^#")
                list(APPEND macros "${line}")
            endif()
        endforeach()
    endif()

    # ケースが「そのライブラリに機能そのものが無い」ために成立しないことがある
    # (numeric_satarith は C++26 のサチュレート演算を要求し、持っているのは libcxx03 だけ)。
    # そこを赤で放置すると、**基準ビルドが落ちたまま REJ_* が全部「合格」に見える**
    # という一番まずい状態になるので、configure 時に基準ケースだけ probe して、
    # 通らないならケースごと登録しない (何を落としたかは必ず表示する)。
    ccwrap_test_probe(SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${src}" INCS "${incs}" OPTS "${opts}"
                      DEFS "${defs}" PREFIX CF_BASE_COMPILES LABEL "compile_fail"
                      OUT_SRC ok)
    if(NOT ok)
        return()
    endif()

    # 基準ケース: マクロ無しでは**通らなければならない**
    ccwrap_compile_fail_target("cf_${base}_base" "${src}" "" "${incs}" "${opts}" "${defs}" OFF)
    # 各マクロ: 定義すると**通ってはならない**
    foreach(m IN LISTS macros)
        ccwrap_compile_fail_target("cf_${base}_${m}" "${src}" "${m}" "${incs}" "${opts}" "${defs}" ON)
    endforeach()
endfunction()

file(GLOB CF_SRCS CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(SORT CF_SRCS)
foreach(src IN LISTS CF_SRCS)
    get_filename_component(n "${src}" NAME)
    ccwrap_compile_fail_case("${n}")
endforeach()
