# Block 15 experiments

Ровно два основных C++20 experiments:

1. `01_constexpr_and_concepts_lab` — `constexpr`, `consteval`, `constinit`,
   standard/custom Concepts, `requires`, constrained overloads и два real
   compile-failure cases.
2. `02_cpp20_feature_tour` — ranges/views, projections, `std::span`,
   designated initializers, defaulted `<=>`, `std::erase_if` и
   `std::string::starts_with`.

Основной курс остаётся C++17. Эти каталоги намеренно требуют C++20 и маркируют
это через `target_compile_features(... cxx_std_20)`.
