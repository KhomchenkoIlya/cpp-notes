#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD="$ROOT/build"
SAN="$ROOT/build-sanitize"
CXX="${CXX:-c++}"

rm -rf "$BUILD" "$SAN"

cmake -S "$ROOT" -B "$BUILD" -DCMAKE_BUILD_TYPE=Debug
cmake --build "$BUILD"
ctest --test-dir "$BUILD" --output-on-failure
"$BUILD/course_catalog_pipeline"

mkdir -p "$SAN"
"$CXX" \
    -std=c++17 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -fsanitize=address,undefined \
    -fno-omit-frame-pointer \
    "$ROOT/course_catalog_pipeline.cpp" \
    -o "$SAN/course_catalog_pipeline_sanitize"

ASAN_OPTIONS=detect_leaks=1 \
UBSAN_OPTIONS=print_stacktrace=1 \
    "$SAN/course_catalog_pipeline_sanitize" \
    >"$SAN/run.txt" 2>&1

grep -q 'COURSE_CATALOG_PIPELINE=PASS' "$SAN/run.txt"

echo "SANITIZER_SAFE_RUN=PASS"
echo "COURSE_CATALOG_CHECKS=PASS"
