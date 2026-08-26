#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD="$ROOT/build"
SAN="$ROOT/build-sanitize"
NEG="$ROOT/negative-build"
CXX="${CXX:-c++}"

rm -rf "$BUILD" "$SAN" "$NEG"
mkdir -p "$NEG"

cmake -S "$ROOT" -B "$BUILD" -DCMAKE_BUILD_TYPE=Debug
cmake --build "$BUILD"
ctest --test-dir "$BUILD" --output-on-failure
"$BUILD/callback_registry_lab"

set +e
"$CXX" -std=c++17 -Wall -Wextra -Wpedantic \
    "$ROOT/move_only_function_fail.cpp" \
    -o "$NEG/move_only_function_fail" \
    >"$NEG/move_only_function_fail.txt" 2>&1
rc=$?
set -e

if [[ "$rc" -eq 0 ]]; then
    echo "NEGATIVE_MOVE_ONLY_STD_FUNCTION=FAIL"
    exit 1
fi

echo "NEGATIVE_MOVE_ONLY_STD_FUNCTION=PASS"

mkdir -p "$SAN"
"$CXX" \
    -std=c++17 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -fsanitize=address,undefined \
    -fno-omit-frame-pointer \
    "$ROOT/callback_registry_lab.cpp" \
    -o "$SAN/callback_registry_lab_sanitize"

ASAN_OPTIONS=detect_leaks=1 \
UBSAN_OPTIONS=print_stacktrace=1 \
    "$SAN/callback_registry_lab_sanitize" \
    >"$SAN/run.txt" 2>&1

grep -q 'CALLBACK_REGISTRY_LAB=PASS' "$SAN/run.txt"

echo "SANITIZER_SAFE_RUN=PASS"
echo "CALLBACK_REGISTRY_CHECKS=PASS"
