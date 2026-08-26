class Base {
private:
    int state_{7};
};

class Derived : public Base {
public:
    int state() const { return state_; } // Base private state is inaccessible.
};

int main() { return Derived{}.state(); }
