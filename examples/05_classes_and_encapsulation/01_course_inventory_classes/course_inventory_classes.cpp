#include <array>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class CourseBlock {
public:
    static CourseBlock inspect(const std::filesystem::path& root, int number,
                               const std::string& full_name) {
        CourseBlock block;
        block.number_ = number;
        block.full_name_ = full_name;
        block.slug_ = full_name.substr(3);
        block.lecture_path_ = root / "lectures" / (full_name + ".tex");
        block.examples_path_ = root / "examples" / full_name;
        block.lecture_exists_ = std::filesystem::is_regular_file(block.lecture_path_);
        block.examples_exist_ = std::filesystem::is_directory(block.examples_path_);
        if (block.lecture_exists_) {
            block.lecture_bytes_ = std::filesystem::file_size(block.lecture_path_);
            block.lecture_lines_ = count_lines(block.lecture_path_);
        }
        if (block.examples_exist_) {
            block.example_files_ = count_example_files(block.examples_path_);
        }
        return block;
    }

    int number() const { return number_; }
    const std::string& slug() const { return slug_; }
    const std::string& full_name() const { return full_name_; }
    const std::filesystem::path& lecture_path() const { return lecture_path_; }
    const std::filesystem::path& examples_path() const { return examples_path_; }
    bool lecture_exists() const { return lecture_exists_; }
    bool examples_exist() const { return examples_exist_; }
    std::uintmax_t lecture_bytes() const { return lecture_bytes_; }
    std::size_t lecture_lines() const { return lecture_lines_; }
    std::size_t example_file_count() const { return example_files_; }
    bool is_complete() const { return lecture_exists_ && examples_exist_; }

private:
    CourseBlock() = default; // Full constructor design belongs to block 06.

    static std::size_t count_lines(const std::filesystem::path& file) {
        std::ifstream input{file};
        std::size_t count{};
        std::string line;
        while (std::getline(input, line)) {
            ++count;
        }
        return count;
    }

    static std::size_t count_example_files(const std::filesystem::path& directory) {
        std::size_t count{};
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
            const std::string path = entry.path().string();
            if (entry.is_regular_file() && entry.path().filename() != ".gitkeep" &&
                path.find("/build") == std::string::npos) {
                ++count;
            }
        }
        return count;
    }

    int number_{};
    std::string slug_;
    std::string full_name_;
    std::filesystem::path lecture_path_;
    std::filesystem::path examples_path_;
    bool lecture_exists_{};
    bool examples_exist_{};
    std::uintmax_t lecture_bytes_{};
    std::size_t lecture_lines_{};
    std::size_t example_files_{};
};

using BlockNames = std::array<const char*, 15>;

bool read_arguments(int argc, char* argv[], std::filesystem::path& root, int& selected) {
    root = std::filesystem::current_path();
    selected = 0;
    for (int i = 1; i < argc; ++i) {
        const std::string argument{argv[i]};
        if (argument == "--root" && i + 1 < argc) {
            root = argv[++i];
        } else if (argument == "--block" && i + 1 < argc) {
            try {
                selected = std::stoi(argv[++i]);
            } catch (...) {
                std::cerr << "--block expects an integer from 1 to 15\n";
                return false;
            }
        } else {
            std::cerr << "Usage: course_inventory [--root PATH] [--block 1..15]\n";
            return false;
        }
    }
    return true;
}

std::vector<CourseBlock> collect_blocks(const std::filesystem::path& root, int selected,
                                        const BlockNames& names) {
    const std::size_t first = selected == 0 ? 0 : static_cast<std::size_t>(selected - 1);
    const std::size_t last = selected == 0 ? names.size() : first + 1;
    std::vector<CourseBlock> blocks;
    blocks.reserve(names.size());
    for (std::size_t index = first; index < last; ++index) {
        blocks.push_back(CourseBlock::inspect(root, static_cast<int>(index + 1), names[index]));
    }
    return blocks;
}

bool print_report(const BlockNames& names, const std::vector<CourseBlock>& blocks) {
    std::uintmax_t total_bytes{};
    std::size_t total_lines{};
    std::size_t total_files{};
    bool complete = true;
    std::cout << "fixed blocks: " << names.size() << ", block data @ "
              << static_cast<const void*>(names.data()) << '\n';
    std::cout << "results: " << blocks.size() << ", result data @ "
              << static_cast<const void*>(blocks.data()) << "\n\n";
    std::cout << std::left << std::setw(43) << "block" << std::right << std::setw(10)
              << "bytes" << std::setw(9) << "lines" << std::setw(9) << "files" << "  state\n";
    for (const CourseBlock& block : blocks) {
        total_bytes += block.lecture_bytes();
        total_lines += block.lecture_lines();
        total_files += block.example_file_count();
        complete = complete && block.is_complete();
        std::cout << std::left << std::setw(43) << block.full_name() << std::right
                  << std::setw(10) << block.lecture_bytes() << std::setw(9)
                  << block.lecture_lines() << std::setw(9) << block.example_file_count()
                  << "  " << (block.is_complete() ? "OK" : "INCOMPLETE") << '\n';
    }
    std::cout << "TOTAL: bytes=" << total_bytes << " lines=" << total_lines
              << " files=" << total_files << '\n';
    return complete;
}

int main(int argc, char* argv[]) {
    const BlockNames names{
        "01_intro_and_first_program", "02_declarations_flow_and_errors",
        "03_pointers_memory_and_arrays", "04_functions_references_and_const",
        "05_classes_and_encapsulation", "06_constructors_destructors_and_copy",
        "07_inheritance_and_polymorphism", "08_design_solid_and_patterns",
        "09_templates", "10_exceptions_and_raii", "11_smart_pointers",
        "12_move_semantics_and_casts", "13_type_deduction_lambdas_and_function",
        "14_stl_containers_iterators_algorithms", "15_modern_cpp_and_next_steps"};
    std::filesystem::path root;
    int selected{};
    if (!read_arguments(argc, argv, root, selected)) {
        return 2;
    }
    if (selected < 0 || selected > static_cast<int>(names.size())) {
        std::cerr << "--block expects an integer from 1 to 15\n";
        return 2;
    }
    if (!std::filesystem::is_directory(root / "lectures") ||
        !std::filesystem::is_directory(root / "examples")) {
        std::cerr << "Not a course root: " << root << '\n';
        return 2;
    }
    const std::vector<CourseBlock> blocks = collect_blocks(root, selected, names);
    return print_report(names, blocks) ? 0 : 1;
}
