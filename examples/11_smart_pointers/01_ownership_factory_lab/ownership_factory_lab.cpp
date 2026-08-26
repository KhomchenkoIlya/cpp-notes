#include <iostream>
#include <memory>
#include <string>
#include <utility>

namespace {

int destroyed_products = 0;
int destroyed_jobs = 0;

class Product {
public:
    virtual ~Product() {
        ++destroyed_products;
    }

    virtual std::string format(const std::string& title) const = 0;
    virtual const char* kind() const = 0;
};

class TextProduct final : public Product {
public:
    std::string format(const std::string& title) const override {
        return title + " -> plain text";
    }

    const char* kind() const override {
        return "text";
    }
};

class HtmlProduct final : public Product {
public:
    std::string format(const std::string& title) const override {
        return title + " -> HTML";
    }

    const char* kind() const override {
        return "html";
    }
};

class ReportCreator {
public:
    virtual ~ReportCreator() = default;

    std::string publish(const std::string& title) const {
        std::unique_ptr<Product> product = make_product();
        return product->format(title);
    }

    std::unique_ptr<Product> create_owned() const {
        return make_product();
    }

protected:
    virtual std::unique_ptr<Product> make_product() const = 0;
};

class TextReportCreator final : public ReportCreator {
protected:
    std::unique_ptr<Product> make_product() const override {
        return std::make_unique<TextProduct>();
    }
};

class HtmlReportCreator final : public ReportCreator {
protected:
    std::unique_ptr<Product> make_product() const override {
        return std::make_unique<HtmlProduct>();
    }
};

std::string inspect_borrow(const Product& product) {
    return std::string{"borrow:"} + product.kind();
}

std::string inspect_nullable(const Product* product) {
    return product == nullptr ? "borrow:null" : std::string{"borrow:"} + product->kind();
}

class Job {
public:
    explicit Job(std::string name) : name_{std::move(name)} {}

    ~Job() {
        ++destroyed_jobs;
        std::cout << "destroy job " << name_ << '\n';
    }

    const std::string& name() const {
        return name_;
    }

private:
    std::string name_;
};

std::string consume(std::unique_ptr<Job> job) {
    return "consumed:" + job->name();
}

bool factory_case() {
    destroyed_products = 0;

    const TextReportCreator text;
    const HtmlReportCreator html;

    const std::string text_result = text.publish("audit");
    const std::string html_result = html.publish("audit");

    const bool pass =
        text_result == "audit -> plain text" &&
        html_result == "audit -> HTML" &&
        destroyed_products == 2;

    std::cout
        << "text=" << text_result << '\n'
        << "html=" << html_result << '\n'
        << "destroyed_products=" << destroyed_products << '\n'
        << "FACTORY_OWNERSHIP=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool transfer_case() {
    destroyed_jobs = 0;
    auto owner = std::make_unique<Job>("block-11");
    const Job* borrowed_before = owner.get();

    const bool before =
        owner != nullptr &&
        borrowed_before != nullptr &&
        borrowed_before->name() == "block-11";

    const std::string result = consume(std::move(owner));

    const bool pass =
        before &&
        result == "consumed:block-11" &&
        owner == nullptr &&
        destroyed_jobs == 1;

    std::cout
        << "result=" << result << '\n'
        << "old_owner_empty=" << (owner == nullptr ? "yes" : "no") << '\n'
        << "destroyed_jobs=" << destroyed_jobs << '\n'
        << "TRANSFER_OWNERSHIP=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool borrowing_case() {
    const TextReportCreator creator;
    std::unique_ptr<Product> owner = creator.create_owned();

    const Product* raw_view = owner.get();
    const std::string by_reference = inspect_borrow(*owner);
    const std::string by_pointer = inspect_nullable(raw_view);
    const std::string null_case = inspect_nullable(nullptr);

    const bool pass =
        owner != nullptr &&
        by_reference == "borrow:text" &&
        by_pointer == "borrow:text" &&
        null_case == "borrow:null";

    std::cout
        << "reference=" << by_reference << '\n'
        << "pointer=" << by_pointer << '\n'
        << "null=" << null_case << '\n'
        << "BORROWING_API=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool reset_case() {
    destroyed_products = 0;
    const HtmlReportCreator creator;
    std::unique_ptr<Product> owner = creator.create_owned();

    const bool alive_before = owner != nullptr && destroyed_products == 0;
    owner.reset();
    const bool pass =
        alive_before &&
        owner == nullptr &&
        destroyed_products == 1;

    std::cout
        << "owner_empty=" << (owner == nullptr ? "yes" : "no") << '\n'
        << "destroyed_products=" << destroyed_products << '\n'
        << "RESET_CASE=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

} // namespace

int main(int argc, char* argv[]) {
    std::string selected{"all"};

    if (argc == 3 && std::string{argv[1]} == "--case") {
        selected = argv[2];
    } else if (argc != 1) {
        std::cerr
            << "Usage: ownership_factory_lab "
            << "[--case factory|transfer|borrowing|reset]\n";
        return 2;
    }

    bool pass = true;

    if (selected == "all" || selected == "factory") {
        pass = factory_case() && pass;
    }
    if (selected == "all" || selected == "transfer") {
        pass = transfer_case() && pass;
    }
    if (selected == "all" || selected == "borrowing") {
        pass = borrowing_case() && pass;
    }
    if (selected == "all" || selected == "reset") {
        pass = reset_case() && pass;
    }

    if (selected != "all" &&
        selected != "factory" &&
        selected != "transfer" &&
        selected != "borrowing" &&
        selected != "reset") {
        std::cerr << "Unknown case: " << selected << '\n';
        return 2;
    }

    std::cout << "OWNERSHIP_FACTORY_LAB=" << (pass ? "PASS" : "FAIL") << '\n';
    return pass ? 0 : 1;
}
