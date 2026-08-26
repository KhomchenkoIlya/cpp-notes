# function_contract_lab

```sh
cmake -S . -B build
cmake --build build
./build/function_contract_lab
ctest --test-dir build --output-on-failure
```

The executable checks pass-by-value, mutable reference, const reference, return value versus output reference, local lifetime, overloads, and a function pointer. CTest also compiles three deliberately invalid sources and passes only when compilation fails.

The unsafe case is isolated and opt-in:

```sh
cmake -S . -B build-sanitize -DENABLE_SANITIZER_CASE=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build-sanitize
./build-sanitize/dangling_return
```

PASS means the safe program prints `FUNCTION_CONTRACT_LAB=PASS`, all four CTest cases pass, and the unsafe executable exits nonzero with a real ASan/UBSan diagnostic. A sanitizer run observes this build and path; it is not a proof that other code has no UB.
