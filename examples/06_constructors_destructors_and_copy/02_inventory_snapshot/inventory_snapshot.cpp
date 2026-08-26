#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class CourseBlock {
public:
    CourseBlock(const std::filesystem::path& root, int number,
                const std::string& full_name)
        : number_{number},
          full_name_{full_name},
          slug_{full_name.substr(3)},
          lecture_path_{root / "lectures" / (full_name + ".tex")},
          examples_path_{root / "examples" / full_name},
          lecture_exists_{std::filesystem::is_regular_file(lecture_path_)},
          examples_exist_{std::filesystem::is_directory(examples_path_)},
          lecture_lines_{lecture_exists_ ? count_lines(lecture_path_) : 0} {}

    int number() const { return number_; }
    const std::string& full_name() const { return full_name_; }
    const std::string& slug() const { return slug_; }
    const std::filesystem::path& lecture_path() const { return lecture_path_; }
    const std::filesystem::path& examples_path() const { return examples_path_; }
    bool lecture_exists() const { return lecture_exists_; }
    bool examples_exist() const { return examples_exist_; }
    std::size_t lecture_lines() const { return lecture_lines_; }

private:
    static std::size_t count_lines(const std::filesystem::path& file) {
        std::ifstream input{file};
        std::size_t count{};
        std::string line;
        while (std::getline(input, line)) {
            ++count;
        }
        return count;
    }

    int number_;
    std::string full_name_;
    std::string slug_;
    std::filesystem::path lecture_path_;
    std::filesystem::path examples_path_;
    bool lecture_exists_;
    bool examples_exist_;
    std::size_t lecture_lines_;
};

class MutableBlockSnapshot {
public:
    explicit MutableBlockSnapshot(const CourseBlock& block)
        : number_{block.number()},
          label_{block.full_name()},
          notes_{"captured from " + block.lecture_path().string()} {}

    int number() const { return number_; }
    const std::string& label() const { return label_; }
    const std::vector<std::string>& notes() const { return notes_; }
    void set_label(const std::string& label) { label_ = label; }
    void add_note(const std::string& note) { notes_.push_back(note); }

private:
    int number_;
    std::string label_;
    std::vector<std::string> notes_;
};

using BlockNames = std::array<const char*, 15>;

std::vector<CourseBlock> collect_blocks(const std::filesystem::path& root,
                                        const BlockNames& names) {
    std::vector<CourseBlock> blocks;
    blocks.reserve(names.size());
    for (std::size_t index = 0; index < names.size(); ++index) {
        blocks.emplace_back(root, static_cast<int>(index + 1), names[index]);
    }
    return blocks;
}

int main(int argc, char* argv[]) {
    std::filesystem::path root = std::filesystem::current_path();
    if (argc == 3 && std::string{argv[1]} == "--root") {
        root = argv[2];
    } else if (argc != 1) {
        std::cerr << "Usage: inventory_snapshot [--root PATH]\n";
        return 2;
    }
    if (!std::filesystem::is_directory(root / "lectures") ||
        !std::filesystem::is_directory(root / "examples")) {
        std::cerr << "Not a course root: " << root << '\n';
        return 2;
    }

    const BlockNames names{
        "01_intro_and_first_program", "02_declarations_flow_and_errors",
        "03_pointers_memory_and_arrays", "04_functions_references_and_const",
        "05_classes_and_encapsulation", "06_constructors_destructors_and_copy",
        "07_inheritance_and_polymorphism", "08_design_solid_and_patterns",
        "09_templates", "10_exceptions_and_raii", "11_smart_pointers",
        "12_move_semantics_and_casts", "13_type_deduction_lambdas_and_function",
        "14_stl_containers_iterators_algorithms", "15_modern_cpp_and_next_steps"};

    const std::vector<CourseBlock> inventory = collect_blocks(root, names);
    std::vector<CourseBlock> inventory_copy = inventory; // copy construction
    std::vector<CourseBlock> inventory_assigned;
    inventory_assigned = inventory; // copy assignment of existing vector

    MutableBlockSnapshot original{inventory.at(5)};
    MutableBlockSnapshot snapshot = original;
    snapshot.set_label("06_snapshot_for_analysis");
    snapshot.add_note("modified only in the independent copy");
    MutableBlockSnapshot assigned{inventory.at(0)};
    assigned = original;
    assigned.add_note("assigned snapshot has its own vector");

    const bool course_data_copied = inventory_copy.size() == inventory.size() &&
                                    inventory_assigned.size() == inventory.size() &&
                                    inventory_copy.at(5).full_name() == inventory.at(5).full_name() &&
                                    inventory_assigned.at(5).lecture_lines() ==
                                        inventory.at(5).lecture_lines();
    const bool snapshot_independent =
        original.label() == "06_constructors_destructors_and_copy" &&
        original.notes().size() == 1 && snapshot.label() == "06_snapshot_for_analysis" &&
        snapshot.notes().size() == 2 && assigned.notes().size() == 2;

    std::cout << std::boolalpha
              << "CourseBlock copy construction: " << course_data_copied << '\n'
              << "CourseBlock custom copy members needed: false\n"
              << "snapshot original unchanged: " << snapshot_independent << '\n'
              << "inventory entries: " << inventory.size() << '\n';
    const bool pass = course_data_copied && snapshot_independent;
    std::cout << "INVENTORY_SNAPSHOT=" << (pass ? "PASS" : "FAIL") << '\n';
    return pass ? 0 : 1;
}
