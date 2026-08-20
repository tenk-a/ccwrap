#!/bin/sh
#
#   Usage: ./run.sh [preset] [extra cmake args...]
#
#
#     ./run.sh gcc-latest                 g++ -std=c++23
#
#     cd c_test && cmake --preset gcc && cmake --build --preset gcc && ctest --preset gcc
set -e
cd "$(dirname "$0")"

preset="${1:-gcc}"
if [ $# -gt 0 ]; then shift; fi

cmake --preset "$preset" "$@"
cmake --build --preset "$preset"
ctest --preset "$preset"
