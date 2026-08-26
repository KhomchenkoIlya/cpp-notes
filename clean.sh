#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD="$ROOT/build"

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
                printf "${BLUE}Отменено пользователем. Ничего не изменено.${RESET}\n"
                return 1
                ;;
            *) printf "${RED}Введите y или n.${RESET}\n" ;;
        esac
    done
}

printf "${BLUE}========================================${RESET}\n"
printf "${BLUE} C++ — очистка build/${RESET}\n"
printf "${BLUE}========================================${RESET}\n"
printf "${GREEN}Будет сделано:${RESET}\n"
printf "  • очищено содержимое: ${BLUE}%s${RESET}\n" "$BUILD"
printf "  • сама папка build/ останется на месте\n"
printf "  • исходники .tex, C++-код, policy.txt, sources/ и Git не затрагиваются\n"
printf "${YELLOW}Все существующие файлы внутри build/ будут удалены.${RESET}\n"

if ! confirm; then
    exit 0
fi

mkdir -p "$BUILD"
find "$BUILD" -mindepth 1 -delete

printf "${GREEN}CLEAN=PASS${RESET}\n"
printf "${GREEN}Папка build/ очищена: %s${RESET}\n" "$BUILD"
