# casts_lab

Проверяются четыре named C++ casts и границы их применения:

- `static_cast` — явная numeric conversion;
- `dynamic_cast` — runtime-checked polymorphic downcast;
- `const_cast` — снятие const view только при действительно mutable underlying object;
- `reinterpret_cast` — контролируемый pointer/integer round trip на текущей implementation.

Дополнительно:

- обычный derived-to-base upcast обходится без cast;
- запись через pointer-to-const реально не компилируется;
- `dynamic_cast` downcast из non-polymorphic base реально не компилируется;
- C-style cast способен убрать const в compile-only example, но программа
  намеренно не выполняет UB-запись.

Запуск:

```sh
./run_checks.sh
```

Safe executable также проверяется под ASan/UBSan.
