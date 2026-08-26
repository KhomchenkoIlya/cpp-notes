#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD="$ROOT/build"
CXX="${CXX:-c++}"

cmake -S "$ROOT" -B "$BUILD" -DCMAKE_BUILD_TYPE=Debug
cmake --build "$BUILD"
ctest --test-dir "$BUILD" --output-on-failure
"$BUILD/ownership_factory_lab"

mkdir -p "$ROOT/negative-build"
DIAG="$ROOT/negative-build/copy_unique_fail.txt"

set +e
"$CXX" -std=c++17 -Wall -Wextra -Wpedantic \
    "$ROOT/copy_unique_fail.cpp" \
    -o "$ROOT/negative-build/copy_unique_fail" \
    >"$DIAG" 2>&1
rc=$?
set -e

if [[ "$rc" -eq 0 ]]; then
    echo "NEGATIVE_UNIQUE_COPY=FAIL"
    exit 1
fi

echo "NEGATIVE_UNIQUE_COPY=PASS"
echo "NEGATIVE_DIAGNOSTIC=$DIAG"
echo "OWNERSHIP_FACTORY_CHECKS=PASS"
