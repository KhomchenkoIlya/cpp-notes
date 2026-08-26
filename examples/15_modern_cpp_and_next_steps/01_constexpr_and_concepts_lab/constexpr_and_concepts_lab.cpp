#include <concepts>
#include <iostream>
#include <string>
#include <vector>

namespace {

constexpr int square(int value) {
    return value * value;
}

constexpr int triangular(int value) {
    int result = 0;
    for (int current = 1; current <= value; ++current) {
        result += current;
    }
    return result;
}

consteval int make_id(int group, int number) {
    return group * 1000 + number;
}

constinit int global_multiplier = 3;

template <typename T>
concept Numeric =
    std::integral<T> ||
    std::floating_point<T>;

template <typename T>
concept HasSize = requires(const T& value) {
    { value.size() } -> std::convertible_to<std::size_t>;
};

template <Numeric T>
constexpr T twice(T value) {
    return value + value;
}

template <HasSize T>
std::size_t measured_size(const T& value) {
    return value.size();
}

template <typename T>
std::string describe(const T&) {
    return "generic";
}

template <std::integral T>
std::string describe(const T&) {
    return "integral";
}

bool constexpr_case() {
    static_assert(square(5) == 25);
    static_assert(triangular(5) == 15);

    int runtime_value = 7;
    const int runtime_square = square(runtime_value);

    const bool pass =
        runtime_square == 49 &&
        triangular(runtime_value) == 28;

    std::cout
        << "runtime_square=" << runtime_square << '\n'
        << "CONSTEXPR=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool consteval_case() {
    constexpr int id = make_id(12, 34);
    static_assert(id == 12034);

    const bool pass = id == 12034;

    std::cout
        << "id=" << id << '\n'
        << "CONSTEVAL=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool constinit_case() {
    const int before = global_multiplier;
    global_multiplier = 5;
    const int after = global_multiplier;
    global_multiplier = 3;

    const bool pass =
        before == 3 &&
        after == 5 &&
        global_multiplier == 3;

    std::cout
        << "before=" << before << '\n'
        << "after=" << after << '\n'
        << "CONSTINIT=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool concepts_case() {
    static_assert(Numeric<int>);
    static_assert(Numeric<double>);
    static_assert(!Numeric<std::string>);
    static_assert(HasSize<std::string>);
    static_assert(HasSize<std::vector<int>>);
    static_assert(!HasSize<int>);
    static_assert(twice(21) == 42);
    static_assert(twice(1.5) == 3.0);

    const std::vector<int> values{1, 2, 3, 4};

    const bool pass =
        measured_size(values) == 4 &&
        measured_size(std::string{"abc"}) == 3 &&
        twice(10) == 20;

    std::cout
        << "vector_size=" << measured_size(values) << '\n'
        << "CONCEPTS=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool constrained_overload_case() {
    const auto integer_description = describe(42);
    const auto string_description = describe(std::string{"C++20"});

    const bool pass =
        integer_description == "integral" &&
        string_description == "generic";

    std::cout
        << "int=" << integer_description << '\n'
        << "string=" << string_description << '\n'
        << "CONSTRAINED_OVERLOAD="
        << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

} // namespace

int main(int argc, char* argv[]) {
    std::string selected{"all"};

    if (argc == 3 && std::string{argv[1]} == "--case") {
        selected = argv[2];
    } else if (argc != 1) {
        std::cerr
            << "Usage: constexpr_and_concepts_lab "
            << "[--case constexpr|consteval|constinit|concepts|overload]\n";
        return 2;
    }

    bool pass = true;

    if (selected == "all" || selected == "constexpr") {
        pass = constexpr_case() && pass;
    }
    if (selected == "all" || selected == "consteval") {
        pass = consteval_case() && pass;
    }
    if (selected == "all" || selected == "constinit") {
        pass = constinit_case() && pass;
    }
    if (selected == "all" || selected == "concepts") {
        pass = concepts_case() && pass;
    }
    if (selected == "all" || selected == "overload") {
        pass = constrained_overload_case() && pass;
    }

    if (selected != "all" &&
        selected != "constexpr" &&
        selected != "consteval" &&
        selected != "constinit" &&
        selected != "concepts" &&
        selected != "overload") {
        std::cerr << "Unknown case: " << selected << '\n';
        return 2;
    }

    std::cout
        << "CONSTEXPR_CONCEPTS_LAB="
        << (pass ? "PASS" : "FAIL") << '\n';

    return pass ? 0 : 1;
}
