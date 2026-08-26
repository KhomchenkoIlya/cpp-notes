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
"$BUILD/cpp20_feature_tour"

set +e
"$CXX" -std=c++20 -Wall -Wextra -Wpedantic \
    "$ROOT/designated_order_fail.cpp" \
    -o "$NEG/designated_order_fail" \
    > "$NEG/designated_order_fail.txt" 2>&1
negative_rc=$?
set -e

[[ "$negative_rc" -ne 0 ]] || { echo "NEGATIVE_DESIGNATED_ORDER=FAIL"; exit 1; }
echo "NEGATIVE_DESIGNATED_ORDER=PASS"

"$CXX" -std=c++20 -Wall -Wextra -Wpedantic \
    -fsanitize=address,undefined -fno-omit-frame-pointer \
    "$ROOT/cpp20_feature_tour.cpp" \
    -o "$SAN/cpp20_feature_tour_sanitize"

ASAN_OPTIONS=detect_leaks=1 UBSAN_OPTIONS=print_stacktrace=1 \
    "$SAN/cpp20_feature_tour_sanitize" \
    > "$SAN/run.txt" 2>&1

grep -q 'CPP20_FEATURE_TOUR=PASS' "$SAN/run.txt"

echo "SANITIZER_SAFE_RUN=PASS"
echo "CPP20_FEATURE_TOUR_CHECKS=PASS"
