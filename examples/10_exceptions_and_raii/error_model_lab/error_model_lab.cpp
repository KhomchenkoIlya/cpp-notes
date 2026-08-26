#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

struct Status {
    bool ok;
    std::string message;
};

class Trace {
public:
    explicit Trace(std::string name) : name_{name} { std::cout << "+ " << name_ << '\n'; }
    ~Trace() { std::cout << "- " << name_ << '\n'; }
private:
    std::string name_;
};

class TemporaryReport {
public:
    explicit TemporaryReport(const fs::path& final_path)
        : final_{final_path}, temporary_{final_path.string() + ".tmp"}, output_{temporary_} {
        if (!output_) throw std::runtime_error{"cannot create " + temporary_.string()};
        std::cout << "acquired " << temporary_.filename().string() << '\n';
    }
    TemporaryReport(const TemporaryReport&) = delete;
    TemporaryReport& operator=(const TemporaryReport&) = delete;
    ~TemporaryReport() {
        output_.close();
        if (!committed_) {
            std::error_code ignored;
            fs::remove(temporary_, ignored);
            std::cout << "rollback " << temporary_.filename().string() << '\n';
        }
    }
    std::ofstream& stream() { return output_; }
    void commit() {
        output_.close();
        if (!output_) throw std::runtime_error{"cannot finish temporary report"};
        std::error_code error;
        fs::rename(temporary_, final_, error);
        if (error) throw std::runtime_error{"cannot commit report: " + error.message()};
        committed_ = true;
        std::cout << "committed " << final_.filename().string() << '\n';
    }
private:
    fs::path final_;
    fs::path temporary_;
    std::ofstream output_;
    bool committed_{};
};

Status read_status(const fs::path& input, std::string& value) {
    std::ifstream stream{input};
    if (!stream) return {false, "cannot open input"};
    if (!std::getline(stream, value) || value.empty()) return {false, "empty config"};
    return {true, {}};
}

Status transform_status(const fs::path& input, const fs::path& output, bool fail_after_acquire) {
    std::string value;
    Status status = read_status(input, value);
    if (!status.ok) return status;                         // propagation level 2
    try {
        TemporaryReport report{output};
        report.stream() << "value=" << value << '\n';
        if (fail_after_acquire) return {false, "validation rejected value"};
        report.commit();
    } catch (const std::exception& error) {
        return {false, error.what()};
    }
    return {true, {}};
}

int run_status(const fs::path& input, const fs::path& output, bool fail) {
    const Status status = transform_status(input, output, fail); // level 3
    if (!status.ok) {
        std::cerr << "status boundary: " << status.message << '\n';
        return 2;
    }
    return 0;
}

std::string read_exception(const fs::path& input) {
    std::ifstream stream{input};
    if (!stream) throw std::runtime_error{"cannot open input"};
    std::string value;
    if (!std::getline(stream, value) || value.empty()) throw std::invalid_argument{"empty config"};
    return value;
}

void transform_exception(const fs::path& input, const fs::path& output, bool fail) {
    const std::string value = read_exception(input);
    TemporaryReport report{output};
    report.stream() << "value=" << value << '\n';
    if (fail) throw std::runtime_error{"validation rejected value"};
    report.commit();
}

void deep_throw() { Trace low{"low"}; throw std::runtime_error{"deep failure"}; }
void middle_unwind() { Trace middle{"middle"}; deep_throw(); }

void middle_rethrow() {
    try { deep_throw(); }
    catch (const std::exception& error) {
        std::cerr << "middle context: " << error.what() << '\n';
        throw; // preserves the current exception object
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: error_model_lab CASE WORKDIR\n";
        return 64;
    }
    const std::string mode{argv[1]};
    const fs::path work{argv[2]};
    fs::create_directories(work);
    const fs::path input = work / "config.txt";
    const fs::path output = work / (mode + ".report");
    fs::remove(output);
    fs::remove(output.string() + ".tmp");
    { std::ofstream seed{input}; seed << "course-audit\n"; }

    if (mode == "status-success" || mode == "cleanup-normal") {
        const int code = run_status(input, output, false);
        return code == 0 && fs::is_regular_file(output) &&
                       !fs::exists(output.string() + ".tmp") ? 0 : 5;
    }
    if (mode == "status-failure") {
        const int code = run_status(input, output, true);
        return code != 0 && !fs::exists(output) &&
                       !fs::exists(output.string() + ".tmp") ? code : 5;
    }

    try {
        if (mode == "exception-success") transform_exception(input, output, false);
        else if (mode == "exception-failure" || mode == "cleanup-exception")
            transform_exception(input, output, true);
        else if (mode == "unwind") { Trace top{"top"}; middle_unwind(); }
        else if (mode == "rethrow") middle_rethrow();
        else throw std::invalid_argument{"unknown case"};
    } catch (const std::exception& error) {
        std::cerr << "top boundary caught: " << error.what() << '\n';
        if (mode == "unwind" || mode == "rethrow") return 0;
        if (mode == "cleanup-exception")
            return !fs::exists(output) && !fs::exists(output.string() + ".tmp") ? 0 : 5;
        return 3;
    }
    return fs::is_regular_file(output) && !fs::exists(output.string() + ".tmp") ? 0 : 5;
}
