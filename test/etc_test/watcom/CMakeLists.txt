cmake_minimum_required(VERSION 3.29)

if(POLICY CMP0136)
    cmake_policy(SET CMP0136 NEW)
endif()

project(ccwrap_etc_test_watcom C CXX)


include("${CMAKE_CURRENT_LIST_DIR}/../../bin/ccwrap_test.cmake")

ccwrap_test_compiler(TST_CC)
if(NOT TST_CC STREQUAL "watcom")
    message(FATAL_ERROR "etc_test/watcom needs Open Watcom (wcl386). Use the watcom preset.")
endif()

set(CCWRAP_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../..")

set(TEST_TARGET_C "2026" CACHE STRING "_CCW_TARGET_C: the C feature level the ccwrap backports expose")
set(TEST_TARGET_CXX "2026" CACHE STRING "_CCW_TARGET_CXX: the C++ feature level the ccwrap backports expose")
set(TST_NS "std" CACHE STRING "Namespace environment: 'std' (default) or 'ccwstd' (libcxx03 relocated)")
set(TST_ONLY "" CACHE STRING "Build only these etc_test programs (watcom_ctest;watcom_cinclude;...). Empty = all.")

enable_testing()

ccwrap_test_routing(LANG C   LEVEL "${TEST_TARGET_C}" NS "${TST_NS}"
                    OUT_INCS TST_C_INCS OUT_OPTS TST_C_OPTS OUT_DEFS TST_C_DEFS)
ccwrap_test_routing(LANG CXX LEVEL "${TEST_TARGET_CXX}" NS "${TST_NS}"
                    OUT_INCS TST_CXX_INCS OUT_OPTS TST_CXX_OPTS OUT_DEFS TST_CXX_DEFS)

ccwrap_watcom_lib(TST_CCWRAP_LIB)

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
ccwrap_etc_wat_add_c(watcom_win32_coexist watcom_win32_coexist_test.c)
ccwrap_etc_wat_add_c(watcom_win32_coexist2 watcom_win32_coexist2_test.c)
ccwrap_etc_wat_add_c(watcom_c99      watcom_c99_test.c OPTS -za99)

function(ccwrap_etc_wat_add_cxx name src)
    if(TST_ONLY)
        list(FIND TST_ONLY "${name}" idx)
        if(idx EQUAL -1)
            return()
        endif()
    endif()
    set(lib "${TST_CCWRAP_LIB}")
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
ccwrap_etc_wat_add_cxx(watcom_locale_ctype watcom_locale_ctype_test.cpp)
ccwrap_etc_wat_add_cxx(watcom_win32_coexist_cxx watcom_win32_coexist_cxx_test.cpp)

if(NOT TST_ONLY OR "watcom_eh_catch_only" IN_LIST TST_ONLY)
    set(_ehlib "${TST_CCWRAP_LIB}")
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

if(NOT TST_ONLY OR "ccw_check" IN_LIST TST_ONLY)
    set(lib "${TST_CCWRAP_LIB}")
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

set(TST_WANT_INTRIN ON)
if(TST_ONLY)
    list(FIND TST_ONLY "intrin" idx)
    if(idx EQUAL -1)
        set(TST_WANT_INTRIN OFF)
    endif()
endif()

function(ccwrap_etc_wat_add_intrin name lib)
    if(NOT EXISTS "${lib}")
        message(FATAL_ERROR "prebuilt ${lib} missing -- run `gen` (and `gen full` if this is a ccwstd / -fsc / -xst variant) in ${CCWRAP_ROOT}/watcom/lib")
    endif()
    add_executable(${name}
        "${CMAKE_CURRENT_SOURCE_DIR}/intrin_test.cpp"
        "${CCWRAP_ROOT}/test/cxx_test/test_cxx_vc.cpp")
    target_include_directories(${name} PRIVATE "${CCWRAP_ROOT}/test/cxx_test" ${TST_CXX_INCS})
    target_compile_options(${name} PRIVATE ${ARGN})
    target_compile_definitions(${name} PRIVATE ${TST_CXX_DEFS})
    target_link_libraries(${name} PRIVATE "${lib}")
    add_test(NAME ${name} COMMAND ${name} WORKING_DIRECTORY "${CCWRAP_ROOT}")
endfunction()

if(TST_WANT_INTRIN)
    ccwrap_etc_wat_add_intrin(intrin "${TST_CCWRAP_LIB}" ${TST_CXX_OPTS})
    list(APPEND TST_ADDED intrin)

    set(TEST_NO_EH ON)
    set(TEST_WATCOM_RTTI OFF)
    ccwrap_test_routing(LANG CXX LEVEL "${TEST_TARGET_CXX}" NS "${TST_NS}"
                        OUT_OPTS TST_CXX_OPTS_NOEH)
    unset(TEST_NO_EH)
    unset(TEST_WATCOM_RTTI)
    ccwrap_watcom_lib(TST_CCWRAP_LIB_NOEH NO_EH ON RTTI OFF XS OFF)
    ccwrap_etc_wat_add_intrin(intrin_noeh "${TST_CCWRAP_LIB_NOEH}" ${TST_CXX_OPTS_NOEH})
    list(APPEND TST_ADDED intrin_noeh)
endif()

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
