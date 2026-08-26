# array_vs_vector_lab

Safe run:

```sh
cmake -S . -B build
cmake --build build
./build/array_vs_vector_lab
ctest --test-dir build --output-on-failure
```

Isolated intentional UB cases (GCC/Clang with ASan+UBSan):

```sh
cmake -S . -B build-sanitize -DENABLE_SANITIZER_CASES=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build-sanitize
./build-sanitize/raw_oob
./build-sanitize/dangling
./build-sanitize/vector_invalidated
```

Each last command is expected to exit nonzero and report respectively a stack-buffer-overflow, stack-use-after-scope, and heap-use-after-free. The safe program never dereferences an invalidated pointer.
