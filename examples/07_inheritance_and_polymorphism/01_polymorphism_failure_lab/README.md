# polymorphism_failure_lab

Safe build and all compile-time checks:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/polymorphism_failure_lab
ctest --test-dir build --output-on-failure
```

The normal program demonstrates non-virtual versus virtual dispatch, slicing
versus a reference, a concrete implementation of an abstract interface,
protected access, and virtual destructor order. Three CTest cases require the
compiler to reject a bad `override`, abstract-class instantiation, and direct
access to private base state.

The unsafe deletion experiment is opt-in and is never part of the ordinary
build:

```sh
cmake -S . -B build-sanitize -DENABLE_UNSAFE_CASE=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build-sanitize
ctest --test-dir build-sanitize --output-on-failure
```

Its test passes only when the process fails and AddressSanitizer emits a
relevant diagnostic. Sanitizer behavior is toolchain-specific; the C++ rule
does not depend on whether a particular run diagnoses the undefined behavior.
