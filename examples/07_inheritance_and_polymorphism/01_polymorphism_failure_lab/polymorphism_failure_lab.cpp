#include <iostream>
#include <string>

class Report {
public:
    explicit Report(std::string name) : name_{name} {}
    virtual ~Report() = default;

    std::string non_virtual_kind() const { return "Report"; }
    virtual std::string virtual_kind() const { return "Report"; }
    const std::string& name() const { return name_; }

protected:
    void mark_checked() { checked_ = true; }
    bool checked() const { return checked_; }

private:
    std::string name_;
    bool checked_{};
};

class LectureReport final : public Report {
public:
    LectureReport(std::string name, int lines)
        : Report{name}, lines_{lines} {
        mark_checked(); // Derived uses a protected operation, not base state.
    }

    std::string non_virtual_kind() const { return "LectureReport"; }
    std::string virtual_kind() const override { return "LectureReport"; }
    int lines() const { return lines_; }
    bool was_checked() const { return checked(); }

private:
    int lines_;
};

void print_by_value(Report report) {
    std::cout << "sliced value: kind=" << report.virtual_kind()
              << ", name=" << report.name() << '\n';
}

void print_by_reference(const Report& report) {
    std::cout << "reference: kind=" << report.virtual_kind()
              << ", name=" << report.name() << '\n';
}

class BlockCheck {
public:
    virtual ~BlockCheck() = default;
    virtual const char* name() const = 0;
    virtual bool passes(int lecture_lines) const = 0;
};

class NonEmptyLecture final : public BlockCheck {
public:
    const char* name() const override { return "lecture non-empty"; }
    bool passes(int lecture_lines) const override { return lecture_lines > 0; }
};

class SafeBase {
public:
    virtual ~SafeBase() { std::cout << "destroy SafeBase\n"; }
};

class SafeDerived final : public SafeBase {
public:
    ~SafeDerived() override { std::cout << "destroy SafeDerived\n"; }
};

int main() {
    LectureReport lecture{"07_inheritance_and_polymorphism", 500};
    Report& base_reference{lecture};
    Report* base_pointer{&lecture}; // Non-owning; lecture remains on the stack.

    std::cout << "static dispatch: " << base_reference.non_virtual_kind() << '\n';
    std::cout << "dynamic dispatch: " << base_reference.virtual_kind() << '\n';
    std::cout << "pointer dispatch: " << base_pointer->virtual_kind() << '\n';
    std::cout << "derived state: lines=" << lecture.lines()
              << ", checked=" << std::boolalpha << lecture.was_checked() << '\n';

    print_by_value(lecture);
    print_by_reference(lecture);

    NonEmptyLecture concrete_check;
    const BlockCheck& check{concrete_check};
    std::cout << "abstract interface: " << check.name() << '='
              << check.passes(lecture.lines()) << '\n';

    std::cout << "virtual destructor trace:\n";
    SafeDerived object;
    SafeBase* deletion_view{new SafeDerived};
    delete deletion_view;
    std::cout << "automatic object follows:\n";

    const bool pass = base_reference.non_virtual_kind() == "Report" &&
                      base_reference.virtual_kind() == "LectureReport" &&
                      check.passes(lecture.lines()) && lecture.was_checked();
    std::cout << "POLYMORPHISM_FAILURE_LAB=" << (pass ? "PASS" : "FAIL") << '\n';
    return pass ? 0 : 1;
}
