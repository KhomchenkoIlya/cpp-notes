# encapsulation_break_lab

The positive program contrasts corruptible public data with controlled private state, calls read-only methods through `const CourseProgress&`, demonstrates two independent instances, and keeps a formatting operation free.

```sh
cmake -S . -B build
cmake --build build
./build/encapsulation_break_lab
ctest --test-dir build --output-on-failure
```

CTest passes only if the positive executable succeeds and the compiler rejects both negative files. Actual diagnostics are saved as `build/private_access.log` and `build/const_mutation.log`.
