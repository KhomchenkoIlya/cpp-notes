#include <array>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

struct BlockInfo {
    std::string name;
    bool lecture_exists{};
    bool examples_exist{};
    std::uintmax_t lecture_bytes{};
    std::size_t lecture_lines{};
    std::size_t example_files{};
};

int main(int argc, char* argv[]) {
    const std::array<const char*, 15> blocks{
        "01_intro_and_first_program", "02_declarations_flow_and_errors",
        "03_pointers_memory_and_arrays", "04_functions_references_and_const",
        "05_classes_and_encapsulation", "06_constructors_destructors_and_copy",
        "07_inheritance_and_polymorphism", "08_design_solid_and_patterns",
        "09_templates", "10_exceptions_and_raii", "11_smart_pointers",
        "12_move_semantics_and_casts", "13_type_deduction_lambdas_and_function",
        "14_stl_containers_iterators_algorithms", "15_modern_cpp_and_next_steps"};

    std::filesystem::path root = std::filesystem::current_path();
    int selected{};
    for (int i = 1; i < argc; ++i) {
        const std::string argument{argv[i]};
        if (argument == "--root" && i + 1 < argc) {
            root = argv[++i];
        } else if (argument == "--block" && i + 1 < argc) {
            try {
                selected = std::stoi(argv[++i]);
            } catch (...) {
                std::cerr << "--block expects an integer from 1 to 15\n";
                return 2;
            }
        } else {
            std::cerr << "Usage: course_inventory [--root PATH] [--block 1..15]\n";
            return 2;
        }
    }
    if (selected < 0 || selected > static_cast<int>(blocks.size())) {
        std::cerr << "--block expects an integer from 1 to 15\n";
        return 2;
    }
    if (!std::filesystem::is_directory(root / "lectures") ||
        !std::filesystem::is_directory(root / "examples")) {
        std::cerr << "Not a course root: " << root << '\n';
        return 2;
    }

    std::vector<BlockInfo> rows;
    rows.reserve(blocks.size());
    const std::size_t first = selected == 0 ? 0 : static_cast<std::size_t>(selected - 1);
    const std::size_t last = selected == 0 ? blocks.size() : first + 1;
    for (std::size_t index = first; index < last; ++index) {
        BlockInfo row;
        row.name = blocks[index];
        const auto lecture = root / "lectures" / (row.name + ".tex");
        const auto examples = root / "examples" / row.name;
        row.lecture_exists = std::filesystem::is_regular_file(lecture);
        row.examples_exist = std::filesystem::is_directory(examples);
        if (row.lecture_exists) {
            row.lecture_bytes = std::filesystem::file_size(lecture);
            std::ifstream input{lecture};
            std::string line;
            while (std::getline(input, line)) {
                ++row.lecture_lines;
            }
        }
        if (row.examples_exist) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(examples)) {
                if (entry.is_regular_file() && entry.path().filename() != ".gitkeep" &&
                    entry.path().string().find("/build") == std::string::npos) {
                    ++row.example_files;
                }
            }
        }
        rows.push_back(row);
    }

    std::uintmax_t total_bytes{};
    std::size_t total_lines{};
    std::size_t total_files{};
    bool complete = true;
    std::cout << "fixed blocks: " << blocks.size() << ", block data @ "
              << static_cast<const void*>(blocks.data()) << '\n';
    std::cout << "results: " << rows.size() << ", result data @ "
              << static_cast<const void*>(rows.data()) << "\n\n";
    std::cout << std::left << std::setw(43) << "block" << std::right << std::setw(10)
              << "bytes" << std::setw(9) << "lines" << std::setw(9) << "files" << "  state\n";
    for (const BlockInfo& row : rows) {
        total_bytes += row.lecture_bytes;
        total_lines += row.lecture_lines;
        total_files += row.example_files;
        complete = complete && row.lecture_exists && row.examples_exist;
        std::cout << std::left << std::setw(43) << row.name << std::right
                  << std::setw(10) << row.lecture_bytes << std::setw(9) << row.lecture_lines
                  << std::setw(9) << row.example_files << "  "
                  << (row.lecture_exists && row.examples_exist ? "OK" : "INCOMPLETE") << '\n';
    }
    std::cout << "TOTAL: bytes=" << total_bytes << " lines=" << total_lines
              << " files=" << total_files << '\n';
    return complete ? 0 : 1;
}
