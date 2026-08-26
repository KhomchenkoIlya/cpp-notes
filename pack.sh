#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DOWNLOADS="$HOME/Загрузки"
STAMP="$(date +%Y%m%d_%H%M%S)"
ARCHIVE="$DOWNLOADS/C++_for_chat_${STAMP}.zip"

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
                printf "${BLUE}Отменено пользователем. Архив не создавался.${RESET}\n"
                return 1
                ;;
            *) printf "${RED}Введите y или n.${RESET}\n" ;;
        esac
    done
}

printf "${BLUE}========================================${RESET}\n"
printf "${BLUE} C++ — упаковка проекта для чата${RESET}\n"
printf "${BLUE}========================================${RESET}\n"
printf "${GREEN}Будет сделано:${RESET}\n"
printf "  • создан новый ZIP в: ${BLUE}%s${RESET}\n" "$ARCHIVE"
printf "  • включены LaTeX-исходники, policy.txt, скрипты, vox.png и рабочие папки проекта\n"
printf "  • включены будущие examples/, tools/, tests/, benchmarks/, CMake-файлы, если они есть\n"
printf "  • исключены build/, .git/, тяжёлые PDF из sources/ и типичные кэши/бинарники\n"
printf "${YELLOW}Файлы проекта не удаляются и не изменяются.${RESET}\n"

if ! confirm; then
    exit 0
fi

if ! command -v zip >/dev/null 2>&1; then
    printf "${RED}Ошибка: zip не найден. Установка: sudo apt install zip unzip${RESET}\n" >&2
    exit 1
fi
if ! command -v unzip >/dev/null 2>&1; then
    printf "${RED}Ошибка: unzip не найден. Установка: sudo apt install zip unzip${RESET}\n" >&2
    exit 1
fi

mkdir -p "$DOWNLOADS"
cd "$ROOT"

zip -q -r "$ARCHIVE" . \
    -x 'build/*' \
       '.git/*' \
       'sources/*.pdf' \
       'sources/**/*.pdf' \
       '*.o' '*.obj' '*.a' '*.so' '*.dll' '*.dylib' '*.exe' \
       'a.out' \
       'CMakeFiles/*' 'cmake-build-*/*' \
       '.cache/*' '.clangd/*' \
       '__pycache__/*' '*.pyc' \
       '*.swp' '*.swo' '*~' \
       '.DS_Store' 'Thumbs.db'

unzip -tq "$ARCHIVE" >/dev/null

printf "${GREEN}========================================${RESET}\n"
printf "${GREEN}PACK=PASS${RESET}\n"
printf "${GREEN}ARCHIVE=%s${RESET}\n" "$ARCHIVE"
printf "${GREEN}SIZE=%s${RESET}\n" "$(stat -c '%s' "$ARCHIVE")"
printf "${GREEN}========================================${RESET}\n"
printf "${BLUE}Содержимое архива:${RESET}\n"
unzip -l "$ARCHIVE"
