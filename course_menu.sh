#!/usr/bin/env bash
set -Eeuo pipefail

PROJECT="$HOME/LATEX/projects/C++"

BOLD=$'\033[1m'
CYAN=$'\033[36m'
GREEN=$'\033[32m'
YELLOW=$'\033[33m'
RED=$'\033[31m'
RESET=$'\033[0m'

[[ -d "$PROJECT/examples" ]] || {
    echo "Не найден проект: $PROJECT" >&2
    exit 1
}

BUILD_ROOT="$PROJECT/build/experiment_menu"
mkdir -p "$BUILD_ROOT"

pause() {
    echo
    read -r -p "Enter — вернуться в меню..." _
}

ask_yes_no() {
    local prompt="$1"
    local answer

    while true; do
        read -r -p "$prompt (y/n): " answer
        case "${answer,,}" in
            y|yes|д|да) return 0 ;;
            n|no|н|нет|'') return 1 ;;
            *) echo "Введите y или n." ;;
        esac
    done
}

block_dir() {
    case "$1" in
        1) echo "01_intro_and_first_program" ;;
        2) echo "02_declarations_flow_and_errors" ;;
        3) echo "03_pointers_memory_and_arrays" ;;
        4) echo "04_functions_references_and_const" ;;
        5) echo "05_classes_and_encapsulation" ;;
        6) echo "06_constructors_destructors_and_copy" ;;
        7) echo "07_inheritance_and_polymorphism" ;;
        8) echo "08_design_solid_and_patterns" ;;
        9) echo "09_templates" ;;
        10) echo "10_exceptions_and_raii" ;;
        11) echo "11_smart_pointers" ;;
        12) echo "12_move_semantics_and_casts" ;;
        13) echo "13_type_deduction_lambdas_and_function" ;;
        14) echo "14_stl_containers_iterators_algorithms" ;;
        15) echo "15_modern_cpp_and_next_steps" ;;
        *) return 1 ;;
    esac
}

block_title() {
    case "$1" in
        1) echo "Введение и первая программа" ;;
        2) echo "Объявления, control flow и ошибки" ;;
        3) echo "Pointers, memory и arrays" ;;
        4) echo "Functions, references и const" ;;
        5) echo "Classes и encapsulation" ;;
        6) echo "Constructors, destructors и copy" ;;
        7) echo "Inheritance и polymorphism" ;;
        8) echo "SOLID и patterns" ;;
        9) echo "Templates" ;;
        10) echo "Exceptions и RAII" ;;
        11) echo "Smart pointers" ;;
        12) echo "Move semantics и casts" ;;
        13) echo "Type deduction, lambdas, std::function" ;;
        14) echo "STL containers, iterators, algorithms" ;;
        15) echo "Modern C++ после C++17" ;;
    esac
}

experiments_for_block() {
    case "$1" in
        1) printf '%s\n' "01_compilation_pipeline" "02_basic_abstractions" ;;
        2) printf '%s\n' "01_course_status" "02_control_flow_lab" ;;
        3) printf '%s\n' "01_course_inventory" "02_array_vs_vector_lab" ;;
        4) printf '%s\n' "01_course_inventory_refactored" "02_function_contract_lab" ;;
        5) printf '%s\n' "01_course_inventory_classes" "02_encapsulation_break_lab" ;;
        6) printf '%s\n' "01_copy_semantics_lab" "02_inventory_snapshot" ;;
        7) printf '%s\n' "01_polymorphism_failure_lab" "02_inheritance_vs_composition_project_lab" ;;
        8) printf '%s\n' "01_patterns_lab" "02_course_audit_patterns" ;;
        9) printf '%s\n' "generic_course_audit" "mini_generic_library" ;;
        10) printf '%s\n' "course_audit_error_boundary" "error_model_lab" ;;
        11) printf '%s\n' "01_ownership_factory_lab" "02_shared_weak_graph_lab" ;;
        12) printf '%s\n' "01_move_ownership_pipeline" "02_casts_lab" ;;
        13) printf '%s\n' "01_course_query_pipeline" "02_callback_registry_lab" ;;
        14) printf '%s\n' "01_course_catalog_pipeline" "02_invalidation_and_erase_lab" ;;
        15) printf '%s\n' "01_constexpr_and_concepts_lab" "02_cpp20_feature_tour" ;;
        *) return 1 ;;
    esac
}

