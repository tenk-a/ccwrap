cmake_minimum_required(VERSION 3.21)

if(POLICY CMP0136)
    cmake_policy(SET CMP0136 NEW)
endif()
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

project(ccwrap_c_test C)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()


include("${CMAKE_CURRENT_LIST_DIR}/../bin/ccwrap_test.cmake")

get_filename_component(CCWRAP_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)

option(TST_PROBE "Probe each test file and drop the ones that do not compile" ON)
set(TEST_TARGET_C "" CACHE STRING "_CCW_TARGET_C: the C feature level ccwrap should expose. EMPTY (default) = do not use ccwrap at all.")
set(TST_ONLY "" CACHE STRING "Build only these <header>_test.c (base names w/o _test.c, ;-separated). Empty = all.")

enable_testing()

ccwrap_test_routing(LANG C LEVEL "${TEST_TARGET_C}" C_CONFORM
                    OUT_INCS TST_INCS OUT_OPTS TST_OPTS OUT_DEFS TST_DEFS)
if(TEST_TARGET_C)
    message(STATUS "c_test: routed through ccwrap (_CCW_TARGET_C=${TEST_TARGET_C})")
else()
    message(STATUS "c_test: the compiler's own C library only (no ccwrap)")
endif()
list(INSERT TST_INCS 0 "${CCWRAP_ROOT}/test/src")   # test/src: c_test.h + test_env.h

list(APPEND TST_DEFS __STDC_WANT_LIB_EXT1__=1)

ccwrap_test_select(EXT ".c" DIRS "${CMAKE_CURRENT_LIST_DIR}" ONLY "${TST_ONLY}"
                   OUT_SRC TST_CTEST_ALL OUT_PROBE TST_DO_PROBE)
if(TST_ONLY)
    message(STATUS "c_test: building only: ${TST_ONLY}")
endif()
if(NOT TST_PROBE)
    set(TST_DO_PROBE OFF)
endif()

if(NOT TST_CTEST_ALL)
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

set(TST_MAIN "${CMAKE_CURRENT_LIST_DIR}/test_main.c")
set(TST_HAVE_DEFS "")
foreach(src IN LISTS TST_CTEST_SRC)
    get_filename_component(name "${src}" NAME_WE)
    string(REGEX REPLACE "_test$" "" suite "${name}")
    list(APPEND TST_HAVE_DEFS "TST_HAVE_${suite}")
endforeach()

add_executable(ccw_ctest "${CCWRAP_ROOT}/test/src/c_test.c" "${TST_MAIN}" ${TST_CTEST_SRC})
target_include_directories(ccw_ctest PRIVATE ${TST_INCS})
target_compile_options(ccw_ctest PRIVATE ${TST_OPTS})
target_compile_definitions(ccw_ctest PRIVATE ${TST_DEFS} ${TST_HAVE_DEFS})
ccwrap_test_link_libs(ccw_ctest LANG C)      # gcc/clang: -lm / threads / -latomic
ccwrap_test_utf8_acp(ccw_ctest)

add_test(NAME ctest COMMAND ccw_ctest WORKING_DIRECTORY "${CCWRAP_ROOT}")

ccwrap_test_skip_counts()
