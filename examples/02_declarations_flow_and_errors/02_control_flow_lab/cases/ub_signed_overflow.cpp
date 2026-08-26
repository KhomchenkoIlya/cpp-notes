#include <iostream>

int main() {
    int largest{};
    std::cin >> largest;
    const int overflow = largest + 1;
    std::cout << overflow << '\n';
    return 0;
}
