#include <algorithm>
#include <cstdint>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

bool reserve_case() {
    std::vector<int> values;
    values.reserve(8);

    const auto initial_capacity = values.capacity();

    values.push_back(1);
    int* first_address = values.data();

    values.push_back(2);
    values.push_back(3);
    values.push_back(4);

    const bool pass =
        initial_capacity >= 8 &&
        values.capacity() == initial_capacity &&
        values.data() == first_address &&
        values.size() == 4;

    std::cout
        << "capacity=" << values.capacity() << '\n'
        << "same_data_within_capacity="
        << (values.data() == first_address ? "yes" : "no") << '\n'
        << "RESERVE=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool reallocation_case() {
    std::vector<int> values;
    values.reserve(1);
    values.push_back(10);

    const auto old_capacity = values.capacity();
    int* old_pointer = values.data();
    const std::uintptr_t old_address =
        reinterpret_cast<std::uintptr_t>(old_pointer);

    while (values.capacity() == old_capacity) {
        values.push_back(static_cast<int>(values.size()) + 10);
    }

    const std::uintptr_t new_address =
        reinterpret_cast<std::uintptr_t>(values.data());

    // old_pointer is deliberately never dereferenced after reallocation.
    const bool pass =
        values.capacity() > old_capacity &&
        new_address != old_address &&
        values.front() == 10;

    std::cout
        << "old_capacity=" << old_capacity << '\n'
        << "new_capacity=" << values.capacity() << '\n'
        << "address_changed="
        << (new_address != old_address ? "yes" : "no") << '\n'
        << "REALLOCATION=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool erase_remove_case() {
    std::vector<int> values{1, -1, 2, -2, 3, -3, 4};

    const auto new_end = std::remove_if(
        values.begin(),
        values.end(),
        [](int value) {
            return value < 0;
        }
    );

    const auto logical_size =
        static_cast<std::size_t>(std::distance(values.begin(), new_end));

    values.erase(new_end, values.end());

    const bool pass =
        logical_size == 4 &&
        values == std::vector<int>({1, 2, 3, 4});

    std::cout
        << "logical_size=" << logical_size << '\n'
        << "physical_size=" << values.size() << '\n'
        << "ERASE_REMOVE=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool erase_return_case() {
    std::vector<std::string> values{"a", "b", "c", "d"};

    auto it = values.begin() + 1;
    auto next = values.erase(it);

    const bool pass =
        values == std::vector<std::string>({"a", "c", "d"}) &&
        next != values.end() &&
        *next == "c";

    std::cout
        << "next=" << (next == values.end() ? "end" : *next) << '\n'
        << "ERASE_RETURN=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool list_stability_case() {
    std::list<int> values{1, 3};

    auto first = values.begin();
    int* first_address = &*first;

    auto position = values.begin();
    ++position;
    values.insert(position, 2);

    const bool pass =
        &*first == first_address &&
        *first == 1 &&
        values.size() == 3;

    std::cout
        << "first_stable="
        << (&*first == first_address ? "yes" : "no") << '\n'
        << "LIST_STABILITY=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool unordered_rehash_case() {
    std::unordered_map<int, std::string> values;
    values.reserve(2);

    values.emplace(1, "one");
    values.emplace(2, "two");

    const auto old_bucket_count = values.bucket_count();

    // We save a pointer/reference to an element: rehash invalidates iterators,
    // but references/pointers to elements remain valid for unordered containers.
    const std::string* value_address = &values.at(1);

    values.rehash(old_bucket_count * 4 + 1);

    const bool pass =
        values.bucket_count() > old_bucket_count &&
        value_address == &values.at(1) &&
        *value_address == "one";

    std::cout
        << "old_buckets=" << old_bucket_count << '\n'
        << "new_buckets=" << values.bucket_count() << '\n'
        << "element_reference_stable="
        << (value_address == &values.at(1) ? "yes" : "no") << '\n'
        << "UNORDERED_REHASH=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

} // namespace

int main(int argc, char* argv[]) {
    std::string selected{"all"};

    if (argc == 3 && std::string{argv[1]} == "--case") {
        selected = argv[2];
    } else if (argc != 1) {
        std::cerr
            << "Usage: invalidation_and_erase_lab "
            << "[--case reserve|reallocation|erase_remove|erase_return|list|rehash]\n";
        return 2;
    }

    bool pass = true;

    if (selected == "all" || selected == "reserve") {
        pass = reserve_case() && pass;
    }
    if (selected == "all" || selected == "reallocation") {
        pass = reallocation_case() && pass;
    }
    if (selected == "all" || selected == "erase_remove") {
        pass = erase_remove_case() && pass;
    }
    if (selected == "all" || selected == "erase_return") {
        pass = erase_return_case() && pass;
    }
    if (selected == "all" || selected == "list") {
        pass = list_stability_case() && pass;
    }
    if (selected == "all" || selected == "rehash") {
        pass = unordered_rehash_case() && pass;
    }

    if (selected != "all" &&
        selected != "reserve" &&
        selected != "reallocation" &&
        selected != "erase_remove" &&
        selected != "erase_return" &&
        selected != "list" &&
        selected != "rehash") {
        std::cerr << "Unknown case: " << selected << '\n';
        return 2;
    }

    std::cout
        << "INVALIDATION_AND_ERASE_LAB="
        << (pass ? "PASS" : "FAIL") << '\n';

    return pass ? 0 : 1;
}
