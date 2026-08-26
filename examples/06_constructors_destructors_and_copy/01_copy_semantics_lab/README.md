# copy_semantics_lab

The normal target checks plain class and `std::vector` value copies, a nested
construction/destruction trace, a Rule-of-Three deep copy (including
self-assignment), and a Rule-of-Zero equivalent. CTest also compiles a small
`-Wreorder` case and requires a real compiler warning.

```sh
cmake -S . -B build
cmake --build build
./build/copy_semantics_lab
ctest --test-dir build --output-on-failure
```

The intentionally broken owning-pointer example is opt-in and requires
GCC/Clang sanitizers:

```sh
cmake -S . -B build-sanitize -DENABLE_UNSAFE_CASE=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build-sanitize
./build-sanitize/bad_shallow_copy
```

The last command is expected to fail with an AddressSanitizer double-free
diagnostic. The normal executable must remain successful.
