#include <iostream>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

struct CourseBlock {
    int number{};
    std::string slug;
    int examples{};
    bool complete{};
};

template <typename Predicate>
std::optional<CourseBlock>
find_first(const std::vector<CourseBlock>& blocks, Predicate predicate) {
    for (const auto& block : blocks) {
        if (predicate(block)) {
            return block;
        }
    }

    return std::nullopt;
}

template <typename Predicate>
std::size_t count_if_local(
    const std::vector<CourseBlock>& blocks,
    Predicate predicate) {

    std::size_t count = 0;

    for (const auto& block : blocks) {
        if (predicate(block)) {
            ++count;
        }
    }

    return count;
}

bool deduction_case() {
    const int source = 42;
    const int& source_ref = source;

    auto copied = source_ref;
    const auto& borrowed = source_ref;

    static_assert(std::is_same_v<decltype(copied), int>);
    static_assert(std::is_same_v<decltype(borrowed), const int&>);
    static_assert(std::is_same_v<decltype(source), const int>);
    static_assert(std::is_same_v<decltype((source)), const int&>);

    copied = 43;

    const bool pass =
        copied == 43 &&
        borrowed == 42 &&
        source == 42;

    std::cout
        << "copied=" << copied << '\n'
        << "borrowed=" << borrowed << '\n'
        << "TYPE_DEDUCTION=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool structured_binding_ctad_case() {
    std::pair summary{3, std::string{"ready"}};

    static_assert(
        std::is_same_v<
            decltype(summary),
            std::pair<int, std::string>
        >
    );

    const auto& [count, state] = summary;

    const bool pass =
        count == 3 &&
        state == "ready";

    std::cout
        << "count=" << count << '\n'
        << "state=" << state << '\n'
        << "STRUCTURED_BINDING_CTAD="
        << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool generic_lambda_case() {
    const auto greater = [](const auto& left, const auto& right) {
        return left > right;
    };

    const bool pass =
        greater(7, 3) &&
        greater(2.5, 1.5) &&
        greater(std::string{"z"}, std::string{"a"});

    std::cout
        << "GENERIC_LAMBDA="
        << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

std::vector<CourseBlock> sample_blocks() {
    return {
        {11, "smart_pointers", 2, true},
        {12, "move_semantics_and_casts", 2, true},
        {13, "type_deduction_lambdas_and_function", 2, false},
        {14, "stl_containers_iterators_algorithms", 0, false}
    };
}

bool optional_query_case() {
    const auto blocks = sample_blocks();
    const int min_examples = 2;

    const auto found = find_first(
        blocks,
        [min_examples](const CourseBlock& block) {
            return !block.complete && block.examples >= min_examples;
        }
    );

    const auto missing = find_first(
        blocks,
        [](const CourseBlock& block) {
            return block.number == 99;
        }
    );

    const bool pass =
        found.has_value() &&
        found->number == 13 &&
        found->slug == "type_deduction_lambdas_and_function" &&
        !missing.has_value();

    std::cout
        << "found=" << (found ? found->slug : "none") << '\n'
        << "missing=" << (missing ? "unexpected" : "none") << '\n'
        << "OPTIONAL_QUERY=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool capture_case() {
    const auto blocks = sample_blocks();

    int threshold = 12;
    const auto by_value = [threshold](const CourseBlock& block) {
        return block.number >= threshold;
    };

    const std::size_t first_count = count_if_local(blocks, by_value);

    threshold = 14;
    const std::size_t second_count = count_if_local(blocks, by_value);

    int observed = 0;
    auto by_reference = [&observed](const CourseBlock& block) {
        if (block.complete) {
            ++observed;
        }
        return block.complete;
    };

    const std::size_t complete_count =
        count_if_local(blocks, by_reference);

    const bool pass =
        first_count == 3 &&
        second_count == 3 &&
        complete_count == 2 &&
        observed == 2;

    std::cout
        << "value_capture_count=" << first_count << '\n'
        << "value_capture_after_outer_change=" << second_count << '\n'
        << "reference_capture_observed=" << observed << '\n'
        << "CAPTURES=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

} // namespace

int main(int argc, char* argv[]) {
    std::string selected{"all"};

    if (argc == 3 && std::string{argv[1]} == "--case") {
        selected = argv[2];
    } else if (argc != 1) {
        std::cerr
            << "Usage: course_query_pipeline "
            << "[--case deduction|structured|generic|optional|capture]\n";
        return 2;
    }

    bool pass = true;

    if (selected == "all" || selected == "deduction") {
        pass = deduction_case() && pass;
    }
    if (selected == "all" || selected == "structured") {
        pass = structured_binding_ctad_case() && pass;
    }
    if (selected == "all" || selected == "generic") {
        pass = generic_lambda_case() && pass;
    }
    if (selected == "all" || selected == "optional") {
        pass = optional_query_case() && pass;
    }
    if (selected == "all" || selected == "capture") {
        pass = capture_case() && pass;
    }

    if (selected != "all" &&
        selected != "deduction" &&
        selected != "structured" &&
        selected != "generic" &&
        selected != "optional" &&
        selected != "capture") {
        std::cerr << "Unknown case: " << selected << '\n';
        return 2;
    }

    std::cout
        << "COURSE_QUERY_PIPELINE="
        << (pass ? "PASS" : "FAIL") << '\n';

    return pass ? 0 : 1;
}
