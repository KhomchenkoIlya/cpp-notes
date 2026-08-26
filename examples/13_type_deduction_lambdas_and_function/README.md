# Block 13 experiments

Ровно два основных эксперимента:

1. `01_course_query_pipeline` — `auto`, `decltype`, structured bindings, CTAD,
   generic lambdas и `std::optional` в прикладном query API.
2. `02_callback_registry_lab` — `std::function`, heterogeneous callbacks,
   capture state, `weak_ptr` lifetime boundary и ограничение C++17 для move-only
   targets.

Оба проекта используют C++17, target-based CMake, CTest, negative compile
checks и safe ASan/UBSan run.
