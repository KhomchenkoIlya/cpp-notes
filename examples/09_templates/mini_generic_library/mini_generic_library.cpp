#include "mini_generic.hpp"

#include <iostream>
#include <string>
#include <vector>

struct Revision {
    int number{};
};

bool operator<(const Revision& left, const Revision& right) {
    return left.number < right.number;
}

int main() {
    const int max_int = mini::maximum(4, 9);                // T is deduced as int.
    const double max_double = mini::maximum<double>(2, 3.5);  // T is explicit.
    const std::string max_string = mini::maximum(std::string{"alpha"},
                                                  std::string{"beta"});
    const Revision latest = mini::maximum(Revision{8}, Revision{9});

    const std::vector<int> numbers{1, 2, 2, 3};
    const std::vector<std::string> words{"template", "class", "template"};
    const std::size_t twos = mini::count_matching(numbers, 2);
    const std::size_t templates = mini::count_matching(words, std::string{"template"});

    mini::FixedBuffer<int, 3> scores;
    scores[0] = 7;
    scores[1] = 8;
    scores[2] = 9;
    mini::FixedBuffer<std::string, 2> names;
    names[0] = "function";
    names[1] = "class";

    const bool pass = max_int == 9 && max_double == 3.5 && max_string == "beta" &&
                      latest.number == 9 && twos == 2 && templates == 2 &&
                      scores.size() == 3 && scores[2] == 9 && names.size() == 2 &&
                      names[1] == "class" &&
                      std::string{mini::ValueLabel<int>::text()} == "value" &&
                      std::string{mini::ValueLabel<bool>::text()} == "logical value";

    std::cout << "maximum<int>=" << max_int << "\nmaximum<double>=" << max_double
              << "\nmaximum<string>=" << max_string << "\nmaximum<Revision>="
              << latest.number << "\nmatching<int>=" << twos
              << "\nmatching<string>=" << templates << "\nFixedBuffer<int,3>.size="
              << scores.size() << "\nFixedBuffer<string,2>.size=" << names.size()
              << "\nspecialization<bool>=" << mini::ValueLabel<bool>::text()
              << "\nMINI_GENERIC_LIBRARY=" << (pass ? "PASS" : "FAIL") << '\n';
    return pass ? 0 : 1;
}
