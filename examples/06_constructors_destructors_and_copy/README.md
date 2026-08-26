# Block 06 practice

Exactly two experiments support the lecture:

1. `01_copy_semantics_lab` — value copying, vector independence, lifetime tracing,
   an isolated shallow-copy failure, a Rule-of-Three repair, and Rule of Zero.
2. `02_inventory_snapshot` — independent snapshots of the course inventory built
   from `CourseBlock` values whose standard-library members provide correct copy
   behavior.

Both projects use C++17, target-based CMake, warning flags for GCC/Clang, CTest,
and out-of-source build directories.
