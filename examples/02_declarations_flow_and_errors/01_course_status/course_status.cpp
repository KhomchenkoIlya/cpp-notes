#include <filesystem>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

int main() {
    const int block_count{15};
    const std::vector<std::string> slugs{
        "intro_and_first_program",
        "declarations_flow_and_errors",
        "pointers_memory_and_arrays",
        "functions_references_and_const",
        "classes_and_encapsulation",
        "constructors_destructors_and_copy",
        "inheritance_and_polymorphism",
        "design_solid_and_patterns",
        "templates",
        "exceptions_and_raii",
        "smart_pointers",
        "move_semantics_and_casts",
        "type_deduction_lambdas_and_function",
        "stl_containers_iterators_algorithms",
        "modern_cpp_and_next_steps",
    };

    std::filesystem::path course_root = std::filesystem::current_path();
    bool root_found = false;

    while (!root_found) {
        const bool has_lectures =
            std::filesystem::is_directory(course_root / "lectures");
        const bool has_examples =
            std::filesystem::is_directory(course_root / "examples");
        root_found = has_lectures && has_examples;

        if (!root_found) {
            const std::filesystem::path parent = course_root.parent_path();
            if (parent == course_root) {
                std::cerr << "Course root was not found.\n";
                return 2;
            }
            course_root = parent;
        }
    }

    bool running = true;
    while (running) {
        std::cout << "\nCourse structure: " << course_root << '\n'
                  << "1 - show all blocks\n"
                  << "2 - check one block\n"
                  << "3 - show incomplete pairs\n"
                  << "0 - exit\n"
                  << "> ";

        int choice{};
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) {
                std::cout << "\nInput closed.\n";
                break;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter an integer menu item.\n";
            continue;
        }

        switch (choice) {
        case 1: {
            for (int index = 0; index < block_count; ++index) {
                const int number = index + 1;
                std::string prefix;
                if (number < 10) {
                    prefix = "0";
                }
                const std::string block =
                    prefix + std::to_string(number) + "_" + slugs.at(index);
                const bool lecture_exists =
                    std::filesystem::is_regular_file(course_root / "lectures" / (block + ".tex"));
                const bool examples_exist =
                    std::filesystem::is_directory(course_root / "examples" / block);
                std::cout << block << ": ";
                if (lecture_exists && examples_exist) {
                    std::cout << "complete pair\n";
                } else {
                    std::cout << "INCOMPLETE\n";
                }
            }
            break;
        }
        case 2: {
            std::cout << "Block number [1-15]: ";
            int number{};
            if (!(std::cin >> number)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Block number must be an integer.\n";
                break;
            }
            if (number < 1 || number > block_count) {
                std::cout << "Number must be from 1 to 15.\n";
                break;
            }

            const int index = number - 1;
            std::string prefix;
            if (number < 10) {
                prefix = "0";
            }
            const std::string block =
                prefix + std::to_string(number) + "_" + slugs.at(index);
            const bool lecture_exists =
                std::filesystem::is_regular_file(course_root / "lectures" / (block + ".tex"));
            const bool examples_exist =
                std::filesystem::is_directory(course_root / "examples" / block);
            std::cout << "lecture: ";
            if (lecture_exists) {
                std::cout << "present\n";
            } else {
                std::cout << "MISSING\n";
            }
            std::cout << "examples: ";
            if (examples_exist) {
                std::cout << "present\n";
            } else {
                std::cout << "MISSING\n";
            }
            break;
        }
        case 3: {
            int missing_count{};
            for (int index = 0; index < block_count; ++index) {
                const int number = index + 1;
                std::string prefix;
                if (number < 10) {
                    prefix = "0";
                }
                const std::string block =
                    prefix + std::to_string(number) + "_" + slugs.at(index);
                const bool lecture_exists =
                    std::filesystem::is_regular_file(course_root / "lectures" / (block + ".tex"));
                const bool examples_exist =
                    std::filesystem::is_directory(course_root / "examples" / block);
                if (!lecture_exists || !examples_exist) {
                    ++missing_count;
                    std::cout << block << ":";
                    if (!lecture_exists) {
                        std::cout << " lecture";
                    }
                    if (!examples_exist) {
                        std::cout << " examples";
                    }
                    std::cout << " missing\n";
                }
            }
            if (missing_count == 0) {
                std::cout << "All 15 structural pairs are present.\n";
            }
            break;
        }
        case 0:
            running = false;
            break;
        default:
            std::cout << "Unknown menu item.\n";
            break;
        }
    }

    return 0;
}
