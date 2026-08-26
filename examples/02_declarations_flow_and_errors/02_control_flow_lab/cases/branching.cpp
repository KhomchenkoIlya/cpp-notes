#include <iostream>
#include <string>
#include <vector>

int main() {
    const std::vector<int> values{4, 10, 20};
    const std::vector<std::string> expected{"low", "middle", "high"};

    for (int index = 0; index < 3; ++index) {
        std::string result;
        if (values.at(index) < 5) {
            result = "low";
        } else if (values.at(index) <= 10) {
            result = "middle";
        } else {
            result = "high";
        }
        std::cout << result << ' ';
        if (result != expected.at(index)) {
            return 1;
        }
    }
    std::cout << '\n';
    return 0;
}
