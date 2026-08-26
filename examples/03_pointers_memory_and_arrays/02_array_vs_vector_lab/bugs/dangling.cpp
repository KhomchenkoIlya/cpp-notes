#include <iostream>

int main() {
    int* pointer{};
    {
        int local{42};
        pointer = &local;
    }
    std::cout << *pointer << '\n'; // intentional use after lifetime
    return 0;
}