experiment_info() {
    local id="$1"
    WHAT=""
    SHOWS=""
    FEATURES=""

    case "$id" in
        01_intro_and_first_program/01_compilation_pipeline)
            WHAT="Разбирает сборку hello.cpp по стадиям preprocessing → assembly → object → executable."
            SHOWS="Показывает, что одна команда компилятора скрывает несколько разных этапов."
            FEATURES="g++ -E/-S/-c; object file; linker; generated artifacts"
            ;;
        01_intro_and_first_program/02_basic_abstractions)
            WHAT="Собирает небольшую C++17-программу с базовыми стандартными типами."
            SHOWS="Показывает std::string, std::vector, std::map и первую target-based CMake-сборку."
            FEATURES="standard library; containers; CMake; warnings"
            ;;
        02_declarations_flow_and_errors/01_course_status)
            WHAT="Проверяет пары lecture/examples для курса и выводит их состояние."
            SHOWS="Показывает ветвление, цикл, filesystem-проверки и exit codes на реальных данных проекта."
            FEATURES="if/switch; loops; filesystem; exit status"
            ;;
        02_declarations_flow_and_errors/02_control_flow_lab)
            WHAT="Запускает набор корректных и намеренно ошибочных control-flow cases."
            SHOWS="Показывает if, границы циклов, short-circuit, compiler warning, compile error и UBSan diagnostic."
            FEATURES="branching; loops; short-circuit; diagnostics; UBSan"
            ;;
        03_pointers_memory_and_arrays/01_course_inventory)
            WHAT="Читает структуру курса и считает размеры лекций и файлов практики."
            SHOWS="Показывает pointers/arrays на фоне read-only utility без изменения проекта."
            FEATURES="addresses; arrays; filesystem; read-only traversal"
            ;;
        03_pointers_memory_and_arrays/02_array_vs_vector_lab)
            WHAT="Сравнивает raw array и std::vector в безопасных сценариях."
            SHOWS="Показывает bounds, capacity/invalidation и отдельные sanitizer cases для реальных memory errors."
            FEATURES="array; vector; pointer lifetime; ASan/UBSan"
            ;;
        04_functions_references_and_const/01_course_inventory_refactored)
            WHAT="Повторяет inventory из блока 03, но разбивает программу на функции."
            SHOWS="Показывает рефакторинг без изменения результата и контрактов CLI."
            FEATURES="functions; parameters; const references; decomposition"
            ;;
        04_functions_references_and_const/02_function_contract_lab)
            WHAT="Проверяет разные способы передачи и возврата значений."
            SHOWS="Показывает pass-by-value/reference, const reference, overloads, function pointer и negative compile cases."
            FEATURES="references; const; overloads; function pointer; lifetime"
            ;;
        05_classes_and_encapsulation/01_course_inventory_classes)
            WHAT="Переписывает inventory вокруг класса CourseBlock."
            SHOWS="Показывает invariant, private state и разделение class responsibilities."
            FEATURES="class; constructor; encapsulation; const methods"
            ;;
        05_classes_and_encapsulation/02_encapsulation_break_lab)
            WHAT="Сравнивает публичное изменяемое состояние с контролируемым private state."
            SHOWS="Показывает, как encapsulation защищает invariant и как compiler запрещает private/const violations."
            FEATURES="private/public; invariant; const methods; negative compile"
            ;;
        06_constructors_destructors_and_copy/01_copy_semantics_lab)
            WHAT="Наблюдает construction/destruction и разные варианты копирования объектов."
            SHOWS="Показывает deep copy, Rule of Three, Rule of Zero, self-assignment и double-free diagnostic."
            FEATURES="constructors; destructors; copy; Rule of Three/Zero; ASan"
            ;;
        06_constructors_destructors_and_copy/02_inventory_snapshot)
            WHAT="Создаёт независимые snapshots данных курса."
            SHOWS="Показывает value semantics: копия std::string/std::vector меняется независимо от оригинала."
            FEATURES="value semantics; compiler-generated copy; snapshot"
            ;;
        07_inheritance_and_polymorphism/01_polymorphism_failure_lab)
            WHAT="Сравнивает non-virtual и virtual dispatch и типичные ошибки иерархий."
            SHOWS="Показывает slicing, abstract interface, override checks, protected access и virtual destructor."
            FEATURES="inheritance; virtual; slicing; abstract class; destructor"
            ;;
        07_inheritance_and_polymorphism/02_inheritance_vs_composition_project_lab)
            WHAT="Решает одну задачу аудита двумя дизайнами: inheritance и composition."
            SHOWS="Показывает, когда runtime polymorphism оправдан, а когда composition проще."
            FEATURES="is-a/has-a; composition; runtime polymorphism; design comparison"
            ;;
        08_design_solid_and_patterns/01_patterns_lab)
            WHAT="Сравнивает прямой switch с Strategy и запускает Factory Method и Observer."
            SHOWS="Показывает назначение pattern и случай, где pattern будет лишним."
            FEATURES="Strategy; Factory Method; Observer; overengineering"
            ;;
        08_design_solid_and_patterns/02_course_audit_patterns)
            WHAT="Аудирует все 15 блоков через сменную AuditStrategy и observers."
            SHOWS="Показывает runtime policy replacement и независимую подписку на результаты."
            FEATURES="Strategy; Observer; composition root; 15-block audit"
            ;;
        09_templates/generic_course_audit)
            WHAT="Один function template запускает аудит с двумя разными reporter types."
            SHOWS="Показывает compile-time polymorphism без общей base class и проверяет 15/15 блоков."
            FEATURES="function template; duck typing; compile-time polymorphism; negative compile"
            ;;
        09_templates/mini_generic_library)
            WHAT="Запускает маленькую reusable template library."
            SHOWS="Показывает function/class templates, FixedBuffer, specialization и ошибку отсутствующего operator<."
            FEATURES="templates; specialization; generic algorithms; diagnostics"
            ;;
        10_exceptions_and_raii/course_audit_error_boundary)
            WHAT="Проверяет курс с двумя способами переноса operational failure: status и exception."
            SHOWS="Показывает exception внутри операции и единый catch на CLI boundary."
            FEATURES="exceptions; status; boundary; domain vs operational error"
            ;;
        10_exceptions_and_raii/error_model_lab)
            WHAT="Выполняет одну файловую операцию через Status и через exceptions."
            SHOWS="Показывает RAII cleanup, commit, stack unwinding и rethrow."
            FEATURES="RAII; exceptions; temporary file; unwind; rethrow"
            ;;
        11_smart_pointers/01_ownership_factory_lab)
            WHAT="Factory возвращает owning std::unique_ptr вместо raw owning pointer."
            SHOWS="Показывает unique ownership, std::move, borrowing API, reset и запрет копирования unique_ptr."
            FEATURES="unique_ptr; ownership; borrowing; polymorphic destruction"
            ;;
        11_smart_pointers/02_shared_weak_graph_lab)
            WHAT="Строит несколько ownership graph с shared_ptr и weak_ptr."
            SHOWS="Показывает shared lifetime, strong cycle, weak back-edge и weak_ptr::lock."
            FEATURES="shared_ptr; weak_ptr; ownership graph; cycle"
            ;;
        12_move_semantics_and_casts/01_move_ownership_pipeline)
            WHAT="Передаёт move-only Packet через несколько API."
            SHOWS="Показывает move ctor/assignment, std::move, std::forward, value categories и copy elision."
            FEATURES="move-only type; rvalue; forwarding; copy elision; negative compile"
            ;;
        12_move_semantics_and_casts/02_casts_lab)
            WHAT="Проверяет четыре named C++ casts на безопасных сценариях."
            SHOWS="Показывает static/dynamic/const/reinterpret_cast и границы, где compiler или runtime отвергает операцию."
            FEATURES="static_cast; dynamic_cast; const_cast; reinterpret_cast"
            ;;
        13_type_deduction_lambdas_and_function/01_course_query_pipeline)
            WHAT="Строит query pipeline по данным курса."
            SHOWS="Показывает auto, decltype, CTAD, structured bindings, generic lambda, captures и optional."
            FEATURES="type deduction; lambda; capture; optional; CTAD"
            ;;
        13_type_deduction_lambdas_and_function/02_callback_registry_lab)
            WHAT="Хранит разные callable objects в одном callback registry."
            SHOWS="Показывает std::function type erasure, mutable/weak captures, empty function и C++17 copyability restriction."
            FEATURES="std::function; lambdas; weak_ptr capture; type erasure"
            ;;
        14_stl_containers_iterators_algorithms/01_course_catalog_pipeline)
            WHAT="Обрабатывает каталог 15 блоков как STL data pipeline."
            SHOWS="Показывает vector/index, filter, sort, transform, accumulate и ordered summary."
            FEATURES="vector; unordered_map; map; algorithms; optional"
            ;;
        14_stl_containers_iterators_algorithms/02_invalidation_and_erase_lab)
            WHAT="Меняет containers и наблюдает правила invalidation."
            SHOWS="Показывает vector reallocation, erase-remove, list stability, unordered rehash и ожидаемый ASan failure."
            FEATURES="capacity; invalidation; erase-remove; list; rehash; ASan"
            ;;
        15_modern_cpp_and_next_steps/01_constexpr_and_concepts_lab)
            WHAT="Запускает C++20 compile-time contracts."
            SHOWS="Показывает constexpr, consteval, constinit, Concepts/requires и два реальных compile failures."
            FEATURES="C++20; constexpr; consteval; constinit; Concepts; requires"
            ;;
        15_modern_cpp_and_next_steps/02_cpp20_feature_tour)
            WHAT="Демонстрирует несколько C++20 library/language features на одной программе."
            SHOWS="Показывает ranges/views, span, designated initializers, <=>, erase_if и starts_with."
            FEATURES="C++20; ranges; views; span; <=>; designated init"
            ;;
        *)
            WHAT="Описание не найдено."
            SHOWS="Откройте README эксперимента."
            FEATURES=""
            ;;
    esac
}

