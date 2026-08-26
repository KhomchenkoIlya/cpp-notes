#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GENERATED="$ROOT/generated"

RED=$'\033[1;31m'
GREEN=$'\033[1;32m'
YELLOW=$'\033[1;33m'
BLUE=$'\033[1;34m'
RESET=$'\033[0m'

printf '%s========================================%s\n' "$BLUE" "$RESET"
printf '%s Эксперимент 01 — compilation pipeline%s\n' "$BLUE" "$RESET"
printf '%s========================================%s\n' "$BLUE" "$RESET"
echo
printf '%sБудет сделано:%s\n' "$YELLOW" "$RESET"
echo "  1. preprocessing hello.cpp -> hello.ii;"
echo "  2. генерация assembly -> hello.s;"
echo "  3. создание object file -> hello.o;"
echo "  4. linking -> executable hello;"
echo "  5. запуск программы."
echo
printf '%sСтарое содержимое generated/ будет удалено.%s\n' "$RED" "$RESET"
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

command -v g++ >/dev/null 2>&1 || {
    printf '%sg++ не найден.%s\n' "$RED" "$RESET"
    exit 1
}

mkdir -p "$GENERATED"
find "$GENERATED" -mindepth 1 -maxdepth 1 -delete

printf '\n%s[1/5] Preprocessing%s\n' "$BLUE" "$RESET"
# -E останавливает g++ сразу после preprocessing.
g++ -std=c++17 -E "$ROOT/hello.cpp" -o "$GENERATED/hello.ii"

printf '%s[2/5] Assembly%s\n' "$BLUE" "$RESET"
# -S просит получить текст assembly и не идти дальше.
g++ -S "$GENERATED/hello.ii" -o "$GENERATED/hello.s"

printf '%s[3/5] Object file%s\n' "$BLUE" "$RESET"
# -c собирает объектный файл без linking.
g++ -c "$GENERATED/hello.s" -o "$GENERATED/hello.o"

printf '%s[4/5] Linking%s\n' "$BLUE" "$RESET"
# Здесь object file связывается с нужным C++ runtime/library.
g++ "$GENERATED/hello.o" -o "$GENERATED/hello"

printf '%s[5/5] Run%s\n' "$BLUE" "$RESET"
"$GENERATED/hello"

echo
printf '%sРазмеры результатов:%s\n' "$GREEN" "$RESET"
printf '  hello.ii: %s строк\n' "$(wc -l < "$GENERATED/hello.ii")"
printf '  hello.s:  %s строк\n' "$(wc -l < "$GENERATED/hello.s")"
printf '  hello.o:  %s байт\n' "$(stat -c%s "$GENERATED/hello.o")"
printf '  hello:    %s байт\n' "$(stat -c%s "$GENERATED/hello")"

if command -v file >/dev/null 2>&1; then
    echo
    file "$ROOT/hello.cpp"
    file "$GENERATED/hello.o"
    file "$GENERATED/hello"
fi

printf '\n%sEXPERIMENT_01=PASS%s\n' "$GREEN" "$RESET"
