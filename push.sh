#!/usr/bin/env bash
set -euo pipefail

PROJECT="$HOME/LATEX/projects/C++"

RED=$'\033[1;31m'
GREEN=$'\033[1;32m'
YELLOW=$'\033[1;33m'
BLUE=$'\033[1;34m'
RESET=$'\033[0m'

die() {
    printf '%sОшибка:%s %s\n' "$RED" "$RESET" "$1" >&2
    exit 1
}

confirm() {
    local answer
    printf '%sПродолжить? [y/n]: %s' "$YELLOW" "$RESET"
    IFS= read -r answer
    case "$answer" in
        y|Y)
            return 0
            ;;
        n|N|"")
            printf '%sОтменено пользователем. Git не изменён.%s\n' "$BLUE" "$RESET"
            exit 0
            ;;
        *)
            printf '%sОжидался ответ y или n. Ничего не выполнено.%s\n' "$RED" "$RESET"
            exit 1
            ;;
    esac
}

[[ -d "$PROJECT" ]] || die "проект не найден: $PROJECT"

REPO="$(git -C "$PROJECT" rev-parse --show-toplevel 2>/dev/null)" \
    || die "папка проекта не находится внутри Git-репозитория"

PREFIX="$(git -C "$PROJECT" rev-parse --show-prefix 2>/dev/null)"
BRANCH="$(git -C "$PROJECT" branch --show-current)"

EXPECTED_REPO="$HOME/LATEX/projects"
EXPECTED_PREFIX="C++/"

[[ "$REPO" == "$EXPECTED_REPO" ]] \
    || die "неожиданный корень Git: $REPO"

[[ "$PREFIX" == "$EXPECTED_PREFIX" ]] \
    || die "неожиданный Git-prefix проекта: '$PREFIX'"

[[ -n "$BRANCH" ]] \
    || die "HEAD находится в detached state; автоматический push запрещён"

git -C "$REPO" remote get-url origin >/dev/null 2>&1 \
    || die "remote origin не настроен"

# Критическая проверка:
# push.sh не должен случайно включить в наш коммит уже staged-файлы
# из соседних LaTeX-проектов.
OUTSIDE_STAGED="$(
    git -C "$REPO" diff --cached --name-only |
    while IFS= read -r path; do
        [[ -z "$path" ]] && continue
        if [[ "$path" != "$EXPECTED_PREFIX"* ]]; then
            printf '%s\n' "$path"
        fi
    done
)"

if [[ -n "$OUTSIDE_STAGED" ]]; then
    printf '%s========================================%s\n' "$RED" "$RESET"
    printf '%s PUSH ЗАБЛОКИРОВАН%s\n' "$RED" "$RESET"
    printf '%s========================================%s\n' "$RED" "$RESET"
    echo
    printf '%sВ staging уже есть файлы ИЗ ДРУГИХ ПРОЕКТОВ:%s\n' "$RED" "$RESET"
    printf '%s\n' "$OUTSIDE_STAGED"
    echo
    printf '%sСкрипт ничего не менял.%s\n' "$YELLOW" "$RESET"
    printf '%sСначала разберись с этими staged-изменениями вручную.%s\n' "$YELLOW" "$RESET"
    exit 1
fi

printf '%s========================================%s\n' "$BLUE" "$RESET"
printf '%s C++ — безопасный commit и push%s\n' "$BLUE" "$RESET"
printf '%s========================================%s\n' "$BLUE" "$RESET"
echo
printf '%sОбщий Git-репозиторий:%s %s\n' "$GREEN" "$RESET" "$REPO"
printf '%sНаш проект:%s          %s\n' "$GREEN" "$RESET" "$PROJECT"
printf '%sGit-prefix:%s          %s\n' "$GREEN" "$RESET" "$PREFIX"
printf '%sВетка:%s               %s\n' "$GREEN" "$RESET" "$BRANCH"
printf '%sRemote:%s              %s\n' "$GREEN" "$RESET" "$(git -C "$REPO" remote get-url origin)"
echo
printf '%sИзменения ТОЛЬКО внутри C++/:%s\n' "$YELLOW" "$RESET"
git -C "$REPO" status --short -- "$EXPECTED_PREFIX" || true
echo

