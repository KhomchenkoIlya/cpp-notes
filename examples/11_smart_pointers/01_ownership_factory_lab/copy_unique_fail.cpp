#include <memory>

int main() {
    auto first = std::make_unique<int>(42);
    auto second = first; // Expected compile failure: unique_ptr is not copyable.
    return *second;
}
