#include <iostream>

int main() {
    int values[3]{1, 2, 3};
    volatile int index{3};
    std::cout << values[index] << '\n'; // intentional out-of-bounds read
    return 0;
}
