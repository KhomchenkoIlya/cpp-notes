# inheritance_vs_composition_project_lab

Build and run from this directory:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/project_lab --root ../../..
ctest --test-dir build --output-on-failure
```

Both designs audit the real block 07 and must print the same three results.
The inheritance design treats each independently extensible check as a
`BlockCheck` and invokes it through non-owning base pointers whose automatic
objects remain alive. The composition design makes `CourseAudit` contain two
concrete checking collaborators. It has less hierarchy boilerplate for this
fixed local audit, while the polymorphic design provides a uniform extension
point when checks truly need to vary independently at runtime.

`PROJECT_DESIGN_LAB=PASS`, equivalent results, and exit code 0 are required.
