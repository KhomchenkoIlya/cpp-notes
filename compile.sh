#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD="$ROOT/build"
PDF="$BUILD/c++.pdf"

RED='\033[1;31m'
GREEN='\033[1;32m'
YELLOW='\033[1;33m'
BLUE='\033[1;34m'
RESET='\033[0m'

confirm() {
    local answer
    while true; do
        printf "${YELLOW}Продолжить? [y/n]: ${RESET}"
        IFS= read -r answer
        case "$answer" in
            y|Y) return 0 ;;
            n|N|'')
                printf "${BLUE}Отменено пользователем. Сборка не запускалась.${RESET}\n"
                return 1
                ;;
            *) printf "${RED}Введите y или n.${RESET}\n" ;;
        esac
    done
}

printf "${BLUE}========================================${RESET}\n"
printf "${BLUE} C++ — сборка конспекта${RESET}\n"
printf "${BLUE}========================================${RESET}\n"
printf "${GREEN}Будет сделано:${RESET}\n"
printf "  • очищено прежнее содержимое build/\n"
printf "  • main.tex дважды собран XeLaTeX\n"
printf "  • все aux/log/toc/out и PDF останутся только в build/\n"
printf "  • итоговый PDF: ${BLUE}%s${RESET}\n" "$PDF"
printf "${YELLOW}Исходники проекта изменяться не будут.${RESET}\n"

if ! confirm; then
    exit 0
fi

if ! command -v xelatex >/dev/null 2>&1; then
    printf "${RED}Ошибка: XeLaTeX не найден.${RESET}\n" >&2
    exit 1
fi

cd "$ROOT"
mkdir -p "$BUILD"
find "$BUILD" -mindepth 1 -delete

for pass in 1 2; do
    printf "${BLUE}XeLaTeX: проход %d/2${RESET}\n" "$pass"
    xelatex \
        -interaction=nonstopmode \
        -halt-on-error \
        -file-line-error \
        -output-directory="$BUILD" \
        -jobname='c++' \
        main.tex
 done

if [[ ! -s "$PDF" ]]; then
    printf "${RED}Ошибка: итоговый PDF не найден или пуст.${RESET}\n" >&2
    exit 1
fi

printf "${GREEN}========================================${RESET}\n"
printf "${GREEN}COMPILE=PASS${RESET}\n"
printf "${GREEN}PDF=%s${RESET}\n" "$PDF"
printf "${GREEN}SIZE=%s${RESET}\n" "$(stat -c '%s' "$PDF")"
printf "${GREEN}========================================${RESET}\n"
