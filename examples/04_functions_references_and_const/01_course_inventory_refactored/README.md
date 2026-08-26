# course_inventory_refactored

This is the same read-only task and report format as block 03's `course_inventory`, decomposed into functions. It never writes to the course tree.

```sh
cmake -S . -B build
cmake --build build
./build/course_inventory_refactored --root ../../..
./build/course_inventory_refactored --root ../../.. --block 3
ctest --test-dir build --output-on-failure
```

For equivalence, build the old utility and compare normalized output (the two data addresses vary between processes):

```sh
sed -E 's/@ 0x[0-9a-f]+/@ ADDRESS/' old.out > old.normalized
sed -E 's/@ 0x[0-9a-f]+/@ ADDRESS/' new.out > new.normalized
diff -u old.normalized new.normalized
```

PASS means both tests return 0 and normalized output for the same arguments is identical to the block-03 utility.
