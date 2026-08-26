consteval int square_now(int value) {
    return value * value;
}

int main(int argc, char**) {
    const int runtime_value = argc;
    const int result = square_now(runtime_value);
    // Expected compile failure: runtime value in immediate call.
    return result;
}
