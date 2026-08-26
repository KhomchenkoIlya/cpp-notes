#include <algorithm>
#include <compare>
#include <iostream>
#include <numeric>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace {

struct CourseBlock {
    int number{};
    std::string slug;
    int examples{};
    bool complete{};
};

struct Version {
    int major{};
    int minor{};

    auto operator<=>(const Version&) const = default;
};

int sum_span(std::span<const int> values) {
    return std::accumulate(values.begin(), values.end(), 0);
}

bool ranges_case() {
    std::vector<CourseBlock> blocks{
        {13, "lambdas", 2, true},
        {15, "modern_cpp", 2, false},
        {14, "stl", 2, true},
        {12, "move", 2, true}
    };

    std::ranges::sort(blocks, std::less{}, &CourseBlock::number);

    auto selected =
        blocks
        | std::views::filter([](const CourseBlock& block) {
              return block.number >= 13;
          })
        | std::views::transform([](const CourseBlock& block) {
              return std::string_view{block.slug};
          });

    std::vector<std::string_view> slugs;
    for (std::string_view slug : selected) {
        slugs.push_back(slug);
    }

    const bool pass =
        blocks.front().number == 12 &&
        blocks.back().number == 15 &&
        slugs.size() == 3 &&
        slugs[0] == "lambdas" &&
        slugs[1] == "stl" &&
        slugs[2] == "modern_cpp";

    std::cout
        << "selected=" << slugs.size() << '\n'
        << "RANGES=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool span_case() {
    const int raw[]{1, 2, 3, 4};
    const std::vector<int> dynamic{5, 6, 7};

    const int raw_sum = sum_span(raw);
    const int vector_sum = sum_span(dynamic);

    const bool pass =
        raw_sum == 10 &&
        vector_sum == 18;

    std::cout
        << "raw_sum=" << raw_sum << '\n'
        << "vector_sum=" << vector_sum << '\n'
        << "SPAN=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool designated_case() {
    const CourseBlock block{
        .number = 15,
        .slug = "modern_cpp_and_next_steps",
        .examples = 2,
        .complete = false
    };

    const bool pass =
        block.number == 15 &&
        block.slug == "modern_cpp_and_next_steps" &&
        block.examples == 2 &&
        !block.complete;

    std::cout
        << "slug=" << block.slug << '\n'
        << "DESIGNATED_INITIALIZER="
        << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool comparison_case() {
    const Version old_version{17, 0};
    const Version modern_version{20, 0};
    const Version same_version{20, 0};

    const bool pass =
        old_version < modern_version &&
        modern_version > old_version &&
        modern_version == same_version;

    std::cout
        << "COMPARISON=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool erase_if_case() {
    std::vector<int> values{1, -1, 2, -2, 3, -3};

    const auto removed = std::erase_if(
        values,
        [](int value) {
            return value < 0;
        }
    );

    const bool pass =
        removed == 3 &&
        values == std::vector<int>({1, 2, 3});

    std::cout
        << "removed=" << removed << '\n'
        << "ERASE_IF=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool starts_with_case() {
    const std::string lecture{"lecture_15_modern_cpp"};
    const std::string example{"example_15"};

    const bool pass =
        lecture.starts_with("lecture_") &&
        !example.starts_with("lecture_");

    std::cout
        << "STARTS_WITH=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

} // namespace

int main(int argc, char* argv[]) {
    std::string selected{"all"};

    if (argc == 3 && std::string{argv[1]} == "--case") {
        selected = argv[2];
    } else if (argc != 1) {
        std::cerr
            << "Usage: cpp20_feature_tour "
            << "[--case ranges|span|designated|comparison|erase_if|starts_with]\n";
        return 2;
    }

    bool pass = true;

    if (selected == "all" || selected == "ranges") pass = ranges_case() && pass;
    if (selected == "all" || selected == "span") pass = span_case() && pass;
    if (selected == "all" || selected == "designated") pass = designated_case() && pass;
    if (selected == "all" || selected == "comparison") pass = comparison_case() && pass;
    if (selected == "all" || selected == "erase_if") pass = erase_if_case() && pass;
    if (selected == "all" || selected == "starts_with") pass = starts_with_case() && pass;

    if (selected != "all" &&
        selected != "ranges" &&
        selected != "span" &&
        selected != "designated" &&
        selected != "comparison" &&
        selected != "erase_if" &&
        selected != "starts_with") {
        std::cerr << "Unknown case: " << selected << '\n';
        return 2;
    }

    std::cout
        << "CPP20_FEATURE_TOUR="
        << (pass ? "PASS" : "FAIL") << '\n';

    return pass ? 0 : 1;
}
