#ifndef MINI_GENERIC_HPP
#define MINI_GENERIC_HPP

#include <array>
#include <cstddef>

namespace mini {

template <typename T>
const T& maximum(const T& left, const T& right) {
    return left < right ? right : left;
}

template <typename Container, typename Value>
std::size_t count_matching(const Container& values, const Value& wanted) {
    std::size_t count{};
    for (const auto& value : values) {
        if (value == wanted) {
            ++count;
        }
    }
    return count;
}

template <typename T, std::size_t N>
class FixedBuffer {
public:
    constexpr std::size_t size() const { return N; }

    T& operator[](std::size_t index) { return values_[index]; }
    const T& operator[](std::size_t index) const { return values_[index]; }

private:
    std::array<T, N> values_{};
};

template <typename T>
struct ValueLabel {
    static const char* text() { return "value"; }
};

template <>
struct ValueLabel<bool> {
    static const char* text() { return "logical value"; }
};

}  // namespace mini

#endif
