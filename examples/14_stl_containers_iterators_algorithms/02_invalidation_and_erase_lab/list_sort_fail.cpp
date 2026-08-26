#include <algorithm>
#include <list>

int main() {
    std::list<int> values{4, 1, 3, 2};

    std::sort(values.begin(), values.end());
    // Expected compile failure:
    // std::sort requires random-access iterators.

    return 0;
}
