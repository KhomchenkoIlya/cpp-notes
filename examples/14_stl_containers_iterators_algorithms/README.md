# Block 14 experiments

Ровно два основных эксперимента:

1. `01_course_catalog_pipeline` — прикладной STL pipeline: `vector`,
   `unordered_map`, `map`, `find_if`, `copy_if`, `sort`, `transform`,
   `accumulate`, lambdas и `optional`.
2. `02_invalidation_and_erase_lab` — `vector` capacity/reallocation,
   iterator/pointer invalidation, erase-remove, `list` stability,
   unordered rehash, real ASan diagnostic и compile-failure для
   `std::sort(list.begin(), list.end())`.

Основной стандарт — C++17. Оба каталога используют target-based CMake,
CTest, `-Wall -Wextra -Wpedantic`; safe paths проверяются ASan/UBSan.
