#include <iostream>

int main() {
    int sum{};
    for (int value = 1; value < 5; ++value) { // Ошибка: значение 5 пропущено.
        sum += value;
    }
    std::cout << "expected=15 actual=" << sum << '\n';
    if (sum != 15) {
        return 1;
    }
    return 0;
}
