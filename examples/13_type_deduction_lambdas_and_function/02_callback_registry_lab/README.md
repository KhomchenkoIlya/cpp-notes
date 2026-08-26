# callback_registry_lab

Показывает:

- единый `std::function<void(const Event&)>` для heterogeneous callable types;
- free function, function object и lambdas в одном registry;
- value capture и mutable state;
- `weak_ptr` capture как lifetime-aware observer;
- empty `std::function` и `std::bad_function_call`;
- template callable для immediate generic call;
- real compile failure при попытке сохранить move-only lambda в
  `std::function` в C++17.

Запуск:

```sh
./run_checks.sh
```

Safe executable дополнительно проходит ASan/UBSan.
