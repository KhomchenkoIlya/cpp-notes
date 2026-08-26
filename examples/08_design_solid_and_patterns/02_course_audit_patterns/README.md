# course_audit_patterns

This read-only utility audits all 15 lecture/example pairs. The audit engine
depends on an `AuditStrategy`; event reporting depends on `AuditObserver`.
The built-in `lecture-only` policy is an extension check: it plugs into the
same engine without changing its workflow.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/course_audit_patterns --root ../../.. --policy structural
./build/course_audit_patterns --root ../../.. --policy strict
ctest --test-dir build --output-on-failure
```

The utility never writes under the course root. Success prints
`COURSE_AUDIT_PATTERNS=PASS` and returns 0.
