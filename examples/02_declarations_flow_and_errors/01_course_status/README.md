# Эксперимент 1 — course_status

Read-only инспектор проверяет 15 утверждённых пар `lectures/NN_slug.tex` и
`examples/NN_slug/`. Он ничего не создаёт и не изменяет.

Из корня проекта:

```bash
cmake -S examples/02_declarations_flow_and_errors/01_course_status \
      -B examples/02_declarations_flow_and_errors/01_course_status/build
cmake --build examples/02_declarations_flow_and_errors/01_course_status/build
./examples/02_declarations_flow_and_errors/01_course_status/build/course_status
```

Программа ищет корень курса от текущего каталога вверх. Меню позволяет
показать все пары, проверить номер 1–15 или вывести только неполные пары.
Код завершения `0` означает штатный выход; `2` — корень курса не найден.
