# course_query_pipeline

Показывает:

- `auto` и reference/const deduction;
- `decltype` и различие `decltype(x)` / `decltype((x))`;
- CTAD для `std::pair`;
- structured bindings;
- generic lambda;
- value/reference captures;
- template predicate API;
- `std::optional<CourseBlock>` вместо magic index;
- real compile failure при использовании local variable без capture.

Запуск:

```sh
./run_checks.sh
```

Safe executable дополнительно проходит ASan/UBSan.
