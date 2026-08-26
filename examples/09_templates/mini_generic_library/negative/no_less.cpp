#include "mini_generic.hpp"

struct NotOrdered {
    int value{};
};

int main() {
    const NotOrdered left{1};
    const NotOrdered right{2};
    (void)mini::maximum(left, right);  // NotOrdered does not provide operator<.
}
