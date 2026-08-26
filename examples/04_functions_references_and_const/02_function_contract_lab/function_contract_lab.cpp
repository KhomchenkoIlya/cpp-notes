#include <iostream>
#include <string>
#include <vector>

void change_copy(int value) {
    value = 99;
    std::cout << "inside value=" << value << '\n';
}

void add_point(int& score) {
    ++score;
}

std::size_t total_characters(const std::vector<std::string>& words) {
    std::size_t result{};
    for (const std::string& word : words) {
        result += word.size();
    }
    return result;
}

int square(int value) {
    return value * value;
}

void square_out(int value, int& output) {
    output = value * value;
}

int twice(int value) {
    return value * 2;
}

double twice(double value) {
    return value * 2.0;
}

int add(int left, int right) {
    return left + right;
}

int subtract(int left, int right) {
    return left - right;
}

int apply(int left, int right, int (*operation)(int, int)) {
    return operation(left, right);
}

int local_scope_case() {
    const int local{17};
    return local; // The returned value survives; the local object does not.
}

int main() {
    int original{10};
    change_copy(original);
    std::cout << "after value=" << original << '\n';
    if (original != 10) {
        return 1;
    }

    add_point(original);
    std::cout << "after reference=" << original << '\n';
    if (original != 11) {
        return 1;
    }

    const std::vector<std::string> words{"functions", "express", "contracts"};
    const std::size_t characters = total_characters(words);
    std::cout << "const reference characters=" << characters << '\n';
    if (characters != 25) {
        return 1;
    }

    const int returned = square(6);
    int output{};
    square_out(6, output);
    std::cout << "return=" << returned << " output=" << output << '\n';
    if (returned != 36 || output != 36) {
        return 1;
    }

    std::cout << "local value=" << local_scope_case() << '\n';
    std::cout << "overloads=" << twice(4) << ',' << twice(2.5) << '\n';
    if (twice(4) != 8 || twice(2.5) != 5.0) {
        return 1;
    }

    int (*operation)(int, int) = add;
    const int sum = apply(9, 4, operation);
    operation = subtract;
    const int difference = apply(9, 4, operation);
    std::cout << "function pointer=" << sum << ',' << difference << '\n';
    if (sum != 13 || difference != 5) {
        return 1;
    }

    std::cout << "FUNCTION_CONTRACT_LAB=PASS\n";
    return 0;
}
