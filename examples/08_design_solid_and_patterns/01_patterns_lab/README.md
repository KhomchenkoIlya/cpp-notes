# patterns_lab

The executable compares a direct strategy `switch` with runtime Strategy,
then exercises canonical Factory Method and a non-owning Observer registry.
The `simple` case records why a two-branch stable choice needs no class graph.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/patterns_lab
ctest --test-dir build --output-on-failure
```

`PATTERNS_LAB=PASS` and exit code 0 are the overall success criteria.
