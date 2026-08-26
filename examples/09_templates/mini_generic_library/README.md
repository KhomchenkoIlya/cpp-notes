# mini_generic_library

The header `include/mini_generic.hpp` contains a small reusable C++17 library:
`maximum`, `count_matching`, `FixedBuffer<T, N>`, and one deliberate full class
template specialization. The executable verifies fundamental, library, and
user-defined types. CTest also compiles `negative/no_less.cpp` and passes only
when the compiler rejects the missing `operator<`; its real diagnostic is saved
under `build/negative/no_less.log`.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/mini_generic_library
ctest --test-dir build --output-on-failure
```

Success prints `MINI_GENERIC_LIBRARY=PASS` and all tests pass.
