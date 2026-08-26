# Эксперимент 2 — control flow + diagnostics lab

Лаборатория содержит корректные deterministic cases и намеренно ошибочные
исходники. Запуск из любого каталога:

```bash
printf 'y\n' | examples/02_declarations_flow_and_errors/02_control_flow_lab/run_lab.sh
```

Скрипт безопасно пересоздаёт только локальные `build/` и `generated/`, затем:

- собирает C++17 targets с `-Wall -Wextra -Wpedantic` и запускает CTest;
- проверяет три ветви `if`, границу цикла и short-circuit;
- подтверждает ожидаемый ненулевой exit code logic-error case;
- получает настоящий warning для `=` вместо `==`;
- получает ожидаемую compile-time ошибку изменения `const`;
- собирает signed-overflow case с UBSan и ожидает диагностику runtime.

Полные compiler outputs сохраняются в `generated/`. Отсутствие сообщений
sanitizer никогда не считается доказательством отсутствия UB.
