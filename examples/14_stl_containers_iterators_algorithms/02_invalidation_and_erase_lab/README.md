# invalidation_and_erase_lab

Safe program проверяет:

- `vector::reserve`;
- рост без reallocation внутри capacity;
- forced vector reallocation без разыменования stale pointer;
- erase-remove idiom;
- iterator, возвращаемый `vector::erase`;
- стабильность существующего `list` iterator/reference при вставке;
- `unordered_map::rehash` и различие iterator invalidation от
  reference/pointer stability.

Дополнительно:

- `invalidated_pointer_asan.cpp` намеренно разыменовывает stale pointer после
  reallocation и должен быть пойман ASan;
- `list_sort_fail.cpp` реально не компилируется, потому что `std::sort`
  требует random-access iterators.

Запуск:

```sh
./run_checks.sh
```
