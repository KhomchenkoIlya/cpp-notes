#include <iostream>

int main() {
    const int denominator{};
    int checks{};

    ++checks;
    const bool guarded = denominator != 0 && 12 / denominator > 2;

    std::cout << std::boolalpha
              << "guarded=" << guarded << " checks=" << checks << '\n';
    if (guarded) {
        return 1;
    }
    return 0;
}
