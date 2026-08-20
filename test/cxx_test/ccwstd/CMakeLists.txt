# ccwrap ccwstd smoke build: compiles libcxx03 relocated into namespace `ccwstd`.
# Only watcom/ccwstd/ (+ detail/llibcxx03/include) is on the include path
# -- watcom/std/ is NOT --
# and watcom/ccwstd/ccwrap_header.h is force-included to select the ccwstd namespace.
cmake_minimum_required(VERSION 3.20)
project(ccwrap_ccwstd_smoke CXX)

get_filename_component(CCWRAP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../.." ABSOLUTE)

file(TO_NATIVE_PATH "${CCWRAP_ROOT}/watcom/ccwstd/ccwrap_header.h" TST_CCWSTD_FI)

add_executable(ccwstd_smoke smoke.cpp)
target_compile_definitions(ccwstd_smoke PRIVATE _CCW_TARGET_CXX=2026)
target_link_libraries(ccwstd_smoke PRIVATE
    "${CCWRAP_ROOT}/watcom/lib/nt/ccw_ccwstd-xst3r.lib")
target_compile_options(ccwstd_smoke PRIVATE -xst "-fi=${TST_CCWSTD_FI}")
target_include_directories(ccwstd_smoke PRIVATE
    "${CCWRAP_ROOT}/watcom/ccwstd"
    "${CCWRAP_ROOT}/detail/llibcxx03/include")

enable_testing()
add_test(NAME ccwstd_smoke COMMAND ccwstd_smoke)