show_info() {
    local id="$1"
    experiment_info "$id"
    echo
    echo "${BOLD}Что делает:${RESET}"
    echo "  $WHAT"
    echo
    echo "${BOLD}Что показывает:${RESET}"
    echo "  $SHOWS"
    echo
    echo "${BOLD}Особенности кода:${RESET}"
    echo "  $FEATURES"
}

display_file() {
    local file="$1"
    echo
    echo "${CYAN}${BOLD}$file${RESET}"
    echo "------------------------------------------------------------"

    if command -v batcat >/dev/null 2>&1; then
        batcat --style=numbers --paging=always "$file"
    elif command -v bat >/dev/null 2>&1; then
        bat --style=numbers --paging=always "$file"
    elif command -v less >/dev/null 2>&1 && [[ -t 1 ]]; then
        nl -ba "$file" | less -R
    else
        nl -ba "$file"
    fi
}

show_code_menu() {
    local id="$1"
    local dir="$PROJECT/examples/$id"

    show_info "$id"

    while true; do
        mapfile -t files < <(
            find "$dir" \
                -maxdepth 3 \
                -type f \
                \( -name '*.cpp' -o -name '*.hpp' -o -name '*.h' -o \
                   -name 'CMakeLists.txt' -o -name '*.sh' -o -name 'README.md' \) \
                ! -path '*/build/*' \
                ! -path '*/build-*/*' \
                ! -path '*/negative-build/*' \
                | sort
        )

        echo
        echo "${BOLD}Файлы эксперимента:${RESET}"
        local i
        for i in "${!files[@]}"; do
            printf '  %2d) %s\n' "$((i + 1))" "${files[$i]#$dir/}"
        done
        echo "   0) Назад"

        read -r -p "Выберите файл: " choice

        if [[ "$choice" == "0" ]]; then
            return
        fi

        if [[ "$choice" =~ ^[0-9]+$ ]] &&
           (( choice >= 1 && choice <= ${#files[@]} )); then
            display_file "${files[$((choice - 1))]}"
        else
            echo "Нет такого пункта."
        fi
    done
}

cmake_demo() {
    local id="$1"
    local exe="$2"
    shift 2

    local src="$PROJECT/examples/$id"
    local safe="${id//\//__}"
    local build="$BUILD_ROOT/$safe"

    rm -rf "$build"

    echo
    echo "${CYAN}[configure]${RESET}"
    cmake -S "$src" -B "$build" -DCMAKE_BUILD_TYPE=Debug

    echo
    echo "${CYAN}[build]${RESET}"
    cmake --build "$build" --parallel 2

    echo
    echo "${CYAN}[demo]${RESET}"
    "$build/$exe" "$@"

    echo
    echo "${CYAN}[CTest]${RESET}"
    ctest --test-dir "$build" --output-on-failure
}

run_local_script() {
    local id="$1"
    local script="$2"
    local dir="$PROJECT/examples/$id"

    (
        cd "$dir"
        bash "./$script"
    )
}

run_experiment() {
    local id="$1"

    case "$id" in
        01_intro_and_first_program/01_compilation_pipeline)
            local dir="$PROJECT/examples/$id"
            if [[ -f "$dir/inspect.sh" ]]; then
                (cd "$dir" && bash ./inspect.sh)
            else
                (
                    cd "$dir"
                    rm -rf generated
                    mkdir -p generated
                    echo "[preprocess] hello.ii"
                    g++ -std=c++17 -E hello.cpp -o generated/hello.ii
                    echo "[assembly] hello.s"
                    g++ -std=c++17 -S hello.cpp -o generated/hello.s
                    echo "[object] hello.o"
                    g++ -std=c++17 -c hello.cpp -o generated/hello.o
                    echo "[link] hello"
                    g++ generated/hello.o -o generated/hello
                    echo "[run]"
                    ./generated/hello
                    echo "COMPILATION_PIPELINE=PASS"
                )
            fi
            ;;
        01_intro_and_first_program/02_basic_abstractions)
            if [[ -f "$PROJECT/examples/$id/run.sh" ]]; then
                run_local_script "$id" "run.sh"
            else
                cmake_demo "$id" "basic_abstractions"
            fi
            ;;
        02_declarations_flow_and_errors/01_course_status)
            cmake_demo "$id" "course_status"
            ;;
        02_declarations_flow_and_errors/02_control_flow_lab)
            (
                cd "$PROJECT/examples/$id"
                printf 'y\n' | bash ./run_lab.sh
            )
            ;;
        03_pointers_memory_and_arrays/01_course_inventory)
            cmake_demo "$id" "course_inventory" --root "$PROJECT"
            ;;
        03_pointers_memory_and_arrays/02_array_vs_vector_lab)
            cmake_demo "$id" "array_vs_vector_lab"
            ;;
        04_functions_references_and_const/01_course_inventory_refactored)
            cmake_demo "$id" "course_inventory_refactored" --root "$PROJECT"
            ;;
        04_functions_references_and_const/02_function_contract_lab)
            cmake_demo "$id" "function_contract_lab"
            ;;
        05_classes_and_encapsulation/01_course_inventory_classes)
            cmake_demo "$id" "course_inventory_classes" --root "$PROJECT" --block 5
            ;;
        05_classes_and_encapsulation/02_encapsulation_break_lab)
            cmake_demo "$id" "encapsulation_break_lab"
            ;;
        06_constructors_destructors_and_copy/01_copy_semantics_lab)
            cmake_demo "$id" "copy_semantics_lab"
            ;;
        06_constructors_destructors_and_copy/02_inventory_snapshot)
            cmake_demo "$id" "inventory_snapshot" --root "$PROJECT"
            ;;
        07_inheritance_and_polymorphism/01_polymorphism_failure_lab)
            cmake_demo "$id" "polymorphism_failure_lab"
            ;;
        07_inheritance_and_polymorphism/02_inheritance_vs_composition_project_lab)
            cmake_demo "$id" "project_lab" --root "$PROJECT"
            ;;
        08_design_solid_and_patterns/01_patterns_lab)
            cmake_demo "$id" "patterns_lab"
            ;;
        08_design_solid_and_patterns/02_course_audit_patterns)
            cmake_demo "$id" "course_audit_patterns" --root "$PROJECT" --policy strict
            ;;
        09_templates/generic_course_audit)
            cmake_demo "$id" "generic_course_audit" --root "$PROJECT"
            ;;
        09_templates/mini_generic_library)
            cmake_demo "$id" "mini_generic_library"
            ;;
        10_exceptions_and_raii/course_audit_error_boundary)
            cmake_demo "$id" "course_audit_error_boundary" --root "$PROJECT" --transport exception
            ;;
        10_exceptions_and_raii/error_model_lab)
            local safe="${id//\//__}"
            local demo_dir="$BUILD_ROOT/${safe}_demo"
            mkdir -p "$demo_dir"
            cmake_demo "$id" "error_model_lab" unwind "$demo_dir"
            ;;
        11_smart_pointers/01_ownership_factory_lab)
            run_local_script "$id" "run_checks.sh"
            ;;
        11_smart_pointers/02_shared_weak_graph_lab)
            cmake_demo "$id" "shared_weak_graph_lab"
            ;;
        12_move_semantics_and_casts/01_move_ownership_pipeline)
            run_local_script "$id" "run_checks.sh"
            ;;
        12_move_semantics_and_casts/02_casts_lab)
            run_local_script "$id" "run_checks.sh"
            ;;
        13_type_deduction_lambdas_and_function/01_course_query_pipeline)
            run_local_script "$id" "run_checks.sh"
            ;;
        13_type_deduction_lambdas_and_function/02_callback_registry_lab)
            run_local_script "$id" "run_checks.sh"
            ;;
        14_stl_containers_iterators_algorithms/01_course_catalog_pipeline)
            run_local_script "$id" "run_checks.sh"
            ;;
        14_stl_containers_iterators_algorithms/02_invalidation_and_erase_lab)
            run_local_script "$id" "run_checks.sh"
            ;;
        15_modern_cpp_and_next_steps/01_constexpr_and_concepts_lab)
            run_local_script "$id" "run_checks.sh"
            ;;
        15_modern_cpp_and_next_steps/02_cpp20_feature_tour)
            run_local_script "$id" "run_checks.sh"
            ;;
        *)
            echo "${RED}Неизвестный эксперимент: $id${RESET}"
            return 2
            ;;
    esac
}

