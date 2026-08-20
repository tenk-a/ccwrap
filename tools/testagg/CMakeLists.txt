cmake_minimum_required(VERSION 3.15)
project(testagg CXX)

add_executable(testagg main.cpp)

if(MSVC)
    target_compile_definitions(testagg PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(testagg PRIVATE /W3 /EHsc)
endif()
