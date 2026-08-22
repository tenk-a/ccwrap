cmake_minimum_required(VERSION 3.29)

if(POLICY CMP0136)
    cmake_policy(SET CMP0136 NEW)
endif()

project(ccwrap_header_test C CXX)


include("${CMAKE_CURRENT_LIST_DIR}/../bin/ccwrap_test.cmake")

get_filename_component(CCWRAP_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)

option(TST_PROBE "Probe each test file and drop the ones that do not compile" ON)
set(TEST_TARGET_CXX "" CACHE STRING "_CCW_TARGET_CXX: the feature level ccwrap should expose. EMPTY (default) = do not use ccwrap at all.")
set(TST_ONLY "" CACHE STRING "Build only these cxx_test files (base names w/o _test.cpp, ;-separated). Empty = all.")
set(TST_NS "std" CACHE STRING "Namespace environment on Open Watcom: 'std' (default) or 'ccwstd' (libcxx03 relocated; forces watcom/ccwstd/ + the ccwstd prebuilt lib). Tests spell STD:: so both work.")
set(TST_MSVC_STD "c++latest" CACHE STRING "MSVC only: the /std: level (c++14 / c++17 / c++20 / c++latest)")
set(TST_STDLIB "" CACHE STRING "clang only: 'libcxx' when the preset passes -stdlib=libc++. Empty = the compiler's default library.")

enable_testing()

ccwrap_test_routing(LANG CXX LEVEL "${TEST_TARGET_CXX}" NS "${TST_NS}" MSVC_STD "${TST_MSVC_STD}" C_CONFORM
                    OUT_INCS TST_INCS OUT_OPTS TST_OPTS OUT_DEFS TST_DEFS)

option(TEST_FS_CHAR "filesystem: path::value_type を char に固定する (_CCW_FS_WCHAR=0)" OFF)
if(TEST_FS_CHAR)
    list(APPEND TST_DEFS _CCW_FS_WCHAR=0)
    message(STATUS "cxx_test: filesystem path::value_type = char (_CCW_FS_WCHAR=0)")
endif()
if(TEST_TARGET_CXX)
    message(STATUS "cxx_test: routed through ccwrap (_CCW_TARGET_CXX=${TEST_TARGET_CXX}, ns=${TST_NS})")
else()
    message(STATUS "cxx_test: the compiler's own standard library only (no ccwrap)")
endif()
list(INSERT TST_INCS 0 "${CMAKE_CURRENT_LIST_DIR}")

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

set(TST_EXTRA_SRC "")
if(TST_CC STREQUAL "watcom" AND TEST_TARGET_CXX)
    ccwrap_watcom_lib(TST_CCWRAP_LIB)
    if(NOT EXISTS "${TST_CCWRAP_LIB}")
        if(TEST_NO_EH AND TEST_WATCOM_RTTI)
            message(FATAL_ERROR "TEST_NO_EH=ON + TEST_WATCOM_RTTI=ON (-xr) は prebuilt を用意していない -- preset `watcom-noeh` (例外も RTTI も無し) を使うか、TEST_WATCOM_RTTI=OFF を渡すこと")
        endif()
        message(FATAL_ERROR "prebuilt ${TST_CCWRAP_LIB} missing -- run `gen` in ${CCWRAP_ROOT}/watcom/lib (that builds ccw-eh3r.lib and ccw-ne3r.lib; ccwstd / -fsc / -xst are opt-in: `gen full`; -xrxs only from `gen xrxs`)")
    endif()
elseif(TEST_TARGET_CXX AND (TST_CC STREQUAL "msvc" OR TST_CC STREQUAL "gnu"))
    set(TST_CCWRAP_LIB "")
    list(APPEND TST_EXTRA_SRC "${CCWRAP_ROOT}/detail/llibcxx03/src/filesystem.cpp")
    if(TST_NS STREQUAL "ccwstd")
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
ccwrap_test_utf8_acp(ccw_htest)

if(TST_CC STREQUAL "watcom")
    set(TST_ST_SRC "${CMAKE_CURRENT_LIST_DIR}/stacktrace_test.cpp")
    if(EXISTS "${TST_ST_SRC}")
        set_source_files_properties("${TST_ST_SRC}" PROPERTIES COMPILE_OPTIONS "-of+")
    endif()
endif()
if(TST_CCWRAP_LIB)
    target_link_libraries(ccw_htest PRIVATE "${TST_CCWRAP_LIB}")
endif()

add_test(NAME htest COMMAND ccw_htest WORKING_DIRECTORY "${CCWRAP_ROOT}")

ccwrap_test_skip_counts()
