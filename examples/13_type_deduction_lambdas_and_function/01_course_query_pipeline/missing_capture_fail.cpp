int main() {
    int threshold = 10;

    auto predicate = [](int value) {
        return value >= threshold;
        // Expected compile failure:
        // threshold is a local variable but is not captured.
    };

    return predicate(12);
}
