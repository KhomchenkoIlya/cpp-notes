#include <functional>
#include <memory>
#include <utility>

int main() {
    auto owner = std::make_unique<int>(42);

    auto move_only =
        [value = std::move(owner)]() {
            return *value;
        };

    std::function<int()> callback = std::move(move_only);
    // Expected compile failure in C++17:
    // std::function target must be CopyConstructible.

    return callback();
}
