# ccwrap ccwstd smoke build: compiles libcxx03 relocated into namespace `ccwstd`.
# Only watcom/ccwstd/ (+ detail/llibcxx03/include) is on the include path
# -- watcom/std/ is NOT --
# and watcom/ccwstd/ccwrap_header.h is force-included to select the ccwstd namespace.
cmake_minimum_required(VERSION 3.20)
project(ccwrap_ccwstd_smoke CXX)

get_filename_component(CCWRAP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../.." ABSOLUTE)

# wcl386 mis-parses a leading '/' inside -fi=, so hand it a native (backslash) path.
file(TO_NATIVE_PATH "${CCWRAP_ROOT}/watcom/ccwstd/ccwrap_header.h" TST_CCWSTD_FI)

# The libcxx03 out-of-line definitions (thread/locale/filesystem) come from the PREBUILT
# ccwstd-variant static library (watcom/lib/win32-ccwstd-xst/libccwcxx.lib -- `gen full`), built once by
# watcom/lib/CMakeLists.txt. Build that first: `cmake --preset watcom` in
# ../../../watcom/lib.
add_executable(ccwstd_smoke smoke.cpp)
target_compile_definitions(ccwstd_smoke PRIVATE _CCW_TARGET_CXX=2026)
target_link_libraries(ccwstd_smoke PRIVATE
    "${CCWRAP_ROOT}/watcom/lib/win32-ccwstd-xst/libccwcxx.lib")
target_compile_options(ccwstd_smoke PRIVATE -xs "-fi=${TST_CCWSTD_FI}")
target_include_directories(ccwstd_smoke PRIVATE
    "${CCWRAP_ROOT}/watcom/ccwstd"
    "${CCWRAP_ROOT}/detail/llibcxx03/include")

enable_testing()
add_test(NAME ccwstd_smoke COMMAND ccwstd_smoke)
