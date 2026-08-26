class Base {
public:
    virtual ~Base() = default;
    virtual int evaluate() const { return 0; }
};

class Derived : public Base {
public:
    int evaluate() override { return 1; } // Missing const: no override exists.
};

int main() { return 0; }
