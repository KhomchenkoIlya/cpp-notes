#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct CourseBlock {
    std::string name;
    bool lecture_exists{};
    bool examples_exist{};
    std::size_t lecture_lines{};
    std::size_t example_files{};
};

struct AuditResult {
    std::string block;
    std::string policy;
    bool passed{};
    std::string detail;
};

std::size_t count_lines(const std::filesystem::path& path) {
    std::ifstream input{path};
    std::size_t count{};
    std::string line;
    while (std::getline(input, line)) ++count;
    return count;
}

std::size_t count_source_files(const std::filesystem::path& directory) {
    if (!std::filesystem::is_directory(directory)) return 0;
    std::size_t count{};
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
        const std::string path = entry.path().string();
        if (entry.is_regular_file() && entry.path().filename() != ".gitkeep" &&
            path.find("/build") == std::string::npos) ++count;
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

class AuditStrategy {
public:
    virtual ~AuditStrategy() = default;
    virtual AuditResult evaluate(const CourseBlock& block) const = 0;
    virtual const char* name() const = 0;
};

class StructuralAuditStrategy final : public AuditStrategy {
public:
    AuditResult evaluate(const CourseBlock& block) const override {
        const bool pass = block.lecture_exists && block.examples_exist;
        return {block.name, name(), pass, pass ? "pair exists" : "missing lecture/examples"};
    }
    const char* name() const override { return "structural"; }
};

class StrictAuditStrategy final : public AuditStrategy {
public:
    AuditResult evaluate(const CourseBlock& block) const override {
        const bool pass = block.lecture_exists && block.examples_exist &&
                          block.lecture_lines > 0 && block.example_files > 0;
        const std::string detail = "lines=" + std::to_string(block.lecture_lines) +
                                   ", files=" + std::to_string(block.example_files);
        return {block.name, name(), pass, detail};
    }
    const char* name() const override { return "strict"; }
};

// Extension proof: AuditEngine is unchanged when this policy is added.
class LectureOnlyAuditStrategy final : public AuditStrategy {
public:
    AuditResult evaluate(const CourseBlock& block) const override {
        return {block.name, name(), block.lecture_exists && block.lecture_lines > 0,
                "lecture lines=" + std::to_string(block.lecture_lines)};
    }
    const char* name() const override { return "lecture-only"; }
};

class AuditObserver {
public:
    virtual ~AuditObserver() = default;
    virtual void on_result(const AuditResult& result) = 0;
};

class ConsoleSummaryObserver final : public AuditObserver {
public:
    void on_result(const AuditResult& result) override {
        std::cout << result.block << ' ' << (result.passed ? "PASS" : "FAIL") << '\n';
    }
};

class IssueDetailObserver final : public AuditObserver {
public:
    void on_result(const AuditResult& result) override {
        if (!result.passed) {
            ++issues_;
            std::cout << "  issue: " << result.detail << '\n';
        }
    }
    std::size_t issues() const { return issues_; }

private:
    std::size_t issues_{};
};

class StatisticsObserver final : public AuditObserver {
public:
    void on_result(const AuditResult& result) override {
        ++checked_;
        if (result.passed) ++passed_;
    }
    std::size_t checked() const { return checked_; }
    std::size_t passed() const { return passed_; }

private:
    std::size_t checked_{};
    std::size_t passed_{};
};

class AuditEngine {
public:
    explicit AuditEngine(const AuditStrategy& strategy) : strategy_{strategy} {}
    void subscribe(AuditObserver& observer) { observers_.push_back(&observer); }
    void run(const std::vector<CourseBlock>& blocks) const {
        for (const CourseBlock& block : blocks) {
            const AuditResult result = strategy_.evaluate(block);
            for (AuditObserver* observer : observers_) observer->on_result(result);
        }
    }

private:
    const AuditStrategy& strategy_;          // Non-owning.
    std::vector<AuditObserver*> observers_;  // Non-owning.
};

using Names = std::array<const char*, 15>;

int main(int argc, char* argv[]) {
    std::filesystem::path root = std::filesystem::current_path();
    std::string policy{"structural"};
    for (int i = 1; i < argc; ++i) {
        const std::string argument{argv[i]};
        if (argument == "--root" && i + 1 < argc) root = argv[++i];
        else if (argument == "--policy" && i + 1 < argc) policy = argv[++i];
        else {
            std::cerr << "Usage: course_audit_patterns [--root PATH] "
                         "[--policy structural|strict|lecture-only]\n";
            return 2;
        }
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
    for (const char* name : names) blocks.push_back(inspect(root, name));

    StructuralAuditStrategy structural;
    StrictAuditStrategy strict;
    LectureOnlyAuditStrategy lecture_only;
    const AuditStrategy* strategy = nullptr;
    if (policy == structural.name()) strategy = &structural;
    else if (policy == strict.name()) strategy = &strict;
    else if (policy == lecture_only.name()) strategy = &lecture_only;
    else {
        std::cerr << "Unknown policy: " << policy << '\n';
        return 2;
    }

    ConsoleSummaryObserver console;
    IssueDetailObserver issues;
    StatisticsObserver statistics;
    AuditEngine engine{*strategy};
    engine.subscribe(console);
    engine.subscribe(issues);
    engine.subscribe(statistics);
    engine.run(blocks);
    // The completed course has substantive lecture/examples pairs for all blocks.
    const std::size_t expected_passed = names.size();
    const bool pass = statistics.checked() == names.size() &&
                      statistics.passed() == expected_passed &&
                      issues.issues() == names.size() - expected_passed;
    std::cout << "policy=" << strategy->name() << ", observers=3, checked="
              << statistics.checked() << ", passed=" << statistics.passed()
              << "\nCOURSE_AUDIT_PATTERNS=" << (pass ? "PASS" : "FAIL") << '\n';
    return pass ? 0 : 1;
}
