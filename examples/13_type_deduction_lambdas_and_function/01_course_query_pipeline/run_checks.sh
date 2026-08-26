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
"$BUILD/course_query_pipeline"

set +e
"$CXX" -std=c++17 -Wall -Wextra -Wpedantic \
    "$ROOT/missing_capture_fail.cpp" \
    -o "$NEG/missing_capture_fail" \
    >"$NEG/missing_capture_fail.txt" 2>&1
rc=$?
set -e

if [[ "$rc" -eq 0 ]]; then
    echo "NEGATIVE_MISSING_CAPTURE=FAIL"
    exit 1
fi

echo "NEGATIVE_MISSING_CAPTURE=PASS"

mkdir -p "$SAN"
"$CXX" \
    -std=c++17 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -fsanitize=address,undefined \
    -fno-omit-frame-pointer \
    "$ROOT/course_query_pipeline.cpp" \
    -o "$SAN/course_query_pipeline_sanitize"

ASAN_OPTIONS=detect_leaks=1 \
UBSAN_OPTIONS=print_stacktrace=1 \
    "$SAN/course_query_pipeline_sanitize" \
    >"$SAN/run.txt" 2>&1

grep -q 'COURSE_QUERY_PIPELINE=PASS' "$SAN/run.txt"

echo "SANITIZER_SAFE_RUN=PASS"
echo "COURSE_QUERY_CHECKS=PASS"
