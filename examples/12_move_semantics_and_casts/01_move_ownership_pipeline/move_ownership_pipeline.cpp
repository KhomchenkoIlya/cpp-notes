#include <iostream>
#include <memory>
#include <string>
#include <utility>

namespace {

struct Trace {
    static int constructed;
    static int moved_constructed;
    static int moved_assigned;
    static int destroyed;

    static void reset() {
        constructed = 0;
        moved_constructed = 0;
        moved_assigned = 0;
        destroyed = 0;
    }
};

int Trace::constructed = 0;
int Trace::moved_constructed = 0;
int Trace::moved_assigned = 0;
int Trace::destroyed = 0;

class Packet {
public:
    explicit Packet(std::string text)
        : payload_{std::make_unique<std::string>(std::move(text))} {
        ++Trace::constructed;
    }

    ~Packet() {
        ++Trace::destroyed;
    }

    Packet(const Packet&) = delete;
    Packet& operator=(const Packet&) = delete;

    Packet(Packet&& other) noexcept
        : payload_{std::move(other.payload_)} {
        ++Trace::moved_constructed;
    }

    Packet& operator=(Packet&& other) noexcept {
        if (this != &other) {
            payload_ = std::move(other.payload_);
        }
        ++Trace::moved_assigned;
        return *this;
    }

    bool empty() const noexcept {
        return payload_ == nullptr;
    }

    const std::string& text() const {
        return *payload_;
    }

private:
    std::unique_ptr<std::string> payload_;
};

std::string category(Packet&) {
    return "lvalue";
}

std::string category(const Packet&) {
    return "const-lvalue";
}

std::string category(Packet&&) {
    return "rvalue";
}

template <typename T>
std::string relay_category(T&& packet) {
    return category(std::forward<T>(packet));
}

Packet make_packet() {
    return Packet{"from-factory"};
}

std::string consume(Packet packet) {
    return packet.empty() ? "unexpected-empty" : "consumed:" + packet.text();
}

bool move_construction_case() {
    Trace::reset();

    bool pass = false;
    {
        Packet source{"alpha"};
        Packet destination{std::move(source)};

        pass =
            source.empty() &&
            !destination.empty() &&
            destination.text() == "alpha" &&
            Trace::constructed == 1 &&
            Trace::moved_constructed == 1 &&
            Trace::moved_assigned == 0;
    }

    pass = pass && Trace::destroyed == 2;

    std::cout
        << "moves_ctor=" << Trace::moved_constructed << '\n'
        << "destroyed=" << Trace::destroyed << '\n'
        << "MOVE_CONSTRUCTION=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool move_assignment_case() {
    Trace::reset();

    bool pass = false;
    {
        Packet destination{"old"};
        Packet source{"new"};

        destination = std::move(source);

        pass =
            source.empty() &&
            !destination.empty() &&
            destination.text() == "new" &&
            Trace::constructed == 2 &&
            Trace::moved_assigned == 1;
    }

    pass = pass && Trace::destroyed == 2;

    std::cout
        << "moves_assign=" << Trace::moved_assigned << '\n'
        << "MOVE_ASSIGNMENT=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool value_category_case() {
    Packet packet{"named"};
    const Packet const_packet{"const"};

    Packet&& rvalue_reference = std::move(packet);

    const bool pass =
        category(packet) == "lvalue" &&
        category(const_packet) == "const-lvalue" &&
        category(Packet{"temporary"}) == "rvalue" &&
        category(std::move(packet)) == "rvalue" &&
        category(rvalue_reference) == "lvalue";

    std::cout
        << "named=" << category(packet) << '\n'
        << "const_named=" << category(const_packet) << '\n'
        << "temporary=" << category(Packet{"temporary-2"}) << '\n'
        << "named_rvalue_ref=" << category(rvalue_reference) << '\n'
        << "VALUE_CATEGORIES=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool forwarding_case() {
    Packet packet{"forward-me"};

    const bool pass =
        relay_category(packet) == "lvalue" &&
        relay_category(Packet{"temporary"}) == "rvalue" &&
        relay_category(std::move(packet)) == "rvalue";

    std::cout
        << "relay_lvalue=" << relay_category(packet) << '\n'
        << "relay_rvalue=" << relay_category(Packet{"temporary-2"}) << '\n'
        << "FORWARDING=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool elision_case() {
    Trace::reset();

    bool pass = false;
    {
        Packet packet = make_packet();

        pass =
            !packet.empty() &&
            packet.text() == "from-factory" &&
            Trace::constructed == 1 &&
            Trace::moved_constructed == 0 &&
            Trace::moved_assigned == 0;
    }

    pass = pass && Trace::destroyed == 1;

    std::cout
        << "constructed=" << Trace::constructed << '\n'
        << "moves_ctor=" << Trace::moved_constructed << '\n'
        << "COPY_ELISION_CXX17=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool ownership_pipeline_case() {
    Trace::reset();

    Packet source{"pipeline"};
    const std::string result = consume(std::move(source));

    const bool pass =
        source.empty() &&
        result == "consumed:pipeline" &&
        Trace::moved_constructed == 1;

    std::cout
        << "result=" << result << '\n'
        << "source_empty=" << (source.empty() ? "yes" : "no") << '\n'
        << "OWNERSHIP_PIPELINE=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

} // namespace

int main(int argc, char* argv[]) {
    std::string selected{"all"};

    if (argc == 3 && std::string{argv[1]} == "--case") {
        selected = argv[2];
    } else if (argc != 1) {
        std::cerr
            << "Usage: move_ownership_pipeline "
            << "[--case ctor|assign|category|forward|elision|pipeline]\n";
        return 2;
    }

    bool pass = true;

    if (selected == "all" || selected == "ctor") {
        pass = move_construction_case() && pass;
    }
    if (selected == "all" || selected == "assign") {
        pass = move_assignment_case() && pass;
    }
    if (selected == "all" || selected == "category") {
        pass = value_category_case() && pass;
    }
    if (selected == "all" || selected == "forward") {
        pass = forwarding_case() && pass;
    }
    if (selected == "all" || selected == "elision") {
        pass = elision_case() && pass;
    }
    if (selected == "all" || selected == "pipeline") {
        pass = ownership_pipeline_case() && pass;
    }

    if (selected != "all" &&
        selected != "ctor" &&
        selected != "assign" &&
        selected != "category" &&
        selected != "forward" &&
        selected != "elision" &&
        selected != "pipeline") {
        std::cerr << "Unknown case: " << selected << '\n';
        return 2;
    }

    std::cout
        << "MOVE_OWNERSHIP_PIPELINE="
        << (pass ? "PASS" : "FAIL") << '\n';

    return pass ? 0 : 1;
}