experiment_screen() {
    local id="$1"

    while true; do
        clear 2>/dev/null || true
        echo "${BOLD}${CYAN}Эксперимент: $id${RESET}"
        show_info "$id"
        echo
        echo "  1) Запустить"
        echo "  2) Посмотреть код / особенности"
        echo "  3) Открыть README"
        echo "  0) Назад"
        echo

        read -r -p "Выбор: " action

        case "$action" in
            1)
                echo
                show_info "$id"
                echo
                if ask_yes_no "Продолжить запуск"; then
                    echo
                    echo "${GREEN}${BOLD}========== НАЧАЛО ЭКСПЕРИМЕНТА ==========${RESET}"
                    set +e
                    run_experiment "$id"
                    rc=$?
                    set -e
                    echo "${GREEN}${BOLD}========== КОНЕЦ ЭКСПЕРИМЕНТА ===========${RESET}"
                    echo "EXIT_CODE=$rc"
                    pause
                fi
                ;;
            2)
                show_code_menu "$id"
                ;;
            3)
                display_file "$PROJECT/examples/$id/README.md"
                ;;
            0)
                return
                ;;
            *)
                echo "Нет такого пункта."
                pause
                ;;
        esac
    done
}

block_screen() {
    local block_no="$1"
    local block
    block="$(block_dir "$block_no")"
    mapfile -t exps < <(experiments_for_block "$block_no")

    while true; do
        clear 2>/dev/null || true
        echo "${BOLD}${CYAN}Блок $(printf '%02d' "$block_no"): $(block_title "$block_no")${RESET}"
        echo
        printf '  1) %s\n' "${exps[0]}"
        printf '  2) %s\n' "${exps[1]}"
        echo "  0) Назад"
        echo

        read -r -p "Выберите эксперимент: " choice

        case "$choice" in
            1) experiment_screen "$block/${exps[0]}" ;;
            2) experiment_screen "$block/${exps[1]}" ;;
            0) return ;;
            *) echo "Нет такого пункта."; pause ;;
        esac
    done
}

