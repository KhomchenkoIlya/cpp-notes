#include "audit.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::size_t count_lines(const std::filesystem::path& path) {
    std::ifstream input{path};
    std::size_t count{};
    std::string line;
    while (std::getline(input, line)) {
        ++count;
    }
    return count;
}

std::size_t count_source_files(const std::filesystem::path& directory) {
    if (!std::filesystem::is_directory(directory)) {
        return 0;
    }
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

CourseBlock inspect(const std::filesystem::path& root, const std::string& name) {
    const std::filesystem::path lecture = root / "lectures" / (name + ".tex");
    const std::filesystem::path examples = root / "examples" / name;
    const bool lecture_exists = std::filesystem::is_regular_file(lecture);
    return {name, lecture_exists, std::filesystem::is_directory(examples),
            lecture_exists ? count_lines(lecture) : 0, count_source_files(examples)};
}

class CompactReporter {
public:
    void on_result(const AuditResult& result) {
        std::cout << (result.passed ? '.' : 'F');
    }
    void on_finish(const AuditSummary& summary) {
        std::cout << " compact checked=" << summary.checked << ", passed=" << summary.passed
                  << '\n';
    }
};

class DetailedReporter {
public:
    void on_result(const AuditResult& result) {
        std::cout << result.block << ' ' << (result.passed ? "PASS" : "FAIL")
                  << " (" << result.detail << ")\n";
    }
    void on_finish(const AuditSummary& summary) {
        std::cout << "detailed checked=" << summary.checked << ", passed=" << summary.passed
                  << '\n';
    }
};

using Names = std::array<const char*, 15>;

int main(int argc, char* argv[]) {
    std::filesystem::path root = std::filesystem::current_path();
    if (argc == 3 && std::string{argv[1]} == "--root") {
        root = argv[2];
    } else if (argc != 1) {
        std::cerr << "Usage: generic_course_audit [--root PATH]\n";
        return 2;
    }
    if (!std::filesystem::is_directory(root / "lectures") ||
        !std::filesystem::is_directory(root / "examples")) {
        std::cerr << "Not a course root: " << root << '\n';
        return 2;
    }

    const Names names{
        "01_intro_and_first_program", "02_declarations_flow_and_errors",
        "03_pointers_memory_and_arrays", "04_functions_references_and_const",
        "05_classes_and_encapsulation", "06_constructors_destructors_and_copy",
        "07_inheritance_and_polymorphism", "08_design_solid_and_patterns",
        "09_templates", "10_exceptions_and_raii", "11_smart_pointers",
        "12_move_semantics_and_casts", "13_type_deduction_lambdas_and_function",
        "14_stl_containers_iterators_algorithms", "15_modern_cpp_and_next_steps"};
    std::vector<CourseBlock> blocks;
    blocks.reserve(names.size());
    for (const char* name : names) {
        blocks.push_back(inspect(root, name));
    }

    const StrictPolicy policy;
    CompactReporter compact;
    DetailedReporter detailed;
    const AuditSummary compact_summary = run_audit(blocks, policy, compact);
    const AuditSummary detailed_summary = run_audit(blocks, policy, detailed);

    const bool same = compact_summary.checked == detailed_summary.checked &&
                      compact_summary.passed == detailed_summary.passed;
    const bool pass = same && compact_summary.checked == names.size() &&
                      compact_summary.passed == names.size();
    std::cout << "reporters agree=" << std::boolalpha << same
              << "\nGENERIC_COURSE_AUDIT=" << (pass ? "PASS" : "FAIL") << '\n';
    return pass ? 0 : 1;
}
