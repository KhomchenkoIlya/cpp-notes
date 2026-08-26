#include <memory>

class Packet {
public:
    Packet() : payload_{std::make_unique<int>(42)} {}

    Packet(const Packet&) = delete;
    Packet& operator=(const Packet&) = delete;
    Packet(Packet&&) noexcept = default;
    Packet& operator=(Packet&&) noexcept = default;

private:
    std::unique_ptr<int> payload_;
};

int main() {
    Packet first;
    Packet second = first; // Expected compile failure: copy deleted.
    (void)second;
}
