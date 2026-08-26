struct Config {
    int retries{};
    bool verbose{};
};

int main() {
    Config config{
        .verbose = true,
        .retries = 3
    };
    // Expected compile failure: C++20 designators follow declaration order.
    return config.retries;
}
