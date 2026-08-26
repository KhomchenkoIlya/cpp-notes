# course_inventory_classes

This is the class-based continuation of block 04's read-only inventory. `CourseBlock` keeps a block's identity, derived paths, and measured state consistent. Argument parsing and report formatting remain free functions.

```sh
cmake -S . -B build
cmake --build build
./build/course_inventory_classes --root ../../.. --block 5
ctest --test-dir build --output-on-failure
```

The command line, table columns, totals, and exit-code classes match `course_inventory_refactored`. Generated files stay under `build*/`.
