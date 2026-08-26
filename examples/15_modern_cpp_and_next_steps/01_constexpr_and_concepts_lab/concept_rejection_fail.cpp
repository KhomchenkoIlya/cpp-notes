#include <concepts>
#include <string>

template <typename T>
concept Numeric =
    std::integral<T> ||
    std::floating_point<T>;

template <Numeric T>
T twice(T value) {
    return value + value;
}

int main() {
    const std::string value{"C++20"};
    const auto result = twice(value);
    // Expected compile failure: std::string does not satisfy Numeric.
    return static_cast<int>(result.size());
}
