#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

struct Event {
    std::string name;
    int value{};
};

using Callback = std::function<void(const Event&)>;

std::vector<std::string>* free_function_log = nullptr;

void free_logger(const Event& event) {
    if (free_function_log != nullptr) {
        free_function_log->push_back("free:" + event.name);
    }
}

struct ScaleRecorder {
    int factor{};
    std::vector<int>* output{};

    void operator()(const Event& event) const {
        output->push_back(event.value * factor);
    }
};

class Registry {
public:
    void subscribe(Callback callback) {
        callbacks_.push_back(std::move(callback));
    }

    void publish(const Event& event) {
        for (auto& callback : callbacks_) {
            callback(event);
        }
    }

    std::size_t size() const noexcept {
        return callbacks_.size();
    }

private:
    std::vector<Callback> callbacks_;
};

class ObserverState {
public:
    explicit ObserverState(std::string prefix)
        : prefix_{std::move(prefix)} {}

    std::string format(const Event& event) const {
        return prefix_ + ":" + event.name;
    }

private:
    std::string prefix_;
};

bool heterogeneous_registry_case() {
    Registry registry;

    std::vector<std::string> text_log;
    std::vector<int> scaled;

    free_function_log = &text_log;
    registry.subscribe(free_logger);

    registry.subscribe(ScaleRecorder{3, &scaled});

    const std::string prefix{"lambda"};
    registry.subscribe(
        [prefix, &text_log](const Event& event) {
            text_log.push_back(prefix + ":" + event.name);
        }
    );

    registry.publish(Event{"build", 4});

    free_function_log = nullptr;

    const bool pass =
        registry.size() == 3 &&
        text_log.size() == 2 &&
        text_log[0] == "free:build" &&
        text_log[1] == "lambda:build" &&
        scaled.size() == 1 &&
        scaled[0] == 12;

    std::cout
        << "callbacks=" << registry.size() << '\n'
        << "text_entries=" << text_log.size() << '\n'
        << "scaled=" << (scaled.empty() ? -1 : scaled[0]) << '\n'
        << "HETEROGENEOUS_REGISTRY="
        << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool mutable_capture_case() {
    Registry registry;
    std::vector<int> calls;

    registry.subscribe(
        [count = 0, &calls](const Event&) mutable {
            ++count;
            calls.push_back(count);
        }
    );

    registry.publish(Event{"one", 1});
    registry.publish(Event{"two", 2});
    registry.publish(Event{"three", 3});

    const bool pass =
        calls == std::vector<int>({1, 2, 3});

    std::cout
        << "calls="
        << calls[0] << "," << calls[1] << "," << calls[2] << '\n'
        << "MUTABLE_CAPTURE="
        << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool weak_capture_case() {
    Registry registry;
    std::vector<std::string> output;

    auto owner = std::make_shared<ObserverState>("alive");
    std::weak_ptr<ObserverState> observer = owner;

    registry.subscribe(
        [observer, &output](const Event& event) {
            if (const auto locked = observer.lock()) {
                output.push_back(locked->format(event));
            } else {
                output.push_back("expired:" + event.name);
            }
        }
    );

    registry.publish(Event{"first", 1});
    owner.reset();
    registry.publish(Event{"second", 2});

    const bool pass =
        output.size() == 2 &&
        output[0] == "alive:first" &&
        output[1] == "expired:second" &&
        observer.expired();

    std::cout
        << "first=" << output[0] << '\n'
        << "second=" << output[1] << '\n'
        << "WEAK_CAPTURE="
        << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool empty_function_case() {
    Callback callback;

    const bool initially_empty = !callback;
    bool threw = false;

    try {
        callback(Event{"unused", 0});
    } catch (const std::bad_function_call&) {
        threw = true;
    }

    callback = [](const Event&) {};
    const bool filled = static_cast<bool>(callback);

    const bool pass =
        initially_empty &&
        threw &&
        filled;

    std::cout
        << "initially_empty=" << (initially_empty ? "yes" : "no") << '\n'
        << "bad_function_call=" << (threw ? "yes" : "no") << '\n'
        << "filled=" << (filled ? "yes" : "no") << '\n'
        << "EMPTY_FUNCTION="
        << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

template <typename Callable>
int immediate_call(Callable callable) {
    return callable(7);
}

bool template_callable_case() {
    const int factor = 5;

    const int result = immediate_call(
        [factor](int value) {
            return value * factor;
        }
    );

    const bool pass = result == 35;

    std::cout
        << "result=" << result << '\n'
        << "TEMPLATE_CALLABLE="
        << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

} // namespace

int main(int argc, char* argv[]) {
    std::string selected{"all"};

    if (argc == 3 && std::string{argv[1]} == "--case") {
        selected = argv[2];
    } else if (argc != 1) {
        std::cerr
            << "Usage: callback_registry_lab "
            << "[--case heterogeneous|mutable|weak|empty|template]\n";
        return 2;
    }

    bool pass = true;

    if (selected == "all" || selected == "heterogeneous") {
        pass = heterogeneous_registry_case() && pass;
    }
    if (selected == "all" || selected == "mutable") {
        pass = mutable_capture_case() && pass;
    }
    if (selected == "all" || selected == "weak") {
        pass = weak_capture_case() && pass;
    }
    if (selected == "all" || selected == "empty") {
        pass = empty_function_case() && pass;
    }
    if (selected == "all" || selected == "template") {
        pass = template_callable_case() && pass;
    }

    if (selected != "all" &&
        selected != "heterogeneous" &&
        selected != "mutable" &&
        selected != "weak" &&
        selected != "empty" &&
        selected != "template") {
        std::cerr << "Unknown case: " << selected << '\n';
        return 2;
    }

    std::cout
        << "CALLBACK_REGISTRY_LAB="
        << (pass ? "PASS" : "FAIL") << '\n';

    return pass ? 0 : 1;
}
