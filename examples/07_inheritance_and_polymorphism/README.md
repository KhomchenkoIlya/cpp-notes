# Block 07 experiments

Exactly two experiments accompany the lecture:

1. `01_polymorphism_failure_lab` contrasts static and dynamic dispatch,
   slicing, abstract classes, access, and destructor behavior. Ill-formed and
   unsafe cases are isolated from the normal executable.
2. `02_inheritance_vs_composition_project_lab` audits a real `CourseBlock`
   with an inheritance-based set of checks and a composition-based audit.

Each directory is an independent C++17 target-based CMake project. Build it
out of source as described in its README.
