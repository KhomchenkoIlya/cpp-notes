struct PlainBase {
};

struct Derived : PlainBase {
};

int main() {
    Derived object;
    PlainBase* base = &object;

    auto* derived = dynamic_cast<Derived*>(base);
    // Expected compile failure: source type is not polymorphic.
    return derived == nullptr;
}
