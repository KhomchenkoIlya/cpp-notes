#include <iostream>

class ShallowBuffer {
public:
    explicit ShallowBuffer(int value) : data_{new int{value}} {}
    ~ShallowBuffer() { delete data_; }

    int& value() { return *data_; }

private:
    int* data_; // owning pointer; generated copy duplicates only this address
};

int main() {
    ShallowBuffer first{10};
    ShallowBuffer second = first;
    second.value() = 99;
    std::cout << "shared value through shallow copies: " << first.value() << '\n';
    return 0; // both destructors try to delete the same allocation
}
