# Эксперимент 01 — compilation pipeline

Запуск:

```bash
./inspect.sh
```

Скрипт отдельно создаёт:

- `hello.ii` — результат preprocessing;
- `hello.s` — assembly;
- `hello.o` — object file;
- `hello` — executable.

Все результаты лежат в `generated/` и не коммитятся.
Главное наблюдение: обычная команда `g++ hello.cpp -o hello` скрывает несколько разных стадий.
