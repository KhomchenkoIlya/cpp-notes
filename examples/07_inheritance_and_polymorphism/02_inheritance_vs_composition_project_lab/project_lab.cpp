#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class CourseBlock {
public:
    CourseBlock(const std::filesystem::path& root, std::string full_name)
        : full_name_{full_name},
          lecture_path_{root / "lectures" / (full_name_ + ".tex")},
          examples_path_{root / "examples" / full_name_},
          lecture_exists_{std::filesystem::is_regular_file(lecture_path_)},
          examples_exist_{std::filesystem::is_directory(examples_path_)},
          lecture_lines_{lecture_exists_ ? count_lines(lecture_path_) : 0},
          example_files_{examples_exist_ ? count_files(examples_path_) : 0} {}

    const std::string& full_name() const { return full_name_; }
    bool lecture_exists() const { return lecture_exists_; }
    bool examples_exist() const { return examples_exist_; }
    std::size_t lecture_lines() const { return lecture_lines_; }
    std::size_t example_files() const { return example_files_; }

private:
    static std::size_t count_lines(const std::filesystem::path& file) {
        std::ifstream input{file};
        std::size_t lines{};
        std::string line;
        while (std::getline(input, line)) {
            ++lines;
        }
        return lines;
    }

    static std::size_t count_files(const std::filesystem::path& directory) {
        std::size_t files{};
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
            if (!entry.is_regular_file()) {
                continue;
            }
            const std::string path{entry.path().string()};
            if (entry.path().filename() != ".gitkeep" &&
                path.find("/build") == std::string::npos) {
                ++files;
            }
        }
        return files;
    }

    std::string full_name_;
    std::filesystem::path lecture_path_;
    std::filesystem::path examples_path_;
    bool lecture_exists_;
    bool examples_exist_;
    std::size_t lecture_lines_;
    std::size_t example_files_;
};

struct CheckResult {
    std::string name;
    bool passed;
};

class BlockCheck {
public:
    virtual ~BlockCheck() = default;
    virtual CheckResult run(const CourseBlock& block) const = 0;
};

class LectureExistsCheck final : public BlockCheck {
public:
    CheckResult run(const CourseBlock& block) const override {
        return {"lecture exists", block.lecture_exists()};
    }
};

class ExamplesExistCheck final : public BlockCheck {
public:
    CheckResult run(const CourseBlock& block) const override {
        return {"examples directory exists", block.examples_exist()};
    }
};

class ContentCheck final : public BlockCheck {
public:
    CheckResult run(const CourseBlock& block) const override {
        return {"lecture and examples non-empty",
                block.lecture_lines() > 0 && block.example_files() > 0};
    }
};

std::vector<CheckResult> audit_with_inheritance(const CourseBlock& block) {
    LectureExistsCheck lecture;
    ExamplesExistCheck examples;
    ContentCheck content;
    const std::array<const BlockCheck*, 3> checks{&lecture, &examples, &content};

    std::vector<CheckResult> results;
    results.reserve(checks.size());
    for (const BlockCheck* check : checks) {
        results.push_back(check->run(block));
    }
    return results;
}

class PathChecks {
public:
    CheckResult lecture_exists(const CourseBlock& block) const {
        return {"lecture exists", block.lecture_exists()};
    }
    CheckResult examples_exist(const CourseBlock& block) const {
        return {"examples directory exists", block.examples_exist()};
    }
};

class ContentChecks {
public:
    CheckResult non_empty(const CourseBlock& block) const {
        return {"lecture and examples non-empty",
                block.lecture_lines() > 0 && block.example_files() > 0};
    }
};

class CourseAudit {
public:
    std::vector<CheckResult> run(const CourseBlock& block) const {
        return {paths_.lecture_exists(block), paths_.examples_exist(block),
                content_.non_empty(block)};
    }

private:
    PathChecks paths_;       // CourseAudit has checking collaborators.
    ContentChecks content_;
};

bool same_results(const std::vector<CheckResult>& left,
                  const std::vector<CheckResult>& right) {
    if (left.size() != right.size()) {
        return false;
    }
    for (std::size_t i = 0; i < left.size(); ++i) {
        if (left[i].name != right[i].name || left[i].passed != right[i].passed) {
            return false;
        }
    }
    return true;
}

bool all_pass(const std::vector<CheckResult>& results) {
    bool passed{true};
    for (const CheckResult& result : results) {
        passed = passed && result.passed;
    }
    return passed;
}

void print_results(const char* design, const std::vector<CheckResult>& results) {
    std::cout << design << ":\n";
    for (const CheckResult& result : results) {
        std::cout << "  " << result.name << '=' << (result.passed ? "PASS" : "FAIL") << '\n';
    }
}

int main(int argc, char* argv[]) {
    std::filesystem::path root = std::filesystem::current_path();
    if (argc == 3 && std::string{argv[1]} == "--root") {
        root = argv[2];
    } else if (argc != 1) {
        std::cerr << "Usage: project_lab [--root PATH]\n";
        return 2;
    }
    if (!std::filesystem::is_directory(root / "lectures") ||
        !std::filesystem::is_directory(root / "examples")) {
        std::cerr << "Not a course root: " << root << '\n';
        return 2;
    }

    const CourseBlock block{root, "07_inheritance_and_polymorphism"};
    const std::vector<CheckResult> inheritance = audit_with_inheritance(block);
    const CourseAudit audit;
    const std::vector<CheckResult> composition = audit.run(block);

    print_results("inheritance", inheritance);
    print_results("composition", composition);
    const bool equivalent = same_results(inheritance, composition);
    const bool pass = equivalent && all_pass(inheritance);
    std::cout << "same observable results=" << std::boolalpha << equivalent << '\n';
    std::cout << "PROJECT_DESIGN_LAB=" << (pass ? "PASS" : "FAIL") << '\n';
    return pass ? 0 : 1;
}
