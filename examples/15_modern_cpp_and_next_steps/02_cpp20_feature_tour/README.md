# cpp20_feature_tour

Требует C++20.

Показывает `std::ranges::sort` с projection, filter/transform views,
`std::span`, designated initializers, defaulted `<=>`, `std::erase_if`,
`std::string::starts_with` и real compile failure для неправильного порядка
designators.

Запуск:

```sh
./run_checks.sh
```

Safe path дополнительно проходит ASan/UBSan.
