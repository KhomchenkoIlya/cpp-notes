#include <memory>
#include <utility>

int main() {
    const auto owner = std::make_unique<int>(42);
    auto transferred = std::move(owner);
    // Expected compile failure:
    // unique_ptr move construction needs a non-const source.
    return *transferred;
}
