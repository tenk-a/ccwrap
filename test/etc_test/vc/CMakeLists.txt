cmake_minimum_required(VERSION 3.21)

if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

project(ccwrap_etc_test_vc C CXX)


include("${CMAKE_CURRENT_LIST_DIR}/../../bin/ccwrap_test.cmake")

ccwrap_test_compiler(TST_CC)
if(NOT TST_CC STREQUAL "msvc")
    message(FATAL_ERROR "etc_test/vc needs MSVC (cl). Use the vc* presets, or a VS developer prompt.")
endif()

set(CCWRAP_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../..")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(TEST_TARGET_C "2023" CACHE STRING "_CCW_TARGET_C: the C feature level the ccwrap backports expose")
set(TEST_TARGET_CXX "2023" CACHE STRING "_CCW_TARGET_CXX: the C++ feature level the ccwrap backports expose")
set(TST_ONLY "" CACHE STRING "Build only these etc_test programs (vc_c;vc_cpp). Empty = all.")

enable_testing()

ccwrap_test_routing(LANG C   LEVEL "${TEST_TARGET_C}" C_CONFORM
                    OUT_INCS TST_C_INCS OUT_OPTS TST_C_OPTS OUT_DEFS TST_C_DEFS)
ccwrap_test_routing(LANG CXX LEVEL "${TEST_TARGET_CXX}"
                    OUT_INCS TST_CXX_INCS OUT_OPTS TST_CXX_OPTS OUT_DEFS TST_CXX_DEFS)
list(APPEND TST_C_INCS "${CCWRAP_ROOT}/ccw")
list(APPEND TST_CXX_INCS "${CCWRAP_ROOT}/ccw")

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
    add_test(NAME ${name} COMMAND ${name} WORKING_DIRECTORY "${CCWRAP_ROOT}")
endfunction()

ccwrap_etc_vc_add(vc_c   vc_c.c     C)
ccwrap_etc_vc_add(vc_cpp vc_cpp.cpp CXX)

if(NOT TARGET vc_c AND NOT TARGET vc_cpp)
    message(STATUS "etc_test/vc: nothing selected -- skipped")
endif()
