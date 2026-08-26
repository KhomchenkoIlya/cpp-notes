class BlockCheck {
public:
    virtual ~BlockCheck() = default;
    virtual bool passes() const = 0;
};

int main() {
    BlockCheck check; // An abstract class cannot be instantiated.
    return check.passes() ? 0 : 1;
}
