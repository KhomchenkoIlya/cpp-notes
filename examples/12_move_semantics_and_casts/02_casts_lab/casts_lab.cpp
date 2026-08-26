#include <cstdint>
#include <iostream>
#include <string>
#include <typeinfo>

namespace {

enum class Severity : int {
    info = 1,
    warning = 2,
    error = 3
};

class Report {
public:
    virtual ~Report() = default;
    virtual const char* kind() const = 0;
};

class LectureReport final : public Report {
public:
    explicit LectureReport(int lines) : lines_{lines} {}

    const char* kind() const override {
        return "lecture";
    }

    int lines() const {
        return lines_;
    }

private:
    int lines_;
};

class BuildReport final : public Report {
public:
    const char* kind() const override {
        return "build";
    }
};

bool static_cast_case() {
    const double ratio = 7.9;
    const int truncated = static_cast<int>(ratio);

    const Severity severity = Severity::warning;
    const int code = static_cast<int>(severity);

    const bool pass = truncated == 7 && code == 2;

    std::cout
        << "truncated=" << truncated << '\n'
        << "severity_code=" << code << '\n'
        << "STATIC_CAST=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool dynamic_cast_case() {
    LectureReport lecture{321};
    BuildReport build;

    Report* lecture_base = &lecture;
    Report* build_base = &build;

    LectureReport* success =
        dynamic_cast<LectureReport*>(lecture_base);
    LectureReport* mismatch =
        dynamic_cast<LectureReport*>(build_base);

    bool reference_failure = false;

    try {
        Report& base_reference = build;
        (void)dynamic_cast<LectureReport&>(base_reference);
    } catch (const std::bad_cast&) {
        reference_failure = true;
    }

    const bool pass =
        success != nullptr &&
        success->lines() == 321 &&
        mismatch == nullptr &&
        reference_failure;

    std::cout
        << "pointer_success=" << (success != nullptr ? "yes" : "no") << '\n'
        << "pointer_mismatch_null=" << (mismatch == nullptr ? "yes" : "no") << '\n'
        << "reference_bad_cast=" << (reference_failure ? "yes" : "no") << '\n'
        << "DYNAMIC_CAST=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool const_cast_case() {
    int counter = 5;
    const int* read_only_view = &counter;

    int* mutable_view = const_cast<int*>(read_only_view);
    *mutable_view = 7;

    const int genuinely_const = 11;
    const int* const_view = &genuinely_const;
    int* cast_only = const_cast<int*>(const_view);

    // cast_only is deliberately NOT dereferenced for mutation:
    // modifying genuinely_const through it would be undefined behavior.
    const bool pass =
        counter == 7 &&
        cast_only != nullptr &&
        genuinely_const == 11;

    std::cout
        << "counter=" << counter << '\n'
        << "genuinely_const=" << genuinely_const << '\n'
        << "CONST_CAST=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool reinterpret_cast_case() {
    int value = 42;
    int* pointer = &value;

    const std::uintptr_t bits =
        reinterpret_cast<std::uintptr_t>(pointer);
    int* restored =
        reinterpret_cast<int*>(bits);

    const bool pass =
        restored == pointer &&
        *restored == 42;

    std::cout
        << "round_trip_same_pointer="
        << (restored == pointer ? "yes" : "no") << '\n'
        << "REINTERPRET_CAST=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool no_cast_upcast_case() {
    LectureReport lecture{12};
    Report& base = lecture;

    const bool pass = std::string{base.kind()} == "lecture";

    std::cout
        << "upcast_kind=" << base.kind() << '\n'
        << "UPCAST_WITHOUT_CAST=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

} // namespace

int main(int argc, char* argv[]) {
    std::string selected{"all"};

    if (argc == 3 && std::string{argv[1]} == "--case") {
        selected = argv[2];
    } else if (argc != 1) {
        std::cerr
            << "Usage: casts_lab "
            << "[--case static|dynamic|const|reinterpret|upcast]\n";
        return 2;
    }

    bool pass = true;

    if (selected == "all" || selected == "static") {
        pass = static_cast_case() && pass;
    }
    if (selected == "all" || selected == "dynamic") {
        pass = dynamic_cast_case() && pass;
    }
    if (selected == "all" || selected == "const") {
        pass = const_cast_case() && pass;
    }
    if (selected == "all" || selected == "reinterpret") {
        pass = reinterpret_cast_case() && pass;
    }
    if (selected == "all" || selected == "upcast") {
        pass = no_cast_upcast_case() && pass;
    }

    if (selected != "all" &&
        selected != "static" &&
        selected != "dynamic" &&
        selected != "const" &&
        selected != "reinterpret" &&
        selected != "upcast") {
        std::cerr << "Unknown case: " << selected << '\n';
        return 2;
    }

    std::cout << "CASTS_LAB=" << (pass ? "PASS" : "FAIL") << '\n';
    return pass ? 0 : 1;
}
