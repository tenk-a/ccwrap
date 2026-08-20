cmake_minimum_required(VERSION 3.29)

if(POLICY CMP0136)
    cmake_policy(SET CMP0136 NEW)
endif()

project(ccwrap_compile_fail C CXX)


include("${CMAKE_CURRENT_LIST_DIR}/../bin/ccwrap_test.cmake")

set(CCWRAP_ROOT "${CMAKE_CURRENT_LIST_DIR}/../..")

set(TEST_TARGET_C "2023" CACHE STRING "_CCW_TARGET_C: the C feature level the ccwrap backports expose")
set(TEST_TARGET_CXX "2023" CACHE STRING "_CCW_TARGET_CXX: the C++ feature level the ccwrap backports expose")
set(TST_NS "std" CACHE STRING "Namespace environment on Open Watcom: 'std' or 'ccwstd'")
set(TST_ONLY "" CACHE STRING "Build only these compile_fail cases (base names). Empty = all.")

enable_testing()

ccwrap_test_routing(LANG C   LEVEL "${TEST_TARGET_C}" NS "${TST_NS}"
                    OUT_INCS CF_C_INCS OUT_OPTS CF_C_OPTS OUT_DEFS CF_C_DEFS)
ccwrap_test_routing(LANG CXX LEVEL "${TEST_TARGET_CXX}" NS "${TST_NS}"
                    OUT_INCS CF_CXX_INCS OUT_OPTS CF_CXX_OPTS OUT_DEFS CF_CXX_DEFS)

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
        file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/${base}.lst" lines ENCODING UTF-8)
        foreach(line IN LISTS lines)
            string(STRIP "${line}" line)
            if(line AND NOT line MATCHES "^#")
                list(APPEND macros "${line}")
            endif()
        endforeach()
    endif()

    ccwrap_test_probe(SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${src}" INCS "${incs}" OPTS "${opts}"
                      DEFS "${defs}" PREFIX CF_BASE_COMPILES LABEL "compile_fail"
                      OUT_SRC ok)
    if(NOT ok)
        return()
    endif()

    ccwrap_compile_fail_target("cf_${base}_base" "${src}" "" "${incs}" "${opts}" "${defs}" OFF)
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
