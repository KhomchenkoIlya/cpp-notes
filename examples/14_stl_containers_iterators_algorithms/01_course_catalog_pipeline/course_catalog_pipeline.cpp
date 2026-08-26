#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {

struct CourseBlock {
    int number{};
    std::string slug;
    int examples{};
    bool complete{};
};

std::vector<CourseBlock> make_catalog() {
    return {
        {1,  "intro_and_first_program", 2, true},
        {2,  "declarations_flow_and_errors", 2, true},
        {3,  "pointers_memory_and_arrays", 2, true},
        {4,  "functions_references_and_const", 2, true},
        {5,  "classes_and_encapsulation", 2, true},
        {6,  "constructors_destructors_and_copy", 2, true},
        {7,  "inheritance_and_polymorphism", 2, true},
        {8,  "design_solid_and_patterns", 2, true},
        {9,  "templates", 2, true},
        {10, "exceptions_and_raii", 2, true},
        {11, "smart_pointers", 2, true},
        {12, "move_semantics_and_casts", 2, true},
        {13, "type_deduction_lambdas_and_function", 2, true},
        {14, "stl_containers_iterators_algorithms", 2, false},
        {15, "modern_cpp_and_next_steps", 0, false}
    };
}

using Index = std::unordered_map<int, std::size_t>;

Index build_index(const std::vector<CourseBlock>& blocks) {
    Index index;
    index.reserve(blocks.size());

    for (std::size_t i = 0; i < blocks.size(); ++i) {
        index.emplace(blocks[i].number, i);
    }

    return index;
}

std::optional<CourseBlock> find_by_index(
    const std::vector<CourseBlock>& blocks,
    const Index& index,
    int number) {

    const auto position = index.find(number);

    if (position == index.end()) {
        return std::nullopt;
    }

    return blocks[position->second];
}

std::optional<CourseBlock> find_linear(
    const std::vector<CourseBlock>& blocks,
    int number) {

    const auto it = std::find_if(
        blocks.cbegin(),
        blocks.cend(),
        [number](const CourseBlock& block) {
            return block.number == number;
        }
    );

    if (it == blocks.cend()) {
        return std::nullopt;
    }

    return *it;
}

