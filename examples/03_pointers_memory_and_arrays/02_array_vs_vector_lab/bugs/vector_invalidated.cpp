#include <iostream>
#include <vector>

int main() {
    std::vector<int> values{42};
    int* old = values.data();
    const auto old_capacity = values.capacity();
    while (values.capacity() == old_capacity) {
        values.push_back(7);
    }
    std::cout << *old << '\n'; // intentional access through invalidated pointer
    return 0;
}
