cmake_minimum_required(VERSION 3.29)

if(POLICY CMP0136)
    cmake_policy(SET CMP0136 NEW)
endif()
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

project(ccwrap_test C CXX)

# ---------------------------------------------------------------------------
# 4 スイートをまとめて 1 回の configure → build → test で回すための umbrella。
#
#   cd test
#   cmake --preset watcom && cmake --build --preset watcom && ctest --preset watcom
#   cmake --preset vc145  && cmake --build --preset vc145  && ctest --preset vc145
#   cmake --preset gcc    && cmake --build --preset gcc    && ctest --preset gcc
#   cmake --preset dmc    && cmake --build --preset dmc    && ctest --preset dmc
#   cmake --preset orangec && cmake --build --preset orangec && ctest --preset orangec
#
# (お手軽ラッパ: test/run.bat <preset> / test/run.sh <preset> が上の 3 行を順に呼ぶ)
#
# スイート:
#   c_test        ヘッダ別 C テスト
#   cxx_test      ヘッダ別 C++ テスト
#   compile_fail  ネガティブテスト (コンパイルが通らないことが合格)
#   etc_vc        ヘッダ別でない MSVC スモーク (etc_test/vc)      … MSVC のときだけ
#   etc_watcom    ヘッダ別でない Watcom スモーク (etc_test/watcom) … Watcom のときだけ
#
# TST_SUITES=auto (既定) は、そのコンパイラで意味のあるスイートを全部選ぶ。
# 明示するなら  -DTST_SUITES=c_test;etc_watcom  のように書く。
#
# 各スイートは**単体でも configure できる** (それぞれ CMakeLists.txt と
# CMakePresets.json を持つ)。ここはそれを束ねるだけで、配線は増やさない。
#
# TST_ONLY は全スイート共通の knob。umbrella では「そのスイートに無い名前」は
# エラーにせず読み飛ばすので、-DTST_ONLY=string;watcom_c99 のようにスイートを
# またいで指定できる。
# ---------------------------------------------------------------------------

set(CCWRAP_TEST_UMBRELLA ON)
include("${CMAKE_CURRENT_LIST_DIR}/bin/ccwrap_test.cmake")
ccwrap_test_compiler(TST_CC)

set(TST_SUITES "auto" CACHE STRING "Suites to build: auto | c_test;cxx_test;compile_fail;etc_vc;etc_watcom")

enable_testing()

if(TST_SUITES STREQUAL "auto")
    # compile_fail はどのコンパイラでも回る。ライブラリに機能そのものが無くて成立
    # しないケース (numeric_satarith = C++26 のサチュレート演算) は、compile_fail 側が
    # 基準ケースを probe して自分で外し、外したことを表示する。
    set(TST_SUITES_RESOLVED c_test cxx_test compile_fail)
    if(TST_CC STREQUAL "msvc")
        list(APPEND TST_SUITES_RESOLVED etc_vc)
    elseif(TST_CC STREQUAL "watcom")
        list(APPEND TST_SUITES_RESOLVED etc_watcom)
    endif()
else()
    set(TST_SUITES_RESOLVED ${TST_SUITES})
endif()
string(REPLACE ";" " " _s "${TST_SUITES_RESOLVED}")
message(STATUS "ccwrap test suites: ${_s}")

foreach(suite IN LISTS TST_SUITES_RESOLVED)
    if(suite STREQUAL "c_test")
        add_subdirectory(c_test)
    elseif(suite STREQUAL "cxx_test")
        add_subdirectory(cxx_test)
    elseif(suite STREQUAL "compile_fail")
        add_subdirectory(compile_fail)
    elseif(suite STREQUAL "etc_vc")
        add_subdirectory(etc_test/vc)
    elseif(suite STREQUAL "etc_watcom")
        add_subdirectory(etc_test/watcom)
    else()
        message(FATAL_ERROR "TST_SUITES: unknown suite '${suite}' (c_test|cxx_test|compile_fail|etc_vc|etc_watcom)")
    endif()
endforeach()
