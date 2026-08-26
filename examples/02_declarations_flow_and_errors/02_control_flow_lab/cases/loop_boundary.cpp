#include <iostream>

int main() {
    int count{};
    int sum{};

    for (int value = 1; value <= 5; ++value) {
        ++count;
        sum += value;
    }

    std::cout << "count=" << count << " sum=" << sum << '\n';
    if (count != 5 || sum != 15) {
        return 1;
    }
    return 0;
}
