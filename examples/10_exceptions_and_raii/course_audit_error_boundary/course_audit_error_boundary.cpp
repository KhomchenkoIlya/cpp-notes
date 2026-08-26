#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct Status { bool ok; std::string message; };
struct BlockResult { std::string name; bool complete; std::string detail; };

const std::array<const char*, 15> names{
    "01_intro_and_first_program", "02_declarations_flow_and_errors",
    "03_pointers_memory_and_arrays", "04_functions_references_and_const",
    "05_classes_and_encapsulation", "06_constructors_destructors_and_copy",
    "07_inheritance_and_polymorphism", "08_design_solid_and_patterns", "09_templates",
    "10_exceptions_and_raii", "11_smart_pointers", "12_move_semantics_and_casts",
    "13_type_deduction_lambdas_and_function", "14_stl_containers_iterators_algorithms",
    "15_modern_cpp_and_next_steps"};

std::size_t line_count(const fs::path& path) {
    std::ifstream input{path};
    if (!input) throw std::runtime_error{"cannot read " + path.string()};
    std::size_t count{}; std::string line;
    while (std::getline(input, line)) ++count;
    if (input.bad()) throw std::runtime_error{"I/O error in " + path.string()};
    return count;
}

std::size_t source_count(const fs::path& directory) {
    std::size_t count{};
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().filename() != ".gitkeep" &&
            entry.path().string().find("/build") == std::string::npos) ++count;
    }
    return count;
}

BlockResult inspect(const fs::path& root, const std::string& name) {
    const fs::path lecture = root / "lectures" / (name + ".tex");
    const fs::path examples = root / "examples" / name;
    if (!fs::exists(lecture) || !fs::is_directory(examples))
        return {name, false, "missing block pair"}; // expected domain result
    const std::size_t lines = line_count(lecture);
    const std::size_t files = source_count(examples);
    return {name, lines > 5 && files > 0,
            "lines=" + std::to_string(lines) + ", files=" + std::to_string(files)};
}

std::vector<BlockResult> audit_exception(const fs::path& root, bool simulate) {
    if (!fs::is_directory(root / "lectures") || !fs::is_directory(root / "examples"))
        throw std::runtime_error{"not a course root: " + root.string()};
    std::vector<BlockResult> results;
    for (const char* name : names) {
        if (simulate && results.size() == 3) throw std::runtime_error{"synthetic I/O failure"};
        results.push_back(inspect(root, name));
    }
    return results;
}

Status audit_status(const fs::path& root, bool simulate, std::vector<BlockResult>& out) {
    try { out = audit_exception(root, simulate); }
    catch (const std::exception& error) { return {false, error.what()}; }
    return {true, {}};
}

int present(const std::vector<BlockResult>& results, bool expect_incomplete) {
    std::size_t complete{};
    for (const auto& result : results) {
        if (result.complete) ++complete;
        std::cout << result.name << ' ' << (result.complete ? "COMPLETE" : "INCOMPLETE")
                  << " (" << result.detail << ")\n";
    }
    std::cout << "checked=" << results.size() << ", complete=" << complete << '\n';
    // Incomplete future blocks are useful audit data, not an operational failure.
    return expect_incomplete && complete == results.size() ? 4 : 0;
}

int main(int argc, char* argv[]) {
    fs::path root = fs::current_path();
    std::string transport{"exception"}; bool simulate{}; bool expect_incomplete{};
    for (int i = 1; i < argc; ++i) {
        const std::string arg{argv[i]};
        if (arg == "--root" && i + 1 < argc) root = argv[++i];
        else if (arg == "--transport" && i + 1 < argc) transport = argv[++i];
        else if (arg == "--simulate-io") simulate = true;
        else if (arg == "--expect-incomplete") expect_incomplete = true;
        else { std::cerr << "invalid arguments\n"; return 64; }
    }
    std::vector<BlockResult> results;
    if (transport == "status") {
        const Status status = audit_status(root, simulate, results);
        if (!status.ok) { std::cerr << "status boundary: " << status.message << '\n'; return 2; }
        return present(results, expect_incomplete);
    }
    if (transport != "exception") { std::cerr << "unknown transport\n"; return 64; }
    try {
        results = audit_exception(root, simulate); // intermediate calls do not catch
        return present(results, expect_incomplete);
    } catch (const std::exception& error) {
        std::cerr << "audit failed: " << error.what() << '\n';
        return 2;
    }
}
