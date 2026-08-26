#include <string>

void rename(const std::string& name) {
    name += "!"; // Expected error: mutation through a const reference.
}
