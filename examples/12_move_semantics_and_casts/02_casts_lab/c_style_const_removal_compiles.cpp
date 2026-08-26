int main() {
    const int value = 7;

    int* writable_type = (int*)&value;
    // This C-style cast compiles on the teaching toolchain.
    // We deliberately DO NOT write through writable_type:
    // modifying the genuinely const object would be undefined behavior.

    return writable_type == nullptr;
}
