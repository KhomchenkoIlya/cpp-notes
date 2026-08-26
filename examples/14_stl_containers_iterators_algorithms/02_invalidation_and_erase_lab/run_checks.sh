#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD="$ROOT/build"
SAN="$ROOT/build-sanitize"
NEG="$ROOT/negative-build"
CXX="${CXX:-c++}"

rm -rf "$BUILD" "$SAN" "$NEG"
mkdir -p "$SAN" "$NEG"

cmake -S "$ROOT" -B "$BUILD" -DCMAKE_BUILD_TYPE=Debug
cmake --build "$BUILD"
ctest --test-dir "$BUILD" --output-on-failure
"$BUILD/invalidation_and_erase_lab"

# Safe program under ASan/UBSan.
"$CXX" \
    -std=c++17 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -fsanitize=address,undefined \
    -fno-omit-frame-pointer \
    "$ROOT/invalidation_and_erase_lab.cpp" \
    -o "$SAN/invalidation_and_erase_lab_sanitize"

ASAN_OPTIONS=detect_leaks=1 \
UBSAN_OPTIONS=print_stacktrace=1 \
    "$SAN/invalidation_and_erase_lab_sanitize" \
    >"$SAN/safe_run.txt" 2>&1

grep -q 'INVALIDATION_AND_ERASE_LAB=PASS' "$SAN/safe_run.txt"
echo "SANITIZER_SAFE_RUN=PASS"

# Expected ASan failure for a stale pointer after forced vector reallocation.
"$CXX" \
    -std=c++17 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -fsanitize=address \
    -fno-omit-frame-pointer \
    "$ROOT/invalidated_pointer_asan.cpp" \
    -o "$SAN/invalidated_pointer_asan"

set +e
ASAN_OPTIONS=detect_leaks=0:halt_on_error=1 \
    "$SAN/invalidated_pointer_asan" \
    >"$SAN/expected_asan.txt" 2>&1
asan_rc=$?
set -e

if [[ "$asan_rc" -eq 0 ]]; then
    echo "EXPECTED_INVALIDATED_POINTER_ASAN=FAIL"
    exit 1
fi

if ! grep -Eqi \
    'AddressSanitizer|heap-use-after-free|use-after-free' \
    "$SAN/expected_asan.txt"
then
    echo "EXPECTED_INVALIDATED_POINTER_ASAN=FAIL_NO_DIAGNOSTIC"
    cat "$SAN/expected_asan.txt"
    exit 1
fi

echo "EXPECTED_INVALIDATED_POINTER_ASAN=PASS"

# Expected compile failure: list iterators do not satisfy std::sort.
set +e
"$CXX" -std=c++17 -Wall -Wextra -Wpedantic \
    "$ROOT/list_sort_fail.cpp" \
    -o "$NEG/list_sort_fail" \
    >"$NEG/list_sort_fail.txt" 2>&1
compile_rc=$?
set -e

if [[ "$compile_rc" -eq 0 ]]; then
    echo "NEGATIVE_LIST_STD_SORT=FAIL"
    exit 1
fi

echo "NEGATIVE_LIST_STD_SORT=PASS"
echo "INVALIDATION_ERASE_CHECKS=PASS"
