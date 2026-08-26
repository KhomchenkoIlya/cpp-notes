# generic_course_audit

This read-only utility inspects the real 15 course blocks. One function
template, `run_audit`, works with both `CompactReporter` and `DetailedReporter`;
the reporters have no common base class. Their concrete types are known during
compilation, so runtime policy replacement is neither promised nor paid for.

CTest also verifies a bad root and compiles a reporter that lacks the required
operations. The actual diagnostic is saved in `build/negative/bad_reporter.log`.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/generic_course_audit --root ../../..
ctest --test-dir build --output-on-failure
```

Success means both reporters process 15 blocks, agree on 9 completed blocks,
print `GENERIC_COURSE_AUDIT=PASS`, and all three tests pass.
