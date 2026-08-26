void show(int value) {}
void show(double value) {}

int main() {
    show(1L); // Expected error: neither conversion is better.
}
