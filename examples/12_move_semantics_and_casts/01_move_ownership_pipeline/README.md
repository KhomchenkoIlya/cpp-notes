# move_ownership_pipeline

Эксперимент показывает:

- move-only type с unique-owned resource;
- move constructor и move assignment;
- состояние source после move по contract собственного `Packet`;
- lvalue, const lvalue, temporary/xvalue и named rvalue-reference expression;
- `std::move` как выбор rvalue path;
- forwarding reference + `std::forward`;
- гарантированный C++17 prvalue copy-elision case;
- два реальных compile failures: copy Packet и move из const `unique_ptr`.

Запуск:

```sh
./run_checks.sh
```

Safe executable дополнительно запускается под ASan/UBSan.
