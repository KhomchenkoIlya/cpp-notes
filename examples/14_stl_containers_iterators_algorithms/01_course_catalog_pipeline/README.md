# course_catalog_pipeline

Прикладной STL pipeline над каталогом 15 учебных блоков.

Показывает:

- `std::vector<CourseBlock>` как source of truth;
- `std::unordered_map<int, std::size_t>` как индекс;
- `std::find_if`;
- `std::copy_if`;
- `std::sort`;
- `std::transform`;
- `std::accumulate`;
- `std::count_if` / `std::all_of`;
- ordered summary через `std::map`;
- `std::optional<CourseBlock>` для missing query.

Запуск:

```sh
./run_checks.sh
```

Safe executable дополнительно проходит ASan/UBSan.