printf '%sЕсли продолжить, будет сделано:%s\n' "$YELLOW" "$RESET"
echo "  1. В staging будут добавлены изменения ТОЛЬКО из:"
echo "       $REPO/$EXPECTED_PREFIX"
echo "  2. Соседние проекты в ~/LATEX/projects не будут добавлены."
echo "  3. Будет выполнен git diff --cached --check для C++/."
echo "  4. Если есть изменения, скрипт запросит имя коммита."
echo "  5. Коммит будет содержать только staged-файлы C++/."
echo "  6. Затем текущая ветка '$BRANCH' будет отправлена в origin."
echo
printf '%sВажно:%s commit создаётся в ОБЩЕМ репозитории latex-projects,\n' "$RED" "$RESET"
echo "но его содержимое будет ограничено файлами проекта C++/."
echo

confirm

printf '\n%sДобавляю в staging только C++/...%s\n' "$BLUE" "$RESET"
git -C "$REPO" add -A -- "$EXPECTED_PREFIX"

# Повторяем защитную проверку уже после git add.
OUTSIDE_STAGED_AFTER="$(
    git -C "$REPO" diff --cached --name-only |
    while IFS= read -r path; do
        [[ -z "$path" ]] && continue
        if [[ "$path" != "$EXPECTED_PREFIX"* ]]; then
            printf '%s\n' "$path"
        fi
    done
)"

if [[ -n "$OUTSIDE_STAGED_AFTER" ]]; then
    printf '%sОшибка безопасности: после git add обнаружены staged-файлы вне C++/.%s\n' \
        "$RED" "$RESET"
    printf '%s\n' "$OUTSIDE_STAGED_AFTER"
    echo
    echo "Коммит НЕ выполнен."
    echo "Staging автоматически не очищается, чтобы не уничтожить чужую работу."
    exit 1
fi

printf '%sПроверяю whitespace-ошибки...%s\n' "$BLUE" "$RESET"
git -C "$REPO" diff --cached --check -- "$EXPECTED_PREFIX"

if git -C "$REPO" diff --cached --quiet -- "$EXPECTED_PREFIX"; then
    printf '%sНовых изменений C++/ для коммита нет.%s\n' "$YELLOW" "$RESET"
else
    echo
    printf '%sФайлы, которые войдут в коммит:%s\n' "$GREEN" "$RESET"
    git -C "$REPO" diff --cached --name-status -- "$EXPECTED_PREFIX"
    echo

    printf '%sВведите название коммита.%s\n' "$YELLOW" "$RESET"
    echo "Формат: русский текст, слова через нижние_подчёркивания."
    printf '> '
    IFS= read -r COMMIT_MESSAGE

    [[ -n "$COMMIT_MESSAGE" ]] \
        || die "пустое название коммита"

    if [[ "$COMMIT_MESSAGE" =~ [[:space:]] ]]; then
        die "в названии коммита есть пробелы; используй нижние_подчёркивания"
    fi

    printf '\n%sСоздаю коммит...%s\n' "$BLUE" "$RESET"
    git -C "$REPO" commit -m "$COMMIT_MESSAGE"
fi

echo
printf '%sОтправляю ветку %s в origin...%s\n' "$BLUE" "$BRANCH" "$RESET"
git -C "$REPO" push origin "$BRANCH"

echo
printf '%s========================================%s\n' "$GREEN" "$RESET"
printf '%s PUSH=PASS%s\n' "$GREEN" "$RESET"
printf '%s========================================%s\n' "$GREEN" "$RESET"
echo "REPO=$REPO"
echo "PROJECT=$PROJECT"
echo "PREFIX=$PREFIX"
echo "BRANCH=$BRANCH"
echo "HEAD=$(git -C "$REPO" rev-parse HEAD)"
echo
printf '%sСтатус C++/ после операции:%s\n' "$GREEN" "$RESET"
git -C "$REPO" status --short -- "$EXPECTED_PREFIX" || true
