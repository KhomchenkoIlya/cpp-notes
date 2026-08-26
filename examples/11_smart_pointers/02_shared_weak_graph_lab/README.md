# shared_weak_graph_lab

Показывает разные ownership graph:

- два независимых view действительно совместно владеют `AuditSession`;
- `weak_ptr` наблюдает session, не продлевая lifetime;
- mutual `shared_ptr` edges создают strong cycle и logical leak;
- tree использует strong child edges и weak parent back-edge;
- `weak_ptr::lock()` успешен только пока существует strong owner.

Strong-cycle case намеренно запускается как отдельный короткий process и
демонстрирует logical leak через destructor counter. Это не UB и не production
design.

Запуск:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/shared_weak_graph_lab
```
