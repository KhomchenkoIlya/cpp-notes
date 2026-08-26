# inventory_snapshot

This experiment develops block 05's class-based inventory without modifying
that block. `CourseBlock` establishes its paths and measured state in a public
constructor. It contains only values and standard-library resource managers,
so compiler-generated copy operations implement the intended value semantics.

`MutableBlockSnapshot` is deliberately separate: changing an analysis snapshot
does not weaken the read-only domain model with an artificial setter.

```sh
cmake -S . -B build
cmake --build build
./build/inventory_snapshot --root ../../..
ctest --test-dir build --output-on-failure
```

PASS requires copy construction and copy assignment of course data plus proof
that changing one snapshot's `std::string` and `std::vector` leaves the original
unchanged.
