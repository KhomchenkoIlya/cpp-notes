#include <iostream>

int* bad_address() {
    int local{42};
    return &local; // Deliberate dangling pointer for the sanitizer experiment.
}

int main() {
    int* pointer = bad_address();
    std::cout << *pointer << '\n';
}
