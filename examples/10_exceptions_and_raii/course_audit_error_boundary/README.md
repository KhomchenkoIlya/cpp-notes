# course_audit_error_boundary

Общая domain-модель проверяет 15 блоков. Отсутствующий или незавершённый
block остаётся обычным `BlockResult`; завершённый курс отдельно проверяет,
что режим `--expect-incomplete` отвергается. I/O failure переносится либо
`Status`, либо exception. Основная
политика утилиты — exception внутри операции и один `catch` на CLI boundary.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/course_audit_error_boundary --root ../../.. --transport exception
```
