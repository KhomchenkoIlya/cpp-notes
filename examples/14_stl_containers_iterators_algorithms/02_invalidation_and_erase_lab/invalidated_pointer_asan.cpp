#include <iostream>
#include <vector>

int main() {
    std::vector<int> values;
    values.reserve(1);
    values.push_back(42);

    int* stale = values.data();
    const auto old_capacity = values.capacity();

    while (values.capacity() == old_capacity) {
        values.push_back(7);
    }

    std::cout << *stale << '\n';
    // Intentionally invalid use after vector reallocation.
    // This executable is only run under AddressSanitizer as an expected failure.
}
