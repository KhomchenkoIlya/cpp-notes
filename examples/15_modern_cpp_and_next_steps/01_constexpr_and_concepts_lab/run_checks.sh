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
"$BUILD/constexpr_and_concepts_lab"

set +e
"$CXX" -std=c++20 -Wall -Wextra -Wpedantic \
    "$ROOT/concept_rejection_fail.cpp" \
    -o "$NEG/concept_rejection_fail" \
    > "$NEG/concept_rejection_fail.txt" 2>&1
concept_rc=$?

"$CXX" -std=c++20 -Wall -Wextra -Wpedantic \
    "$ROOT/consteval_runtime_fail.cpp" \
    -o "$NEG/consteval_runtime_fail" \
    > "$NEG/consteval_runtime_fail.txt" 2>&1
consteval_rc=$?
set -e

[[ "$concept_rc" -ne 0 ]] || { echo "NEGATIVE_CONCEPT_REJECTION=FAIL"; exit 1; }
[[ "$consteval_rc" -ne 0 ]] || { echo "NEGATIVE_CONSTEVAL_RUNTIME=FAIL"; exit 1; }

echo "NEGATIVE_CONCEPT_REJECTION=PASS"
echo "NEGATIVE_CONSTEVAL_RUNTIME=PASS"

"$CXX" -std=c++20 -Wall -Wextra -Wpedantic \
    -fsanitize=address,undefined -fno-omit-frame-pointer \
    "$ROOT/constexpr_and_concepts_lab.cpp" \
    -o "$SAN/constexpr_and_concepts_lab_sanitize"

ASAN_OPTIONS=detect_leaks=1 UBSAN_OPTIONS=print_stacktrace=1 \
    "$SAN/constexpr_and_concepts_lab_sanitize" \
    > "$SAN/run.txt" 2>&1

grep -q 'CONSTEXPR_CONCEPTS_LAB=PASS' "$SAN/run.txt"

echo "SANITIZER_SAFE_RUN=PASS"
echo "CONSTEXPR_CONCEPTS_CHECKS=PASS"
