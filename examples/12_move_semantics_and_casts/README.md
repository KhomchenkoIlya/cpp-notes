# Block 12 experiments

Ровно два основных эксперимента:

1. `01_move_ownership_pipeline` — value categories, move-only ownership,
   move construction/assignment, C++17 copy elision и `std::forward`.
2. `02_casts_lab` — `static_cast`, `dynamic_cast`, `const_cast`,
   `reinterpret_cast`, реальные negative compile cases и безопасное сравнение
   с C-style cast.

Основной стандарт — C++17. Оба каталога содержат target-based CMake и
самодостаточный `run_checks.sh`.
