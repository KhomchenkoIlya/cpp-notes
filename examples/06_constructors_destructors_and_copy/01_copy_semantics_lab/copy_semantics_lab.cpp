#include <iostream>
#include <string>
#include <vector>

class LessonCard {
public:
    LessonCard(int number, const std::string& title)
        : number_{number}, title_{title} {}

    int number() const { return number_; }
    const std::string& title() const { return title_; }
    void rename(const std::string& title) { title_ = title; }

private:
    int number_;
    std::string title_;
};

class Trace {
public:
    explicit Trace(const std::string& name) : name_{name} {
        std::cout << "construct " << name_ << '\n';
    }
    Trace(const Trace& other) : name_{other.name_ + " copy"} {
        std::cout << "copy-construct " << name_ << " from " << other.name_ << '\n';
    }
    Trace& operator=(const Trace& other) {
        std::cout << "copy-assign " << name_ << " from " << other.name_ << '\n';
        name_ = other.name_ + " assigned";
        return *this;
    }
    ~Trace() { std::cout << "destroy " << name_ << '\n'; }

private:
    std::string name_;
};

class DeepBuffer {
public:
    explicit DeepBuffer(std::size_t size)
        : size_{size}, data_{new int[size]{}} {}

    DeepBuffer(const DeepBuffer& other)
        : size_{other.size_}, data_{new int[other.size_]} {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    DeepBuffer& operator=(const DeepBuffer& other) {
        if (this == &other) {
            return *this;
        }
        int* replacement = new int[other.size_];
        for (std::size_t i = 0; i < other.size_; ++i) {
            replacement[i] = other.data_[i];
        }
        delete[] data_;
        data_ = replacement;
        size_ = other.size_;
        return *this;
    }

    ~DeepBuffer() { delete[] data_; }

    int& at(std::size_t index) { return data_[index]; }
    int at(std::size_t index) const { return data_[index]; }

private:
    std::size_t size_;
    int* data_; // owning pointer, isolated teaching example
};

class ZeroBuffer {
public:
    ZeroBuffer(const std::string& label, const std::vector<int>& values)
        : label_{label}, values_{values} {}

    const std::string& label() const { return label_; }
    std::vector<int>& values() { return values_; }
    const std::vector<int>& values() const { return values_; }

private:
    std::string label_;
    std::vector<int> values_;
};

int main() {
    LessonCard original{6, "constructors"};
    LessonCard constructed = original;
    constructed.rename("independent copy");
    LessonCard assigned{0, "placeholder"};
    assigned = original;
    assigned.rename("assigned copy");
    const bool plain_copy = original.title() == "constructors" &&
                            constructed.title() == "independent copy" &&
                            assigned.title() == "assigned copy";

    std::vector<std::string> topics{"construction", "destruction"};
    std::vector<std::string> topics_copy = topics;
    topics_copy[0] = "copy construction";
    topics_copy.push_back("Rule of Zero");
    const bool vector_copy = topics.size() == 2 && topics[0] == "construction" &&
                             topics_copy.size() == 3;

    std::cout << "-- trace begin --\n";
    {
        Trace first{"first"};
        Trace second = first;
        Trace third{"third"};
        third = first;
        std::cout << "-- nested scope end --\n";
    }
    std::cout << "-- trace finished --\n";

    DeepBuffer deep_original{3};
    deep_original.at(0) = 10;
    DeepBuffer deep_copy = deep_original;
    deep_copy.at(0) = 99;
    DeepBuffer deep_assigned{1};
    deep_assigned = deep_original;
    deep_assigned = deep_assigned;
    const bool deep_copy_ok = deep_original.at(0) == 10 && deep_copy.at(0) == 99 &&
                              deep_assigned.at(0) == 10;

    ZeroBuffer zero_original{"topics", {1, 2, 3}};
    ZeroBuffer zero_copy = zero_original;
    zero_copy.values()[0] = 42;
    ZeroBuffer zero_assigned{"empty", {}};
    zero_assigned = zero_original;
    const bool zero_copy_ok = zero_original.values()[0] == 1 &&
                              zero_copy.values()[0] == 42 &&
                              zero_assigned.values()[0] == 1;

    std::cout << std::boolalpha
              << "plain value copy independent: " << plain_copy << '\n'
              << "vector copy independent: " << vector_copy << '\n'
              << "deep copy independent: " << deep_copy_ok << '\n'
              << "Rule-of-Zero copy independent: " << zero_copy_ok << '\n';
    const bool pass = plain_copy && vector_copy && deep_copy_ok && zero_copy_ok;
    std::cout << "COPY_SEMANTICS_LAB=" << (pass ? "PASS" : "FAIL") << '\n';
    return pass ? 0 : 1;
}
