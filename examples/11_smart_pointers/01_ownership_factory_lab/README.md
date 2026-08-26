# ownership_factory_lab

Современная owning-версия Factory Method из блока 08.

Показывает:

- `std::unique_ptr<Product>` как ownership-bearing return type;
- polymorphic destruction через virtual destructor;
- передачу unique ownership через минимальное использование `std::move`;
- `T&` и `T*` как non-owning borrowing API;
- `reset()`;
- реальный compile failure при попытке копировать `unique_ptr`.

Запуск:

```sh
./run_checks.sh
```

`std::move` здесь используется только как bridge к следующему блоку; value
categories и move semantics подробно пока не разбираются.
