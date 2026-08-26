#include <array>
#include <cstddef>
#include <iostream>
#include <vector>

int main() {
    std::cout << "[raw array]\n";
    int raw[]{10, 20, 30, 40};
    const std::size_t raw_count = sizeof(raw) / sizeof(raw[0]);
    int* pointer = raw; // array-to-pointer conversion
    std::cout << "count=" << raw_count << " sizeof(array)=" << sizeof(raw)
              << " sizeof(element)=" << sizeof(raw[0])
              << " sizeof(pointer)=" << sizeof(pointer) << '\n';
    for (std::size_t i = 0; i < raw_count; ++i) {
        std::cout << "raw[" << i << "]=" << raw[i] << " @ "
                  << static_cast<void*>(raw + i) << '\n';
    }
    int* one_past = pointer + raw_count;
    std::cout << "*(pointer + 2)=" << *(pointer + 2)
              << " distance=" << one_past - pointer
              << " one-past=" << static_cast<void*>(one_past) << " (not dereferenced)\n\n";

    std::cout << "[std::array]\n";
    std::array<int, 4> fixed{10, 20, 30, 40};
    std::cout << "size=" << fixed.size() << " data=" << static_cast<void*>(fixed.data())
              << " fixed[1]=" << fixed[1] << " fixed.at(2)=" << fixed.at(2) << "\n\n";

    std::cout << "[std::vector growth]\n";
    std::vector<int> growing;
    growing.push_back(10);
    const int* old_data = growing.data();
    const std::size_t old_capacity = growing.capacity();
    while (growing.capacity() == old_capacity) {
        growing.push_back(static_cast<int>(growing.size() * 10));
    }
    std::cout << "old data=" << static_cast<const void*>(old_data)
              << " new data=" << static_cast<const void*>(growing.data())
              << " size=" << growing.size() << " capacity=" << growing.capacity() << '\n';
    std::cout << "old pointer invalidated=" << std::boolalpha
              << (old_data != growing.data()) << " (never dereferenced)\n";

    std::cout << "[std::vector reserve]\n";
    std::vector<int> reserved;
    reserved.reserve(8);
    const int* reserved_data = reserved.data();
    std::cout << "after reserve: size=" << reserved.size()
              << " capacity=" << reserved.capacity() << '\n';
    for (int value = 1; value <= 8; ++value) {
        reserved.push_back(value);
    }
    std::cout << "after 8 pushes: size=" << reserved.size()
              << " capacity=" << reserved.capacity()
              << " same data=" << (reserved_data == reserved.data())
              << " at(7)=" << reserved.at(7) << '\n';
    return 0;
}
