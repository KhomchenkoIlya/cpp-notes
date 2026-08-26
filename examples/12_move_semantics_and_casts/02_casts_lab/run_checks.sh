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
"$BUILD/casts_lab"

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

compile_must_fail "modify_const_fail.cpp" "NEGATIVE_MODIFY_CONST"
compile_must_fail \
    "nonpolymorphic_dynamic_cast_fail.cpp" \
    "NEGATIVE_NONPOLYMORPHIC_DYNAMIC_CAST"

"$CXX" -std=c++17 -Wall -Wextra -Wpedantic \
    "$ROOT/c_style_const_removal_compiles.cpp" \
    -o "$NEG/c_style_const_removal_compiles" \
    >"$NEG/c_style_const_removal_compiles.txt" 2>&1

echo "C_STYLE_CONST_REMOVAL_COMPILES=PASS"

"$CXX" \
    -std=c++17 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -fsanitize=address,undefined \
    -fno-omit-frame-pointer \
    "$ROOT/casts_lab.cpp" \
    -o "$SAN.tmp"

mkdir -p "$SAN"
mv "$SAN.tmp" "$SAN/casts_lab_sanitize"

ASAN_OPTIONS=detect_leaks=1 \
UBSAN_OPTIONS=print_stacktrace=1 \
    "$SAN/casts_lab_sanitize" \
    >"$SAN/run.txt" 2>&1

grep -q 'CASTS_LAB=PASS' "$SAN/run.txt"

echo "SANITIZER_SAFE_RUN=PASS"
echo "CASTS_CHECKS=PASS"