smoke_test() {
    local count=0
    local block_no
    local block
    local exp

    for block_no in $(seq 1 15); do
        block="$(block_dir "$block_no")"
        mapfile -t exps < <(experiments_for_block "$block_no")

        [[ "${#exps[@]}" -eq 2 ]] || {
            echo "BLOCK_${block_no}_EXPERIMENT_COUNT=FAIL"
            return 1
        }

        for exp in "${exps[@]}"; do
            [[ -d "$PROJECT/examples/$block/$exp" ]] || {
                echo "MISSING=$block/$exp"
                return 1
            }
            experiment_info "$block/$exp"
            [[ -n "$WHAT" && -n "$SHOWS" && -n "$FEATURES" ]] || {
                echo "METADATA_MISSING=$block/$exp"
                return 1
            }
            count=$((count + 1))
        done
    done

    echo "COURSE_MENU_SMOKE=PASS"
    echo "BLOCKS=15"
    echo "EXPERIMENTS=$count"
}

if [[ "${COURSE_MENU_SMOKE:-0}" == "1" ]]; then
    smoke_test
    exit 0
fi

while true; do
    clear 2>/dev/null || true
    echo "${BOLD}${CYAN}============================================================${RESET}"
    echo "${BOLD}${CYAN} C++ — МЕНЮ ЭКСПЕРИМЕНТОВ${RESET}"
    echo "${BOLD}${CYAN}============================================================${RESET}"
    echo

    for block_no in $(seq 1 15); do
        printf '  %2d) %s\n' "$block_no" "$(block_title "$block_no")"
    done

    echo
    echo "   0) Выход"
    echo
    read -r -p "Выберите блок: " block_choice

    if [[ "$block_choice" == "0" ]]; then
        echo "Выход."
        exit 0
    fi

    if [[ "$block_choice" =~ ^[0-9]+$ ]] &&
       (( block_choice >= 1 && block_choice <= 15 )); then
        block_screen "$block_choice"
    else
        echo "Нет такого блока."
        pause
    fi
done
