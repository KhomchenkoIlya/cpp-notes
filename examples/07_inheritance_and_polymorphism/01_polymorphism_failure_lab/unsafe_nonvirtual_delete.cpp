#include <iostream>

class UnsafeBase {
public:
    ~UnsafeBase() { std::cout << "destroy UnsafeBase\n"; }
};

class UnsafeDerived final : public UnsafeBase {
public:
    UnsafeDerived() : payload_{new int[64]{}} {}
    ~UnsafeDerived() {
        std::cout << "destroy UnsafeDerived\n";
        delete[] payload_;
    }
private:
    int* payload_;
};

int main() {
    UnsafeBase* base{new UnsafeDerived};
    delete base; // Undefined behavior: the static base type has no virtual destructor.
}
