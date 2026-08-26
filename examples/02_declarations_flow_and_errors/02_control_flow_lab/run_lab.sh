#!/usr/bin/env bash
set -u

LAB_ROOT="/home/ilya/LATEX/projects/C++/examples/02_declarations_flow_and_errors/02_control_flow_lab"
BUILD="$LAB_ROOT/build"
GENERATED="$LAB_ROOT/generated"
CXX="${CXX:-g++}"

RED=$'\033[1;31m'
GREEN=$'\033[1;32m'
YELLOW=$'\033[1;33m'
BLUE=$'\033[1;34m'
RESET=$'\033[0m'

printf '%sControl flow + diagnostics lab%s\n' "$BLUE" "$RESET"
printf '%sБудут пересозданы только build/ и generated/ внутри лаборатории.%s\n' "$YELLOW" "$RESET"
printf '%sФайлы проекта и Git не изменяются; UB-кейс запускается только под UBSan.%s\n' "$RED" "$RESET"
printf '%sПродолжить? [y/n]: %s' "$YELLOW" "$RESET"
IFS= read -r answer
case "$answer" in
    y|Y) ;;
    *) printf '%sОтменено. Ничего не выполнено.%s\n' "$BLUE" "$RESET"; exit 0 ;;
esac

command -v cmake >/dev/null 2>&1 || { echo "cmake not found"; exit 1; }
command -v "$CXX" >/dev/null 2>&1 || { echo "$CXX not found"; exit 1; }
mkdir -p "$BUILD" "$GENERATED"
find "$BUILD" -mindepth 1 -delete
find "$GENERATED" -mindepth 1 -delete

failures=0
cmake -S "$LAB_ROOT" -B "$BUILD" -DCMAKE_CXX_COMPILER="$CXX" || failures=$((failures + 1))
cmake --build "$BUILD" || failures=$((failures + 1))
ctest --test-dir "$BUILD" --output-on-failure || failures=$((failures + 1))

set +e
"$BUILD/logic_error" >"$GENERATED/logic_error.out" 2>&1
logic_status=$?
set -e
if [[ $logic_status -ne 0 ]] && grep -q 'expected=15 actual=10' "$GENERATED/logic_error.out"; then
    printf '%sLOGIC_ERROR_DEMO=PASS (expected nonzero exit)%s\n' "$GREEN" "$RESET"
else
    printf '%sLOGIC_ERROR_DEMO=FAIL%s\n' "$RED" "$RESET"
    failures=$((failures + 1))
fi

set +e
"$CXX" -std=c++17 -Wall -Wextra -Wpedantic "$LAB_ROOT/cases/warning_assignment.cpp" \
    -o "$GENERATED/warning_assignment" >"$GENERATED/warning.log" 2>&1
warning_status=$?
set -e
if [[ $warning_status -eq 0 ]] && grep -qi 'warning:.*assignment' "$GENERATED/warning.log"; then
    printf '%sEXPECTED_WARNING=PASS%s\n' "$GREEN" "$RESET"
else
    printf '%sEXPECTED_WARNING=FAIL%s\n' "$RED" "$RESET"
    failures=$((failures + 1))
fi

set +e
"$CXX" -std=c++17 -Wall -Wextra -Wpedantic "$LAB_ROOT/cases/compile_error.cpp" \
    -o "$GENERATED/compile_error" >"$GENERATED/compile_error.log" 2>&1
compile_status=$?
set -e
if [[ $compile_status -ne 0 ]] && grep -Eqi 'read-only|const|assignment' "$GENERATED/compile_error.log"; then
    printf '%sEXPECTED_COMPILE_ERROR=PASS%s\n' "$GREEN" "$RESET"
else
    printf '%sEXPECTED_COMPILE_ERROR=FAIL%s\n' "$RED" "$RESET"
    failures=$((failures + 1))
fi

set +e
"$CXX" -std=c++17 -Wall -Wextra -Wpedantic -fsanitize=undefined \
    -fno-sanitize-recover=undefined "$LAB_ROOT/cases/ub_signed_overflow.cpp" \
    -o "$GENERATED/ub_signed_overflow" >"$GENERATED/ubsan_build.log" 2>&1
ubsan_build_status=$?
if [[ $ubsan_build_status -eq 0 ]]; then
    printf '2147483647\n' | "$GENERATED/ub_signed_overflow" >"$GENERATED/ubsan.log" 2>&1
    ubsan_run_status=$?
else
    ubsan_run_status=0
fi
set -e
if [[ $ubsan_build_status -eq 0 && $ubsan_run_status -ne 0 ]] \
   && grep -qi 'signed integer overflow' "$GENERATED/ubsan.log"; then
    printf '%sUBSAN_EXPECTED_FAILURE=PASS%s\n' "$GREEN" "$RESET"
elif [[ $ubsan_build_status -ne 0 ]]; then
    printf '%sUBSAN=UNSUPPORTED%s\n' "$YELLOW" "$RESET"
else
    printf '%sUBSAN_EXPECTED_FAILURE=FAIL%s\n' "$RED" "$RESET"
    failures=$((failures + 1))
fi

if [[ $failures -ne 0 ]]; then
    printf '%sLAB=FAIL failures=%d%s\n' "$RED" "$failures" "$RESET"
    exit 1
fi
printf '%sLAB=PASS%s\n' "$GREEN" "$RESET"
