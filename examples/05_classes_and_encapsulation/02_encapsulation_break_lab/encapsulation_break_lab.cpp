#include <iostream>
#include <string>

struct PublicBlock {
    int number;
    std::string slug;
    bool lecture_exists;
    bool examples_exist;
};

class CourseProgress {
public:
    static CourseProgress for_block(int number, const std::string& slug) {
        CourseProgress progress;
        if (number >= 1 && number <= 15 && !slug.empty()) {
            progress.number_ = number;
            progress.slug_ = slug;
        }
        return progress;
    }

    int number() const { return number_; }
    const std::string& slug() const { return slug_; }
    bool is_valid() const { return number_ >= 1 && number_ <= 15 && !slug_.empty(); }
    bool is_complete() const { return is_valid() && lecture_exists_ && examples_exist_; }

    void mark_lecture_found() { lecture_exists_ = true; }
    void mark_examples_found() { examples_exist_ = true; }

private:
    CourseProgress() = default;

    int number_{};
    std::string slug_;
    bool lecture_exists_{};
    bool examples_exist_{};
};

std::string status_line(const CourseProgress& progress) {
    return std::to_string(progress.number()) + "_" + progress.slug() +
           (progress.is_complete() ? ": COMPLETE" : ": INCOMPLETE");
}

int main() {
    PublicBlock broken{99, "", true, false};
    const bool public_data_accepts_impossible_state =
        broken.number == 99 && broken.slug.empty() && broken.lecture_exists != broken.examples_exist;

    CourseProgress first = CourseProgress::for_block(5, "classes_and_encapsulation");
    CourseProgress second = CourseProgress::for_block(4, "functions_references_and_const");
    first.mark_lecture_found();
    first.mark_examples_found();

    const CourseProgress& read_only = first;
    const bool const_read_works = read_only.is_complete() && read_only.number() == 5;
    const bool objects_are_independent = first.is_complete() && !second.is_complete();
    const bool invalid_input_is_contained = !CourseProgress::for_block(99, "").is_valid();

    std::cout << "bad public state accepted: " << std::boolalpha
              << public_data_accepts_impossible_state << '\n';
    std::cout << status_line(first) << '\n';
    std::cout << status_line(second) << '\n';
    std::cout << "two objects independent: " << objects_are_independent << '\n';

    const bool pass = public_data_accepts_impossible_state && const_read_works &&
                      objects_are_independent && invalid_input_is_contained;
    std::cout << "ENCAPSULATION_BREAK_LAB=" << (pass ? "PASS" : "FAIL") << '\n';
    return pass ? 0 : 1;
}
