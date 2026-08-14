cmake_minimum_required(VERSION 3.15)
project(testagg CXX)

# ccwrap テスト pass ログの集計ツール。C++03 文法の範囲で書いてあり ccwrap でも
# ビルドできるが、安定運用のため生の vc14.5 (以上) でビルドする想定。
#   cmake -S . -B build && cmake --build build --config Release
add_executable(testagg main.cpp)

if(MSVC)
    target_compile_definitions(testagg PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(testagg PRIVATE /W3 /EHsc)
endif()
