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
"$BUILD/move_ownership_pipeline"

compile_must_fail() {
    local src="$1"
    local name="$2"
    local diag="$NEG/${name}.txt"

    set +e
    "$CXX" -std=c++17 -Wall -Wextra -Wpedantic \
        "$ROOT/$src" \
        -o "$NEG/$name" \
        >"$diag" 2>&1
    local rc=$?
    set -e

    if [[ "$rc" -eq 0 ]]; then
        echo "${name}=FAIL"
        return 1
    fi

    echo "${name}=PASS"
}

compile_must_fail "copy_packet_fail.cpp" "NEGATIVE_COPY_PACKET"
compile_must_fail "move_from_const_fail.cpp" "NEGATIVE_MOVE_FROM_CONST"

if "$CXX" --version >/dev/null 2>&1; then
    "$CXX" \
        -std=c++17 \
        -Wall \
        -Wextra \
        -Wpedantic \
        -fsanitize=address,undefined \
        -fno-omit-frame-pointer \
        "$ROOT/move_ownership_pipeline.cpp" \
        -o "$SAN.tmp"

    mkdir -p "$SAN"
    mv "$SAN.tmp" "$SAN/move_ownership_pipeline_sanitize"

    ASAN_OPTIONS=detect_leaks=1 \
    UBSAN_OPTIONS=print_stacktrace=1 \
        "$SAN/move_ownership_pipeline_sanitize" \
        >"$SAN/run.txt" 2>&1

    grep -q 'MOVE_OWNERSHIP_PIPELINE=PASS' "$SAN/run.txt"
    echo "SANITIZER_SAFE_RUN=PASS"
fi

echo "MOVE_OWNERSHIP_CHECKS=PASS"