bool lookup_case() {
    const auto blocks = make_catalog();
    const auto index = build_index(blocks);

    const auto indexed = find_by_index(blocks, index, 12);
    const auto linear = find_linear(blocks, 12);
    const auto missing = find_by_index(blocks, index, 99);

    const bool pass =
        index.size() == blocks.size() &&
        indexed.has_value() &&
        linear.has_value() &&
        indexed->slug == "move_semantics_and_casts" &&
        linear->slug == indexed->slug &&
        !missing.has_value();

    std::cout
        << "index_size=" << index.size() << '\n'
        << "indexed=" << (indexed ? indexed->slug : "none") << '\n'
        << "linear=" << (linear ? linear->slug : "none") << '\n'
        << "LOOKUP=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool filter_sort_case() {
    const auto blocks = make_catalog();

    std::vector<CourseBlock> completed;
    std::copy_if(
        blocks.cbegin(),
        blocks.cend(),
        std::back_inserter(completed),
        [](const CourseBlock& block) {
            return block.complete;
        }
    );

    std::sort(
        completed.begin(),
        completed.end(),
        [](const CourseBlock& left, const CourseBlock& right) {
            return left.number > right.number;
        }
    );

    const bool pass =
        completed.size() == 13 &&
        completed.front().number == 13 &&
        completed.back().number == 1 &&
        std::all_of(
            completed.cbegin(),
            completed.cend(),
            [](const CourseBlock& block) {
                return block.complete;
            }
        );

    std::cout
        << "completed=" << completed.size() << '\n'
        << "first_number=" << completed.front().number << '\n'
        << "last_number=" << completed.back().number << '\n'
        << "FILTER_SORT=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool transform_accumulate_case() {
    const auto blocks = make_catalog();

    std::vector<std::string> slugs;
    slugs.reserve(blocks.size());

    std::transform(
        blocks.cbegin(),
        blocks.cend(),
        std::back_inserter(slugs),
        [](const CourseBlock& block) {
            return block.slug;
        }
    );

    const int total_examples = std::accumulate(
        blocks.cbegin(),
        blocks.cend(),
        0,
        [](int total, const CourseBlock& block) {
            return total + block.examples;
        }
    );

    const auto rich_blocks = std::count_if(
        blocks.cbegin(),
        blocks.cend(),
        [](const CourseBlock& block) {
            return block.examples >= 2;
        }
    );

    const bool pass =
        slugs.size() == 15 &&
        slugs.front() == "intro_and_first_program" &&
        slugs.back() == "modern_cpp_and_next_steps" &&
        total_examples == 28 &&
        rich_blocks == 14;

    std::cout
        << "slugs=" << slugs.size() << '\n'
        << "total_examples=" << total_examples << '\n'
        << "rich_blocks=" << rich_blocks << '\n'
        << "TRANSFORM_ACCUMULATE=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool ordered_summary_case() {
    const auto blocks = make_catalog();

    std::map<std::string, int> summary;

    for (const auto& block : blocks) {
        ++summary[block.complete ? "complete" : "pending"];
    }

    const std::vector<std::string> keys = [&summary] {
        std::vector<std::string> result;
        for (const auto& [key, value] : summary) {
            (void)value;
            result.push_back(key);
        }
        return result;
    }();

    const bool pass =
        summary.at("complete") == 13 &&
        summary.at("pending") == 2 &&
        keys.size() == 2 &&
        keys[0] == "complete" &&
        keys[1] == "pending";

    std::cout
        << "complete=" << summary.at("complete") << '\n'
        << "pending=" << summary.at("pending") << '\n'
        << "first_key=" << keys.front() << '\n'
        << "ORDERED_SUMMARY=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool pipeline_case() {
    const auto blocks = make_catalog();

    std::vector<CourseBlock> selected;
    std::copy_if(
        blocks.cbegin(),
        blocks.cend(),
        std::back_inserter(selected),
        [](const CourseBlock& block) {
            return block.examples >= 2 && block.number >= 10;
        }
    );

    std::sort(
        selected.begin(),
        selected.end(),
        [](const CourseBlock& left, const CourseBlock& right) {
            return left.number < right.number;
        }
    );

    std::vector<std::string> slugs;
    std::transform(
        selected.cbegin(),
        selected.cend(),
        std::back_inserter(slugs),
        [](const CourseBlock& block) {
            return block.slug;
        }
    );

    const int examples = std::accumulate(
        selected.cbegin(),
        selected.cend(),
        0,
        [](int total, const CourseBlock& block) {
            return total + block.examples;
        }
    );

    const bool pass =
        selected.size() == 5 &&
        selected.front().number == 10 &&
        selected.back().number == 14 &&
        slugs.front() == "exceptions_and_raii" &&
        slugs.back() == "stl_containers_iterators_algorithms" &&
        examples == 10;

    std::cout
        << "selected=" << selected.size() << '\n'
        << "examples=" << examples << '\n'
        << "PIPELINE=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

} // namespace

int main(int argc, char* argv[]) {
    std::string selected{"all"};

    if (argc == 3 && std::string{argv[1]} == "--case") {
        selected = argv[2];
    } else if (argc != 1) {
        std::cerr
            << "Usage: course_catalog_pipeline "
            << "[--case lookup|filter|transform|summary|pipeline]\n";
        return 2;
    }

    bool pass = true;

    if (selected == "all" || selected == "lookup") {
        pass = lookup_case() && pass;
    }
    if (selected == "all" || selected == "filter") {
        pass = filter_sort_case() && pass;
    }
    if (selected == "all" || selected == "transform") {
        pass = transform_accumulate_case() && pass;
    }
    if (selected == "all" || selected == "summary") {
        pass = ordered_summary_case() && pass;
    }
    if (selected == "all" || selected == "pipeline") {
        pass = pipeline_case() && pass;
    }

    if (selected != "all" &&
        selected != "lookup" &&
        selected != "filter" &&
        selected != "transform" &&
        selected != "summary" &&
        selected != "pipeline") {
        std::cerr << "Unknown case: " << selected << '\n';
        return 2;
    }

    std::cout
        << "COURSE_CATALOG_PIPELINE="
        << (pass ? "PASS" : "FAIL") << '\n';

    return pass ? 0 : 1;
}
