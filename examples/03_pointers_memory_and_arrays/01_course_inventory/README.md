# course_inventory

Read-only C++17 utility that reports lecture bytes/lines and regular files in each examples directory.

```sh
cmake -S . -B build
cmake --build build
./build/course_inventory --root ../../..
./build/course_inventory --root ../../.. --block 3
ctest --test-dir build --output-on-failure
```

Exit code 0 means the selected structural pairs exist; 1 means a pair is incomplete; 2 means the command line or course root is invalid. The program never writes to the course tree.
