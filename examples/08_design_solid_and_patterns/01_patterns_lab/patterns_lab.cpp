#include <iostream>
#include <string>
#include <vector>

enum class PricePolicy { regular, student };

int direct_price(int base, PricePolicy policy) {
    switch (policy) {
    case PricePolicy::regular:
        return base;
    case PricePolicy::student:
        return base * 80 / 100;
    }
    return base;
}

class PricingStrategy {
public:
    virtual ~PricingStrategy() = default;
    virtual int price(int base) const = 0;
    virtual const char* name() const = 0;
};

class RegularPricing final : public PricingStrategy {
public:
    int price(int base) const override { return base; }
    const char* name() const override { return "regular"; }
};

class StudentPricing final : public PricingStrategy {
public:
    int price(int base) const override { return base * 80 / 100; }
    const char* name() const override { return "student"; }
};

class Checkout {
public:
    explicit Checkout(const PricingStrategy& strategy) : strategy_{strategy} {}
    int total(int unit_price, int count) const {
        return strategy_.price(unit_price) * count;
    }

private:
    const PricingStrategy& strategy_; // Non-owning; strategy must outlive Checkout.
};

class Product {
public:
    virtual ~Product() = default;
    virtual const char* format(const std::string& title) const = 0;
};

class TextProduct final : public Product {
public:
    const char* format(const std::string&) const override { return "plain text"; }
};

class HtmlProduct final : public Product {
public:
    const char* format(const std::string&) const override { return "HTML"; }
};

class ReportCreator {
public:
    virtual ~ReportCreator() = default;
    std::string publish(const std::string& title) const {
        const Product& product = create_product();
        return title + " -> " + product.format(title);
    }

protected:
    virtual const Product& create_product() const = 0;
};

class TextReportCreator final : public ReportCreator {
protected:
    const Product& create_product() const override { return product_; }

private:
    TextProduct product_;
};

class HtmlReportCreator final : public ReportCreator {
protected:
    const Product& create_product() const override { return product_; }

private:
    HtmlProduct product_;
};

class Observer {
public:
    virtual ~Observer() = default;
    virtual void on_completed(const std::string& job, int result) = 0;
};

class CompletionPublisher {
public:
    void subscribe(Observer& observer) { observers_.push_back(&observer); }
    void complete(const std::string& job, int result) {
        for (Observer* observer : observers_) {
            observer->on_completed(job, result);
        }
    }

private:
    // Non-owning: registered observers must outlive this publisher's use of them.
    std::vector<Observer*> observers_;
};

class ConsoleObserver final : public Observer {
public:
    void on_completed(const std::string& job, int result) override {
        last_ = job + ":" + std::to_string(result);
        std::cout << "console received " << last_ << '\n';
    }
    const std::string& last() const { return last_; }

private:
    std::string last_;
};

class StatisticsObserver final : public Observer {
public:
    void on_completed(const std::string&, int result) override {
        ++events_;
        total_ += result;
    }
    int events() const { return events_; }
    int total() const { return total_; }

private:
    int events_{};
    int total_{};
};

bool strategy_case() {
    RegularPricing regular;
    StudentPricing student;
    const Checkout regular_checkout{regular};
    const Checkout student_checkout{student};
    const int direct_regular = direct_price(100, PricePolicy::regular) * 2;
    const int direct_student = direct_price(100, PricePolicy::student) * 2;
    const bool pass = direct_regular == regular_checkout.total(100, 2) &&
                      direct_student == student_checkout.total(100, 2) &&
                      regular_checkout.total(100, 2) != student_checkout.total(100, 2);
    std::cout << "strategy baseline regular=" << direct_regular
              << ", refactored " << regular.name() << '=' << regular_checkout.total(100, 2)
              << "\nstrategy baseline student=" << direct_student
              << ", refactored " << student.name() << '=' << student_checkout.total(100, 2)
              << "\nSTRATEGY_CASE=" << (pass ? "PASS" : "FAIL") << '\n';
    return pass;
}

bool factory_case() {
    const TextReportCreator text;
    const HtmlReportCreator html;
    const std::string text_result = text.publish("audit");
    const std::string html_result = html.publish("audit");
    const bool pass = text_result == "audit -> plain text" &&
                      html_result == "audit -> HTML";
    std::cout << "text creator: " << text_result << "\nhtml creator: " << html_result
              << "\nFACTORY_METHOD_CASE=" << (pass ? "PASS" : "FAIL") << '\n';
    return pass;
}

bool observer_case() {
    ConsoleObserver console;
    StatisticsObserver statistics;
    CompletionPublisher publisher;
    publisher.subscribe(console);
    publisher.subscribe(statistics);
    publisher.complete("audit", 7);
    publisher.complete("build", 3);
    const bool pass = console.last() == "build:3" && statistics.events() == 2 &&
                      statistics.total() == 10;
    std::cout << "statistics events=" << statistics.events()
              << ", total=" << statistics.total()
              << "\nOBSERVER_CASE=" << (pass ? "PASS" : "FAIL") << '\n';
    return pass;
}

bool simple_case() {
    const bool verbose = false;
    const char* output = verbose ? "details" : "summary";
    const bool pass = std::string{output} == "summary";
    std::cout << "stable two-way choice: conditional is clearer than extra types\n"
              << "SIMPLE_CASE=" << (pass ? "PASS" : "FAIL") << '\n';
    return pass;
}

int main(int argc, char* argv[]) {
    std::string selected{"all"};
    if (argc == 3 && std::string{argv[1]} == "--case") {
        selected = argv[2];
    } else if (argc != 1) {
        std::cerr << "Usage: patterns_lab [--case strategy|factory|observer|simple]\n";
        return 2;
    }
    bool pass{true};
    if (selected == "all" || selected == "strategy") pass = strategy_case() && pass;
    if (selected == "all" || selected == "factory") pass = factory_case() && pass;
    if (selected == "all" || selected == "observer") pass = observer_case() && pass;
    if (selected == "all" || selected == "simple") pass = simple_case() && pass;
    if (selected != "all" && selected != "strategy" && selected != "factory" &&
        selected != "observer" && selected != "simple") {
        std::cerr << "Unknown case: " << selected << '\n';
        return 2;
    }
    std::cout << "PATTERNS_LAB=" << (pass ? "PASS" : "FAIL") << '\n';
    return pass ? 0 : 1;
}
