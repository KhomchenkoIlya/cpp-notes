#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD="$ROOT/build"

RED=$'\033[1;31m'
GREEN=$'\033[1;32m'
YELLOW=$'\033[1;33m'
BLUE=$'\033[1;34m'
RESET=$'\033[0m'

printf '%s========================================%s\n' "$BLUE" "$RESET"
printf '%s Эксперимент 02 — простые абстракции%s\n' "$BLUE" "$RESET"
printf '%s========================================%s\n' "$BLUE" "$RESET"
echo
printf '%sБудет сделано:%s\n' "$YELLOW" "$RESET"
echo "  1. CMake создаст конфигурацию в build/;"
echo "  2. программа basic_abstractions будет собрана;"
echo "  3. программа будет запущена."
echo
printf '%sСтарое содержимое локального build/ будет удалено.%s\n' "$RED" "$RESET"
printf '%sПродолжить? [y/n]: %s' "$YELLOW" "$RESET"
IFS= read -r answer

case "$answer" in
    y|Y) ;;
    n|N|"")
        printf '%sОтменено. Ничего не выполнено.%s\n' "$BLUE" "$RESET"
        exit 0
        ;;
    *)
        printf '%sОжидался y или n.%s\n' "$RED" "$RESET"
        exit 1
        ;;
esac

command -v cmake >/dev/null 2>&1 || {
    printf '%sCMake не найден.%s\n' "$RED" "$RESET"
    echo "Ubuntu: sudo apt update && sudo apt install -y cmake"
    exit 1
}

mkdir -p "$BUILD"
find "$BUILD" -mindepth 1 -maxdepth 1 -delete

printf '\n%s[1/3] Configure%s\n' "$BLUE" "$RESET"
# -S указывает исходники, -B — отдельную папку сборки.
cmake -S "$ROOT" -B "$BUILD"

printf '%s[2/3] Build%s\n' "$BLUE" "$RESET"
cmake --build "$BUILD"

printf '%s[3/3] Run%s\n' "$BLUE" "$RESET"
"$BUILD/basic_abstractions"

printf '\n%sEXPERIMENT_02=PASS%s\n' "$GREEN" "$RESET"
