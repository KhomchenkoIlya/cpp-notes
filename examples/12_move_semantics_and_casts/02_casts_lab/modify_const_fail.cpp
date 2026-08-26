int main() {
    const int value = 7;
    const int* view = &value;
    *view = 9; // Expected compile failure: write through pointer-to-const.
    return value;
}
